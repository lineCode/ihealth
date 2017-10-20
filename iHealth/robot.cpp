#include "robot.h"
#include <windows.h>
#include "mmsystem.h"
#pragma comment(lib,"winmm.lib")
#define RESET_TIMER 100
MMRESULT Mtimer_ID=0;
UINT wAccuracy=0;
contrlCard ORZCtrlCard;

robot::robot()
{
	pasvMode = NULL;
	pasvMode = new pasvContrl;
	bDetect = NULL;
	bDetect = new boundaryDetection;
	bDetect->SetRobot(this);

	ctrlCard = NULL;
	ctrlCard = new contrlCard;
	activeCtrl = NULL;
	activeCtrl = new activecontrol;
	EMGContrl = NULL;
	EMGContrl = new emgcontrl;
	ctrlCard->Initial();
	eyeModeCtl = NULL;
	eyeModeCtl = new EyeMode(bDetect, ctrlCard);
	
	bDetect->startBydetect();
	m_isActiveModeStart = false;
	m_isEmgModeStart = false;
	m_isPasvModeStart = false;
}

robot::~robot()
{
	if (pasvMode != NULL)
		delete pasvMode;
	if (bDetect != NULL) {
		delete bDetect;
	}
	if (ctrlCard != NULL)
		delete ctrlCard;
	if (NULL != activeCtrl)
		delete activeCtrl;
	if (NULL != EMGContrl)
		delete EMGContrl;
	if (NULL != eyeModeCtl)
		delete eyeModeCtl;
}
VOID  CALLBACK OnEyeTimeFunc(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	move2ORZ();
}

void robot::clearPasvMove()
{
	pasvMode->clearMove();
}

void robot::pushPasvMove(const Teach& move)
{
	pasvMode->pushMove(move);
}

bool robot::isMoving()
{
	return pasvMode->Moving();
}

void robot::startPasvMove(int index)
{
	//if (ctrlCard->IsCardInitial()) {
	if (m_isPasvModeStart == false) {
		m_isPasvModeStart = true;
		pasvMode->beginMove(index, bDetect);
	}
	//}
}
void robot::stopPasvMove()
{
	//if (ctrlCard->IsCardInitial()) {
	if (m_isPasvModeStart == true) {
		m_isPasvModeStart = false;
		pasvMode->stopMove();
	}
	//}
}
void robot::getCurrentPasvMove(Teach& teach)
{
	//if (ctrlCard->IsCardInitial()) {
		pasvMode->getCurrentMove(teach);
	//}
}
void robot::startTeach()
{
	//if (ctrlCard->IsCardInitial()) {
		pasvMode->startTeach(bDetect);
	//}
}
void robot::stopTeach()
{
	//if (ctrlCard->IsCardInitial()) {
		pasvMode->stopTeach();
	//}
}

void robot::getCurrentTeach(Teach& teach)
{
	//if (ctrlCard->IsCardInitial()) {
		pasvMode->getCurrentTeach(teach);
	//}
}

void robot::addPasvMove()
{
	//if (ctrlCard->IsCardInitial()) {
		pasvMode->addMovement();
	//}
}
void robot::startActiveMove()
{
	//if (ctrlCard->IsCardInitial()) {
	if (!m_isActiveModeStart) {
		activeCtrl->startMove(bDetect);
		m_isActiveModeStart = true;
	}
	//}
}
void robot::stopActiveMove()
{
	//if (ctrlCard->IsCardInitial()) {
	if (m_isActiveModeStart) {
		activeCtrl->stopMove();
		m_isActiveModeStart = false;
	}
	//}
}
void robot::getAngle(double angles[2])
{
	//double *output = NULL;
	//if (ctrlCard->IsCardInitial()) {
	activeCtrl->getRawAngle(angles);
	//}
	//return output;
}

double robot::getWirstForce()
{
	double output;
	
	output = activeCtrl->getWirstForce();
	
	return output;
}
bool robot::isFire()
{
	//if (ctrlCard->IsCardInitial()) {
		return activeCtrl->isFire();
	//}
	//return false;
}

void robot::getPlanePos(short w, short h, double XY[2])
{
	activeCtrl->getEndsXY(w, h, XY);
}

void robot::setDamping(float FC/* =0.1 */)
{
	activeCtrl->setDamping(FC);
}


void robot::setEyeVel(double factor)
{
	eyeModeCtl->setVel(factor);
}
void robot::eyeCalibrate()
{
	eyeModeCtl->calibrate();
}
void robot::startEyeMove()
{
	eyeModeCtl->start();
}
void robot::stopEyeMove()
{
	eyeModeCtl->stop();
}
void robot::enterEyeMode()
{
	eyeModeCtl->enter();
}
void robot::exitEyeMode()
{
	eyeModeCtl->exit();
}

void robot::getLeftRGB24(unsigned char* data, int _width, int _height)
{
	eyeModeCtl->getRGB24(data, _width, _height, EyeMode::LEFT);
}
void robot::getRightRGB24(unsigned char* data, int _width, int _height)
{
	eyeModeCtl->getRGB24(data, _width, _height, EyeMode::RIGHT);
}

void robot::resetPos()
{
	//�򿪵���������
	ctrlCard->ServeTheMotor(ON);
	ctrlCard->SetClutch(CON);
	// ����ȫ�ֶ�ʱ��;
	TIMECAPS tc;
	if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) == TIMERR_NOERROR)
	{
		wAccuracy = MIN(MAX(tc.wPeriodMin, 1), tc.wPeriodMax);//�ֱ��ʵ�ֵ���ܳ���ϵͳ��ȡֵ��Χ 
		timeBeginPeriod(wAccuracy);// ����timeBeginPeriod�������ö�ʱ���ķֱ���
								   // ���ö�ʱ��  
		Mtimer_ID = timeSetEvent(RESET_TIMER, wAccuracy, (LPTIMECALLBACK)OnEyeTimeFunc, DWORD(1), TIME_PERIODIC);
	}
}

void robot::setWindow(HWND hWnd)
{
	m_hWnd = hWnd;
	bDetect->Set_hWnd(hWnd);
	pasvMode->Set_hWnd(hWnd);
	ctrlCard->Set_hWnd(hWnd);
	EMGContrl->m_hWnd = hWnd;
}

bool robot::isEMGMove()
{
	return EMGContrl->isBeginMove;
}

void robot::startEMGMove()
{
	//if (ctrlCard->IsCardInitial()) {
	if (m_isEmgModeStart == false) {
		m_isEmgModeStart = true;
		EMGContrl->start(bDetect);
	}	
	//}
}
void robot::stopEMGMove()
{
	//if (ctrlCard->IsCardInitial()) {
	if (m_isEmgModeStart == true) {
		m_isEmgModeStart = false;
		EMGContrl->stop();
	}
	//}
}

double robot::getEMGSignal(int index /* = 0 */)
{
	return EMGContrl->getRawData(index);
}

void robot::stopResetPos()
{
	if(Mtimer_ID!=0)
		timeKillEvent(Mtimer_ID);
	if(wAccuracy!=0)
		timeEndPeriod(wAccuracy);
}


void getSensorData(bool Travel_Switch[4])
{
	I32 DI_Group = 0; // If DI channel less than 32
	I32 DI_Data = 0; // Di data
	I32 di_ch[__MAX_DI_CH];
	I32 returnCode = 0; // Function return code
	returnCode = APS_read_d_input(0, DI_Group, &DI_Data);
	for (int i = 0; i < __MAX_DI_CH; i++)
		di_ch[i] = ((DI_Data >> i) & 1);

	Travel_Switch[0] = di_ch[16];//0�ŵ��ORG�ź�-�ⲿ���
	Travel_Switch[1] = di_ch[17];//0�ŵ��MEL�ź�-�ⲿ���

	Travel_Switch[2] = di_ch[18];//1�ŵ��ORG�ź�-�粿���
	Travel_Switch[3] = di_ch[19];//1�ŵ��MEL�ź�-�粿���
}

void move2ORZ()
{
	//��ȡ��翪����Ϣ
	bool RobotORZ[4] = {0};
	getSensorData(RobotORZ);
	static int i = 0;
	//�ж�0�ŵ��-�粿 �Ƿ�����λ��������������λ�˶���ʼ���������ֹͣ�˶�
	if (RobotORZ[2] != true)
	{
		APS_vel(shoudlerAxisId, 1, 5 / Unit_Convert, 0);
	}
	else
	{
		APS_stop_move(shoudlerAxisId);
	}
	//�ж�1�ŵ��-�ⲿ�Ƿ�����λ��������������λ�˶���ʼ���������ֹͣ�˶�
	if (RobotORZ[0] != true)
	{
		APS_vel(elbowAxisId, 1, 5 / Unit_Convert, 0);
	}
	else
	{
		APS_stop_move(elbowAxisId);
	}

	if ((RobotORZ[0] == true) && (RobotORZ[2] == true))
	{
		//�����Ժ�ص��
		ORZCtrlCard.ServeTheMotor(OFF);
		ORZCtrlCard.SetClutch(COFF);
		ORZCtrlCard.SetParamZero();
		//ֹͣ��ʱ��
		if (Mtimer_ID != 0)
			timeKillEvent(Mtimer_ID);
		if (wAccuracy != 0)
			timeEndPeriod(wAccuracy);
		char message_tracing[1024];
		sprintf(message_tracing, "Reset position completed");
		LOG1(message_tracing);
	}
}
