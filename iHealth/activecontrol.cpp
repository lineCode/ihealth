#include "activecontrol.h"
#include<iostream>
#include <process.h>
#include <fstream>
#include <iomanip>

#define FTS_TIME 0.1
using namespace Eigen;
double Force_Fc=0.3;
double Force_a=0.3;
double Force_b=1;

double angle_arm=0;//手臂关节角
double angle_shoul=0;//肩部关节角
double Ud_Arm=0;//力控模式算出手臂的命令速度
double Ud_Shoul=0;//力控模式算出肩部的命令速度
const char *FCH = "Dev3/ai18";//握力采集通道
std::fstream active_data_record;

std::string get_file_name_by_localtime();
void write_record_header();
void write_current_time();
void write_force_sensor(const double *data);
void write_angle();
std::string get_current_time();

activecontrol::activecontrol()
{
	m_hThread = 0;
	m_stop =false;
	isMove = false;
    ctrlCard=NULL;
    ctrlCard=new ControlCard;
    for(int i=0;i<2;i++)
        cmdVel[i]=0;
}
activecontrol:: ~activecontrol()
{
    if(ctrlCard!=NULL)
        delete ctrlCard;
}
unsigned int __stdcall FTSThreadFun(PVOID pParam)
{
	activecontrol *FTS = (activecontrol*)pParam;
	UINT oldTickCount, newTickCount;
	oldTickCount = GetTickCount();
	while (TRUE)
	{
		if (FTS->m_stop)
			break;
		//延时 BOYDET_TIME s
		while (TRUE)
		{
			newTickCount = GetTickCount();
			if (newTickCount - oldTickCount >= FTS_TIME * 1000)
			{
				oldTickCount = newTickCount;
				break;
			}
			else
				SwitchToThread();
		}
		FTS->timerAcquisit();
	}
	//std::cout << "FTSThreadFun Thread ended." << std::endl;
	return 0;
}
void activecontrol::startAcquisit()
{
     //qDebug()<<"activecontrol  Start!";
     mFTWrapper.LoadCalFile();
     mFTWrapper.BiasCurrentLoad(true);
     mFTWrapper.setFUnit();
     mFTWrapper.setTUnit();
	 m_stop = false;

	 m_hThread = (HANDLE)_beginthreadex(NULL, 0, FTSThreadFun, this, 0, NULL);
}
void activecontrol::stopAcquisit()
{
    //qDebug()<<"activecontrol  Stoped!";
	m_stop =true;

	if (m_hThread != 0) {
		::WaitForSingleObject(m_hThread, INFINITE);
		m_hThread = 0;
	}
}
void activecontrol::startMove(boundaryDetection *Angle)
{
    
    ctrlCard->SetMotor(MotorOn);
    ctrlCard->SetClutch(ClutchOn);
	bDetect =Angle;
	isMove = true;
	std::string file_name = get_file_name_by_localtime();
	active_data_record.open(file_name, std::ofstream::app);
	write_record_header();
	startAcquisit();
}
void activecontrol::stopMove()
{
	ctrlCard->SetMotor(MotorOff);
	//ctrlCard->SetClutch(ClutchOff);
	isMove = false;
	if (active_data_record) {
		active_data_record.close();
	}
	stopAcquisit();
}
void activecontrol::timerAcquisit()
{
    double readings[7] = { 0 };
    double distData[6]={0};
    double filtedData[6]={0};
    mFTWrapper.GetForcesAndTorques(readings);
    Raw2Trans(readings,distData);
    Trans2Filter(distData,filtedData);
    FiltedVolt2Vel(filtedData);

	write_current_time();
	write_force_sensor(readings);
	write_angle();

	if (isMove) {
		FTSContrl();
	}

    //qDebug()<<"readings is "<<filtedData[0]<<" "<<filtedData[1]<<" "<<filtedData[2]<<" "<<filtedData[3]<<" "<<filtedData[4]<<" "<<filtedData[5];
}
void activecontrol::Raw2Trans(double RAWData[6],double DistData[6])
{
        MatrixXd A(6,6);
        VectorXd Value_Origi(6);
        VectorXd Value_Convers(6);
        A<< 1,0,0,0,0.1055,0,
            0,0,1,0,-0.0447,0,
            0,-1,0,0.1055,0,-0.0447,
            0,0,0,1,0,0,
            0,0,0,0,0,1,
            0,0,0,0,-1,0;
        for (int i = 0; i < 6; i++)
        {
            if (i<3)
            {
                Value_Origi(i)=RAWData[i+3];
            }
            else
            {
                Value_Origi(i)=RAWData[i-3];
            }

        }
        Value_Convers=A*Value_Origi;
        for(int m=0;m<6;m++)
        {
            DistData[m]=Value_Convers(m);
        }
}
void activecontrol::Trans2Filter(double TransData[6], double FiltedData[6])
{
    double Wc = 5;
    double Ts = 0.05;
    static int i=0;
    static double Last_Buffer[6]={0};
    static double Last2_Buffer[6]={0};
    static double Force_Buffer[6]={0};
    static double Last_FT[6]={0};
    static double Last2_FT[6]={0};
    for (int m = 0; m < 6; m++)
    {
        if (i == 0)
        {
            Last2_Buffer[m] = TransData[m];
            FiltedData[m] = 0;
            i++;
        }
        else if (i == 1)
        {
            Last_Buffer[m] = TransData[m];
            FiltedData[m] = 0;
            i++;
        }
        else
        {
            //二阶巴特沃斯低通滤波器
            Force_Buffer[m] = TransData[m];
            FiltedData[m] = (1/(Wc*Wc+2*1.414*Wc/Ts+4/(Ts*Ts)))*((Wc*Wc)*Force_Buffer[m]
            +(2*Wc*Wc)*Last_Buffer[m]
            + (Wc*Wc)*Last2_Buffer[m]
            - (2*Wc*Wc-8/(Ts*Ts))*Last_FT[m]
            -(Wc*Wc-2*1.414*Wc/Ts+4/(Ts*Ts))*Last2_FT[m]);

            Last2_FT[m] = Last_FT[m];
            Last_FT[m] = FiltedData[m];
            Last2_Buffer[m] = Last_Buffer[m];
            Last_Buffer[m] = Force_Buffer[m];
        }
    }
}
void activecontrol::FiltedVolt2Vel(double FiltedData[6])
{
    MatrixXd Vel(2,1);
    MatrixXd Pos(2,1);
    MatrixXd A(6,6);
    VectorXd Six_Sensor_Convert(6);
	double *angle = bDetect->getAngle();

	angle_shoul = angle[0];
	angle_arm = angle[1];

    Pos(0,0)= angle[0];
    Pos(1,0)= angle[1];

    for (int i = 0; i < 6; i++)
    {
        Six_Sensor_Convert(i)=FiltedData[i];
    }
    damping_control(Six_Sensor_Convert,Pos,Vel,Force_Fc,Force_a,Force_b);
    Ud_Shoul=Vel(0,0);
    Ud_Arm=Vel(1,0);

    if (fabs(Ud_Arm)>15)
    {
        Ud_Arm=10;
    }
    if (fabs(Ud_Shoul)>15)
    {
        Ud_Shoul=10;
    }
}
void activecontrol::FTSContrl()
{
	bool *swithData = bDetect->GetSwithData();
	bool  shoulderSwitch[2] = { 0 };
	bool  elbowSwitch[2] = { 0 };

	//获取光电传感器读数
	for (int i = 0; i<1; i++)
	{
		elbowSwitch[i] = swithData[i];
		shoulderSwitch[i] = swithData[2 + i];
	}
	ctrlCard->VelocityMove(ShoulderAxisId, Ud_Shoul);
	ctrlCard->VelocityMove(ElbowAxisId, Ud_Arm);
}

void activecontrol::getRawAngle(double angle[2])
{
	int ret = 0;
	double raw_arm = 0;
	double raw_shoulder = 0;
	ret = APS_get_position_f(ElbowAxisId, &raw_arm);
	ret = APS_get_position_f(ShoulderAxisId, &raw_shoulder);
	angle[0] = raw_shoulder*Unit_Convert;
	angle[1] = raw_arm*Unit_Convert;
}

double* activecontrol::getAngles()
{
	double *angle = bDetect->getAngle();
	return angle;
}

double activecontrol::getWirstForce()
{
	bool fireOrNot = false;
	int32       error = 0;
	TaskHandle  taskHandle = 0;
	int32       read;
	float64     data[100] = {-1};
	for (int i = 0; i < 100; i++) {
		data[i] = -1;
	}
	DAQmxCreateTask("", &taskHandle);
	DAQmxCreateAIVoltageChan(taskHandle, FCH, "",  DAQmx_Val_RSE, 0, 10.0, DAQmx_Val_Volts, NULL);
	DAQmxCfgSampClkTiming(taskHandle, "", 1000, DAQmx_Val_Rising, DAQmx_Val_FiniteSamps, 100);
	DAQmxStartTask(taskHandle);
	DAQmxReadAnalogF64(taskHandle, 100, 10.0, DAQmx_Val_GroupByChannel, data, 100, &read, NULL);
	DAQmxStopTask(taskHandle);
	DAQmxClearTask(taskHandle);

	return data[50]*20;
}

bool activecontrol::isFire()
{
	bool fireOrNot = false;
	int32       error = 0;
	TaskHandle  taskHandle = 0;
	int32       read;
	float64     data[100] = {0};
	DAQmxCreateTask("", &taskHandle);
	DAQmxCreateAIVoltageChan(taskHandle, FCH, "",  DAQmx_Val_RSE, 0, 10, DAQmx_Val_Volts, NULL);
	DAQmxCfgSampClkTiming(taskHandle, "", 1000, DAQmx_Val_Rising, DAQmx_Val_FiniteSamps, 100);
	DAQmxStartTask(taskHandle);
	DAQmxReadAnalogF64(taskHandle, 100, 10.0, DAQmx_Val_GroupByChannel, data, 100, &read, NULL);
	DAQmxStopTask(taskHandle);
	DAQmxClearTask(taskHandle);
	if(data[50]>1)
		fireOrNot = true;
	return fireOrNot;
}

void activecontrol::getEndsXY(short rangeX,short rangeY, double XY[2])
{
	MatrixXd Theta(5, 1);
	MatrixXd T0h(4, 4);
	VectorXd Pos(2);

	double angle[2] = {0};
	getRawAngle(angle);
	//double *angle = bDetect->getAngle();
	Pos << angle[0], angle[1];
	fwd_geo_coup(Pos, Theta);
	fwd_geo_kineB(Theta, T0h);
	XY[0] = T0h(1, 3)*rangeX*5;
	XY[1] = T0h(0, 3)*rangeY;
}

void activecontrol::setDamping(float FC )
{
	Force_Fc = FC;
}

std::string get_file_name_by_localtime() {
	std::string current_time = get_current_time();
	std::string file_name("d:\\ihealth_data\\");
	file_name += current_time;
	file_name += ".txt";
	return file_name;
}

void write_record_header() {
	if (!active_data_record) {
		return;
	}
	active_data_record << std::setw(12) << "时间" << std::setw(56) << "六维力传感器读数" << std::setw(53)
		<< "肩部关节角度" << std::setw(20) << "肘部关节角度" << "\n";
}

void write_current_time() {
	if (!active_data_record) {
		return;
	}
	std::string s = get_current_time();
	active_data_record << std::left << std::setw(24) << s;
}

void write_force_sensor(const double *data) {
	if (!active_data_record) {
		return;
	}
	for (int i = 0; i < 6; i++) {
		active_data_record << std::setprecision(6) << std::setw(15) << data[i];
	}

}

void write_angle() {
	if (!active_data_record) {
		return;
	}
	active_data_record << std::setprecision(6) << std::setw(20) << angle_shoul << std::setw(20) << angle_arm << "\n";
}

std::string get_current_time() {
	std::string current_time("");
	SYSTEMTIME time;
	GetLocalTime(&time);
	char buf[100];
	sprintf_s(buf, "%.4hd-%.2hd-%.2hd %.2hd-%.2hd-%.2hd", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	current_time += buf;
	return current_time;
}
