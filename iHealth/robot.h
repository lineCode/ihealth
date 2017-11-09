#pragma once
#include"pasvcontrl.h"
#include "boundarydetection.h"
#include "control_card.h"
#include"activecontrol.h"
#include"emgcontrl.h"
#include "EyeMode.h"


class robot 
{
public:
	robot();
	~robot();
	/************************************************************************/
	/*                           ����ģʽ�ӿ�                                */
	/************************************************************************/
	void clearPasvMove();
	void pushPasvMove(const Teach& move);
	bool isMoving();
	//��ʼ�����˶���index-��ʾ����������
	void startPasvMove(int index);
	//ֹͣ�����˶�
	void stopPasvMove();
	void getCurrentPasvMove(Teach& teach);
	//��ʼʾ��
	void startTeach();
	//����ʾ��
	void stopTeach();
	void getCurrentTeach(Teach& teach);

	//����¶���
	void addPasvMove();
	//�ؽ�����ֵ-���ݽӿڣ�0-�粿�ؽ����أ�1-�ⲿ�ؽ�����
	bool* getSwitchData();
	/************************************************************************/
	/*                           ����ģʽ�ӿ�                                */
	/************************************************************************/
	//��ʼ�����˶�
	void startActiveMove();
	//���������˶�
	void stopActiveMove();
	//���عؽڽǶ�-���ݽӿ�,0-�粿�ؽڽǶȣ�1-�ⲿ�ؽڽǶ�
	void getAngle(double angles[2]);
	//��������-���ݽӿ�
	double getWirstForce();
	bool	isFire();
	void	getPlanePos(short w, short h, double XY[2]);
	void	setDamping(float FC=0.1);	
	/************************************************************************/
	/*                           sEMGģʽ�ӿ�                                */
	/************************************************************************/
	bool isEMGMove();
	//��ʼEMG�˶�
	void startEMGMove();
	//ֹͣEMG�˶�
	void stopEMGMove();
	//��ȡEMG�ź�-���ݽӿڣ�index-�źű�ţ��ֱ�Ϊ0��1��2��3
	double getEMGSignal(int index = 0);
	//���عؽڽǶ�-���ݽӿ�,0-�粿�ؽڽǶȣ�1-�ⲿ�ؽڽǶ�(ͬ��������ģʽ�ӿ�) 
	/************************************************************************/
	/*                           �۶�ģʽ�ӿ�                                */
	/************************************************************************/
	//���عؽڽǶ�-���ݽӿ�,0-�粿�ؽڽǶȣ�1-�ⲿ�ؽڽǶ�(ͬ��������ģʽ�ӿ�) 
	/************************************************************************/
	/*                           �۶�ģʽ�ӿ�                                */
	/************************************************************************/
	void enterEyeMode(); // call it while enter eye mode.
	void exitEyeMode();  // call it while enter eye mode.
	void getLeftRGB24(unsigned char* data, int _width, int _height);  // get image data of left eye
	void getRightRGB24(unsigned char* data, int _width, int _height); // get image data of right eye
	void startEyeMove(); // call it while clicking the start
	void stopEyeMove();  // call it while clicking the stop
	void setEyeVel(double factor); // set velocity
	void eyeCalibrate(); // call it before startEyeControl.

	//��λ
	void resetPos();
	void stopResetPos();

	void setWindow(HWND hWnd);
public:
	pasvContrl *pasvMode;//��������ģʽ
	boundaryDetection *bDetect;//�߽���
	ControlCard *ctrlCard;
	activecontrol *activeCtrl;
	emgcontrl *EMGContrl;
	EyeMode *eyeModeCtl;
	
	HWND m_hWnd = NULL;
	bool m_isPasvModeStart;
	bool m_isActiveModeStart;
	bool m_isEmgModeStart;
};

void move2ORZ();
void getSensorData(bool Travel_Switch[4]);
VOID  CALLBACK OnEyeTimeFunc(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2);