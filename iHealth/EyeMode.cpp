#include "EyeMode.h"
#include "Log.h"

EyeMode::EyeMode(boundaryDetection* _bound, ControlCard* _card) :
 isRunning(false),
 isCalibrated(false),
 isSendingData(false),
 mLeftCamIndex(0),
 mRightCamIndex(1),
 bLeftCamOpened(false),
 bRightCamOpened(false),
 handle_eye(0)
{
	pBound = _bound;
	pControlCard = _card;
	pEyeControl = NULL;
	pEyeControl = new EyeControl;
	pLeftEye = NULL;
	pRightEye = NULL;
}

EyeMode::~EyeMode()
{
	if (isRunning == true && handle_eye != 0) {
		isRunning = false;
		::WaitForSingleObject(handle_eye, INFINITE);
	}
	
	delete pEyeControl;
	if (pLeftEye != NULL)
		delete pLeftEye;
	if (pRightEye != NULL)
		delete pRightEye;
}


void EyeMode::start()
{
	pControlCard->SetClutch(ClutchOn);
	pControlCard->SetMotor(MotorOn);
	isSendingData = true;
}

void EyeMode::stop()
{
	if (isSendingData) {
		// pControlCard->SetClutch(ClutchOff);
		pControlCard->SetMotor(MotorOff);

	}
	isSendingData = false;
}

void EyeMode::setVel(double _vel)
{
	pEyeControl->setControlSpeed(_vel);
}

void EyeMode::calibrate()
{
	pEyeControl->calibrate();
	isCalibrated = true;
}
void EyeMode::getRGB24(unsigned char* _data, int _width, int _height, RGB24 _LR)
{
	if (!bLeftCamOpened && !bRightCamOpened) {
		return;
	}

	unsigned char *data = new unsigned char[_width * _height * 3];
	cv::Mat rgb24 = cv::Mat(_height, _width, CV_8UC3, data);
	if (_LR == LEFT && bLeftCamOpened && pLeftEye != NULL)
	{
		cv::Mat tMat = pLeftEye->clone();
		cv::circle(tMat, this->pEyeControl->getLeftEyeCenter(), 20, cv::Scalar(255, 0, 134),10);
		cv::circle(tMat, this->pEyeControl->getLeftCalibCenter(), 20, cv::Scalar(0, 255, 134), 10);
		cv::resize(tMat, rgb24, rgb24.size());
		cv::flip(rgb24, rgb24, 0);
	}
	if (_LR == RIGHT && bRightCamOpened && pRightEye != NULL)
	{
		cv::Mat tMat = pRightEye->clone();
		cv::circle(tMat, this->pEyeControl->getRightEyeCenter(), 20, cv::Scalar(255, 0, 134), 10);
		cv::circle(tMat, this->pEyeControl->getRightCalibCenter(), 20, cv::Scalar(0, 255, 134), 10);
		cv::resize(tMat, rgb24, rgb24.size());
		cv::flip(rgb24, rgb24, 0);
	}
	for (int i = 0; i < _width*_height * 3; i++)
	{
		_data[i] = data[i];
	}
	delete data;
}

void EyeMode::enter()
{
	isRunning = true;
	
	handle_eye = (HANDLE)_beginthreadex(NULL, 0, EyeThread, this, 0, NULL);
}

void EyeMode::exit()
{
	//stop();
	isRunning = false;
	isCalibrated = false;

	if (isSendingData) {
		stop();
	}
	if (handle_eye != 0) {
		::WaitForSingleObject(handle_eye, INFINITE);
		handle_eye = 0;
	}
}

unsigned int __stdcall EyeThread(PVOID pParam)
{
	EyeMode *eyeMode = (EyeMode*)pParam;
	SLOG1("entry EyeThread function begin");
    cv::VideoCapture leftCam(eyeMode->mLeftCamIndex);
	cv::VideoCapture rightCam(eyeMode->mRightCamIndex);
	// cv::VideoCapture leftCam("1.avi"), rightCam("2.avi");
	eyeMode->bLeftCamOpened = leftCam.isOpened();
	eyeMode->bRightCamOpened = rightCam.isOpened(); 

	
	cv::Mat tmp;
	leftCam >> tmp;
	if (eyeMode->pLeftEye == NULL)
	{
		eyeMode->pLeftEye = new cv::Mat(tmp.size(), CV_8UC3);
	}
	rightCam >> tmp;
	if (eyeMode->pRightEye == NULL)
	{
		eyeMode->pRightEye = new cv::Mat(tmp.size(), CV_8UC3);
	}
	
	while (eyeMode->isRunning)  
	{
		leftCam >> *(eyeMode->pLeftEye);
		rightCam >> *(eyeMode->pRightEye);
		
		double currentAngles[2];
		eyeMode->pControlCard->getEncoderData(currentAngles);
		if (!eyeMode->isRunning) {
			break;
		}
		eyeMode->pEyeControl->update(*(eyeMode->pLeftEye), *(eyeMode->pRightEye), currentAngles);
		if (eyeMode->isSendingData && eyeMode->isCalibrated)
		{
			double velocity[2] = { 0, 0 };
			std::pair<double, double> anglesPair;
			anglesPair = eyeMode->pEyeControl->getAngles();

			velocity[0] = anglesPair.first - currentAngles[0];
			velocity[1] = anglesPair.second - currentAngles[1];

			velocity[1] = min(max(velocity[1], -10.0), 10.0);
			velocity[0] = min(max(velocity[0], -10.0), 10.0);
			bool *swithData=eyeMode->pBound->GetSwithData();
			bool  shoulderSwitch[2] = { 0 };
			bool  elbowSwitch[2] = { 0 };

			//获取光电传感器读数
			for (int i = 0; i<2; i++)
			{
				elbowSwitch[i] = swithData[i];
				shoulderSwitch[i] = swithData[2 + i];
			}
			

			eyeMode->pControlCard->VelocityMove(ElbowAxisId, velocity[1]);
			eyeMode->pControlCard->VelocityMove(ShoulderAxisId, velocity[0]);
			char message_tracing[1024];
			sprintf(message_tracing, "In eyectrl Mode output elbow Vel is %0.2f,elbow Swith is %d-%d", velocity[1], elbowSwitch[0], elbowSwitch[1]);
			LOG1(message_tracing);
			
		}
	}
	leftCam.release();
	rightCam.release();
	return 0;
}