#include "boundarydetection.h"
#include<math.h>
#include <iostream>
#include <process.h> 
#include <tchar.h>
#include "robot.h"

#define BOYDET_TIME 0.1 
#define BrokenTorque0 20
#define BrokenTorque1 20

#define PULL_LIMIT0 100
#define PULL_LIMIT1 100
#define PULL_LIMIT2 100
#define PULL_LIMIT3 100

double rawTorqueData[5]={0};
double raw_pull_data[20] = { 0 };

const char *TCH = "Dev3/ai19:20";//���زɼ�ͨ��
const char *pull_sensor_channel = "Dev3/ai12:15";

boundaryDetection::boundaryDetection()
{
	for (int i = 0; i < 4; i++) {
		Pull_Sensor[i] = 0;
		Travel_Switch[i] = 0;
	}
	for (int j = 0; j < 2; j++) {
		angle[j] = 0;
		Torque_Sensor[j]=0;
		vel[j] = 0;
	}
	for (int k = 0; k < 3; k++) {
		m_Pos_A[k] = 0;
		m_Pos_S[k] = 0;
	}
	m_emergency_stop_status = true;
	vel_i = 0;
    m_stop=false;
    ctrlCardOfTorque=NULL;
    ctrlCardOfTorque=new contrlCard;
	//����һ�������Ļ��������Ϊ���ź�״̬��
	hMutex = CreateMutex(NULL, FALSE, NULL);	hAngleMutex= CreateMutex(NULL, FALSE, NULL);
	hVelMutex= CreateMutex(NULL, FALSE, NULL);
}
boundaryDetection::~boundaryDetection()
{
    //qDebug()<<"boundaryDetection destroyed.";
    if(ctrlCardOfTorque!=NULL)
        delete ctrlCardOfTorque;  
	stopBydetect();
}
unsigned int __stdcall BydetectThreadFun(PVOID pParam)
{
	boundaryDetection *Bydetect = (boundaryDetection*)pParam;
	UINT oldTickCount, newTickCount;
	oldTickCount = GetTickCount();
	while (TRUE)
	{
        if(Bydetect->m_stop)
            break;
		//��ʱ BOYDET_TIME s
		while (TRUE)
		{
			newTickCount = GetTickCount();
			if (newTickCount - oldTickCount >= BOYDET_TIME * 1000)
			{
				oldTickCount = newTickCount;
				break;
			}
			else
			{
				SwitchToThread();
				::Sleep(5);
			}
		}
		if(Bydetect->m_stop)
			break;

		WaitForSingleObject(Bydetect->hMutex, INFINITE);
		Bydetect->getSensorData();
		ReleaseMutex(Bydetect->hMutex);

		WaitForSingleObject(Bydetect->hAngleMutex, INFINITE);
		Bydetect->getEncoderData();
		ReleaseMutex(Bydetect->hAngleMutex);

		WaitForSingleObject(Bydetect->hVelMutex, INFINITE);
		Bydetect->getJointVel();
		ReleaseMutex(Bydetect->hVelMutex);

		Bydetect->getTorqueData();
		Bydetect->GetPullSensorData();

		Bydetect->check();
    }
    std::cout<<"boundaryDetection Thread ended."<<std::endl;
	return 0;
}
void boundaryDetection::getSensorData()
{
    I32 DI_Group = 0; // If DI channel less than 32
    I32 DI_Data = 0; // Di data
    I32 di_ch[__MAX_DI_CH];
    I32 returnCode = 0; // Function return code
    returnCode = APS_read_d_input(0, DI_Group, &DI_Data);
    for (int i = 0; i < __MAX_DI_CH; i++)
        di_ch[i] = ((DI_Data >> i) & 1);

	Travel_Switch[0]=di_ch[16];//0�ŵ��ORG�ź�-�ⲿ���
	Travel_Switch[1]=di_ch[17];//0�ŵ��MEL�ź�-�ⲿ���

	Travel_Switch[2]=di_ch[18];//1�ŵ��ORG�ź�-�粿���
	Travel_Switch[3]=di_ch[19];//1�ŵ��MEL�ź�-�粿���

	m_emergency_stop_status = di_ch[20];
}
 bool* boundaryDetection::GetSwithData()
 {
	 WaitForSingleObject(hMutex, INFINITE);
     bool *output=Travel_Switch;
	 ReleaseMutex(hMutex);

     return output;
 }
 double* boundaryDetection::getAngle()
 {
	 double *output = NULL;
	 WaitForSingleObject(hAngleMutex, INFINITE);
	 output = angle;
	 ReleaseMutex(hAngleMutex);
	 return output;
 }
 void boundaryDetection::startBydetect()
 {
	 HANDLE handle;
	 handle = (HANDLE)_beginthreadex(NULL, 0, BydetectThreadFun, this, 0, NULL);
	 //WaitForSingleObject(handle, INFINITE);
 }
 void boundaryDetection::stopBydetect()
 {
	 m_stop = true;
	 //_endthreadex(m_Handle);
 }
void boundaryDetection::getTorqueData()
{
    TaskHandle  taskHandle = 0;
    int32       read=0;
    int status =0;
    status =DAQmxCreateTask("TorqueDataTask", &taskHandle);
    status =DAQmxCreateAIVoltageChan(taskHandle, TCH, "TorqueDataChannel", DAQmx_Val_RSE, -10, 10, DAQmx_Val_Volts, NULL);

    status =DAQmxCfgSampClkTiming(taskHandle, "OnboardClock", 1000, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 10);

    status =DAQmxStartTask(taskHandle);
    status =DAQmxReadAnalogF64(taskHandle, 10, 0.2, DAQmx_Val_GroupByScanNumber, rawTorqueData,20, &read, NULL);
    status =DAQmxStopTask(taskHandle);
    status =DAQmxClearTask(taskHandle);

    for(int j=0;j<2;j++){
        Torque_Sensor[j]=rawTorqueData[j]*2;
    }
}

void boundaryDetection::GetPullSensorData() {
	TaskHandle taskHandle = 0;
	int32 read = 0;
	int status = 0;

	status = DAQmxCreateTask("PullDataTask", &taskHandle);
	status = DAQmxCreateAIVoltageChan(taskHandle, pull_sensor_channel, "PullDataChannel", DAQmx_Val_RSE, -10, 10, DAQmx_Val_Volts, NULL);
	status = DAQmxCfgSampClkTiming(taskHandle, "OnboardClock", 1000, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 10);
	status = DAQmxStartTask(taskHandle);
	status = DAQmxReadAnalogF64(taskHandle, 5, 0.2, DAQmx_Val_GroupByScanNumber, raw_pull_data, 20, &read, NULL);
	status = DAQmxStopTask(taskHandle);
	status = DAQmxClearTask(taskHandle);
	for (int i = 0; i < 4; i++) {
		Pull_Sensor[i] = raw_pull_data[i] * 2;
	}
}

void boundaryDetection::getTorqueData(double data[2])
{
	TaskHandle  taskHandle = 0;
	int32       read=0;
	int status =0;
	status =DAQmxCreateTask("TorqueDataTask", &taskHandle);
	status =DAQmxCreateAIVoltageChan(taskHandle, TCH, "TorqueDataChannel", DAQmx_Val_RSE, -10, 10, DAQmx_Val_Volts, NULL);

	status =DAQmxCfgSampClkTiming(taskHandle, "OnboardClock", 1000, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 10);

	status =DAQmxStartTask(taskHandle);
	status =DAQmxReadAnalogF64(taskHandle, 10, 0.2, DAQmx_Val_GroupByScanNumber, rawTorqueData,20, &read, NULL);
	status =DAQmxStopTask(taskHandle);
	status =DAQmxClearTask(taskHandle);

	for(int j=0;j<2;j++){
		data[j]=rawTorqueData[j]*2;
	}
}
void boundaryDetection::getEncoderData()
{
	int ret = 0;
	double raw_arm = 0;
	double  raw_shoulder = 0;
	ret = APS_get_position_f(elbowAxisId, &raw_arm);
	ret = APS_get_position_f(shoudlerAxisId, &raw_shoulder);
	angle[0] = raw_shoulder*Unit_Convert;
	angle[1] = raw_arm*Unit_Convert;
}
void boundaryDetection::getJointVel()
{	
	double Sample_Vel_Time = 0.1;
	if (vel_i >= 3)
	{
		vel_i = 0;
	}
	WaitForSingleObject(hAngleMutex, INFINITE);
	m_Pos_S[vel_i] = angle[0];//�粿�Ƕ�ֵ
	m_Pos_A[vel_i] = angle[1];//�ⲿ�Ƕ�ֵ
	ReleaseMutex(hAngleMutex);
	//��λ�����ٶȵ��㷨
	
	switch (vel_i)
	{
	case 0:
		(m_Pos_A[1] == 0) ? vel[1] = 0 :
			vel[1] = (m_Pos_A[0] - m_Pos_A[1]) / (BOYDET_TIME * 2);
		(m_Pos_S[1] == 0) ? vel[0] = 0 :
			vel[0] = (m_Pos_S[0] - m_Pos_S[1]) / (BOYDET_TIME * 2);
		break;
	case 1:
		(m_Pos_A[2] == 0) ? vel[1] = 0 :
			vel[1] = (m_Pos_A[1] - m_Pos_A[2]) / (BOYDET_TIME * 2);
		(m_Pos_S[2] == 0) ? vel[0] = 0 :
			vel[0] = (m_Pos_S[1] - m_Pos_S[2]) / (BOYDET_TIME * 2);
		break;
	case 2:
		vel[1] = (m_Pos_A[2] - m_Pos_A[0]) / (BOYDET_TIME * 2);
		vel[0] = (m_Pos_S[2] - m_Pos_S[0]) / (BOYDET_TIME * 2);
		break;
	}	
	vel_i++;
}
double* boundaryDetection::getVel()
{
	WaitForSingleObject(hVelMutex, INFINITE);
	double *output = vel;
	ReleaseMutex(hVelMutex);
	return output;
}

void boundaryDetection::check() {
	//��ͣ������ʾ
	if (!m_emergency_stop_status) {
		int ret = ::MessageBox(m_hWnd, _T("��⵽��ͣ���ر����£���������Ƿ���������, ���ȷ���ر������"), _T("Ӳ����ͣ"), MB_OK);
		if (ret == IDOK) {
			::PostMessage(m_hWnd, WM_CLOSE, NULL, NULL);
		}
	}

	double timeTorque1 = fabs(Torque_Sensor[1]);
	if (timeTorque1 > BrokenTorque0) {
		ctrlCardOfTorque->ServeTheMotor(OFF);
		ctrlCardOfTorque->SetClutch(COFF);

		char message_tracing[1024];
		sprintf(message_tracing, "Shoulder Torque is out of range, BrokenTorque is %0.2f", timeTorque1);
		LOG1(message_tracing);
		int ret = ::MessageBox(m_hWnd, _T("�ⲿ����ֵ���ڶֵ���Ƿ�λ�����ȡ���ر����"), _T("���ر���"), MB_YESNOCANCEL);
		switch (ret) {
		case IDYES:
			if (m_pRobot != NULL) {
				m_pRobot->resetPos();
			}
			break;
		case IDCANCEL:
			::PostMessage(m_hWnd, WM_CLOSE, NULL, NULL);
			break;
		default:
			break;
		}

	}

	double timeTorque0 = fabs(Torque_Sensor[0]);
	if (timeTorque0 > BrokenTorque1) {
		ctrlCardOfTorque->ServeTheMotor(OFF);
		ctrlCardOfTorque->SetClutch(COFF);

		char message_tracing[1024];
		sprintf(message_tracing, "Elbow Torque is out of range, BrokenTorque is %0.2f", timeTorque0);
		LOG1(message_tracing);

		int ret = ::MessageBox(m_hWnd, _T("�粿����ֵ���ڶֵ���Ƿ�λ�����ȡ���ر����"), _T("���ر���"), MB_YESNOCANCEL);
		switch (ret) {
		case IDYES:
			if (m_pRobot != NULL) {
				m_pRobot->resetPos();
			}
			break;
		case IDCANCEL:
			::PostMessage(m_hWnd, WM_CLOSE, NULL, NULL);
			break;
		default:
			break;
		}
	}

	double time_pull[4]{ 0 };
	double pull_limit[4]{ PULL_LIMIT0, PULL_LIMIT1, PULL_LIMIT2, PULL_LIMIT3 };
	for (int i = 0; i < 4; i++) {
		time_pull[i] = Pull_Sensor[i];
		if (time_pull[i] > pull_limit[i]) {
			ctrlCardOfTorque->ServeTheMotor(OFF);
			ctrlCardOfTorque->SetClutch(COFF);

			char message_tracing[1024];
			sprintf(message_tracing, "pull sensor %d is out of range, BrokenTorque is %0.2f", i, timeTorque0);
			LOG1(message_tracing);

			int ret = ::MessageBox(m_hWnd, _T("��˿���������ڶֵ���Ƿ�λ�����ȡ���ر����"), _T("��������"), MB_YESNOCANCEL);
			switch (ret) {
			case IDYES:
				if (m_pRobot != NULL) {
					m_pRobot->resetPos();
				}
				break;
			case IDCANCEL:
				::PostMessage(m_hWnd, WM_CLOSE, NULL, NULL);
				break;
			default:
				break;
			}
		}
	}
}


 void boundaryDetection::Set_hWnd(HWND hWnd) {
	 m_hWnd = hWnd;
 }

 void boundaryDetection::SetRobot(robot *pRobot) {
	 m_pRobot = pRobot;
 }
