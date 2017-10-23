#pragma once
#include"control_card.hpp"
#include"eyeControl.h"
//#include<thread>
#include <process.h>
#include "boundarydetection.h"
#include <iostream>
unsigned int __stdcall EyeThread(PVOID pParam);
class EyeMode
{
public:
	EyeMode(boundaryDetection*, ControlCard*);
	~EyeMode();
	friend unsigned int __stdcall EyeThread(PVOID pParam);
	enum RGB24{LEFT = 0, RIGHT = 1};
	void start();
	void stop();
	void setVel(double _vel);
	void calibrate();
	void getRGB24(unsigned char* , int _width, int _height, RGB24 _LR);
	void enter();
	void exit();

private:
	ControlCard* pControlCard;
	EyeControl* pEyeControl;
	boundaryDetection* pBound;
	cv::Mat* pLeftEye;
	cv::Mat* pRightEye;

	
	bool isRunning;
	bool isCalibrated;
	bool isSendingData;
	int mLeftCamIndex;
	int mRightCamIndex;
	
	HANDLE handle_eye;
	//unsigned nThreadID;

	bool bLeftCamOpened;
	bool bRightCamOpened;
};
