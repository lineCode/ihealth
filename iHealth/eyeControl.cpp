#include "eyeControl.h"

EyeControl::EyeControl(int _step0 , int _step1, double _maxAngle0, double _maxAngle1, double _minSpeed, double _maxSpeed, double _lambda):
	mAngle0(0),
	mAngle1(0),
	mSteps0(_step0),
	mSteps1(_step1),
	mMaxAngle0(_maxAngle0),
	mMaxAngle1(_maxAngle1),
	mMinSpeed(_minSpeed),
	mMaxSpeed(_maxSpeed),
	mLambdaSpeed(_lambda)
{
	pLeftEye = NULL;
	pLeftEye = new EyeTracking;
	pRightEye = NULL;
	pRightEye = new EyeTracking;
	pRobotSurface = new cv::Mat(mSteps0, mSteps1, CV_64FC3);
	initRobotSurface();
}

EyeControl::~EyeControl()
{
	delete pLeftEye;
	delete pRightEye;
	delete pRobotSurface;
}

void EyeControl::setSteps0(int _step)
{
	mSteps0 = _step;
}

void EyeControl::setSteps1(int _step)
{
	mSteps1 = _step;
}

void EyeControl::setMaxAngle0(double _angle)
{
	mMaxAngle0 = _angle;
}

void EyeControl::setMaxAngle1(double _angle)
{
	mMaxAngle1 = _angle;
}

double EyeControl::getAngle0()
{
	return mAngle0;
}

double EyeControl::getAngle1()
{
	return mAngle1;
}

std::pair<double, double> EyeControl::getAngles()
{
	return std::pair<double, double>(mAngle0, mAngle1);
}

void EyeControl::calibrate()
{
	mLeftEyeCalibCenter = mLeftEyeCenter;
	mRightEyeCalibCenter = mRightEyeCenter;
}
cv::Point EyeControl::getLeftEyeCenter()
{
	return mLeftEyeCenter;
}

cv::Point EyeControl::getRightEyeCenter()
{
	return mRightEyeCenter;
}

cv::Point EyeControl::getLeftCalibCenter() {
	return mLeftEyeCalibCenter;
}
cv::Point EyeControl::getRightCalibCenter() {
	return mRightEyeCalibCenter;
}
void EyeControl::setControlSpeed(double _lambda)
{
	mLambdaSpeed = _lambda;
}

void EyeControl::setMinSpeed(double _min)
{
	mMinSpeed = _min;
}

void EyeControl::setMaxSpeed(double _max)
{
	mMaxSpeed = _max;
}

void EyeControl::initRobotSurface()
{
	//declare the homo transform matrix
	double T0sInit[4][4] = { { -1,0,0,0.468 },{ 0,0,-1,0 },{ 0,-1,0,0.23 },{ 0,0,0,1 } };
	cv::Mat T0s(4, 4, CV_64FC1, T0sInit);
	cv::Mat Ts1(4, 4, CV_64FC1, cv::Scalar(0));
	cv::Mat T12(4, 4, CV_64FC1, cv::Scalar(0));
	cv::Mat T23(4, 4, CV_64FC1, cv::Scalar(0));
	cv::Mat T34(4, 4, CV_64FC1, cv::Scalar(0));
	cv::Mat T45(4, 4, CV_64FC1, cv::Scalar(0));
	cv::Mat T05(4, 4, CV_64FC1, cv::Scalar(0));

	//homo transform matrix invariant param init

	Ts1.at<double>(0, 0) = 1;
	Ts1.at<double>(3, 3) = 1;

	T12.at<double>(1, 1) = 1;
	T12.at<double>(3, 3) = 1;


	T23.at<double>(2, 2) = 1;
	T23.at<double>(3, 3) = 1;

	T34.at<double>(0, 3) = -307; //unit: mm
	T34.at<double>(2, 2) = 1;
	T34.at<double>(3, 3) = 1;

	T45.at<double>(0, 0) = 1;
	T45.at<double>(0, 3) = -270.06; //unit:mm
	T45.at<double>(3, 3) = 1;

	//declare the motor control angle and param
	double thetaHatInit[5] = { 0,-7.44,0,26.5885,-21.4734 }; // unit:degree
	double etaInit[5][2] = { { 1,0 },{ 0.88,0 },{ 0,1 },{ 0,1.3214 },{ 0,0.6607 } };
	const cv::Mat thetaHat(5, 1, CV_64FC1, thetaHatInit);
	const cv::Mat eta(5, 2, CV_64FC1, etaInit);
	cv::Mat motorU(2, 1, CV_64FC1, cv::Scalar(0));
	cv::Mat theta(6, 1, CV_64FC1, cv::Scalar(0));


	for (int i = 0; i < mSteps0; i++)
		for (int j = 0; j < mSteps1; j++)
		{
			motorU.at<double>(0) = mMaxAngle0 * i / mSteps0;
			motorU.at<double>(1) = mMaxAngle1 * j / mSteps1;
			theta = thetaHat + eta * motorU;

			Ts1.at<double>(1, 1) = cos(theta.at<double>(0) / 360 * 6.28);
			Ts1.at<double>(1, 2) = -sin(theta.at<double>(0) / 360 * 6.28);
			Ts1.at<double>(2, 1) = sin(theta.at<double>(0) / 360 * 6.28);
			Ts1.at<double>(2, 2) = cos(theta.at<double>(0) / 360 * 6.28);

			T12.at<double>(0, 0) = cos(theta.at<double>(1) / 360 * 6.28);
			T12.at<double>(0, 2) = sin(theta.at<double>(1) / 360 * 6.28);
			T12.at<double>(2, 0) = -sin(theta.at<double>(1) / 360 * 6.28);
			T12.at<double>(2, 2) = cos(theta.at<double>(1) / 360 * 6.28);

			T23.at<double>(0, 0) = cos(theta.at<double>(2) / 360 * 6.28);
			T23.at<double>(0, 1) = -sin(theta.at<double>(2) / 360 * 6.28);
			T23.at<double>(1, 0) = sin(theta.at<double>(2) / 360 * 6.28);
			T23.at<double>(1, 1) = cos(theta.at<double>(2) / 360 * 6.28);

			T34.at<double>(0, 0) = cos(theta.at<double>(3) / 360 * 6.28);
			T34.at<double>(0, 1) = -sin(theta.at<double>(3) / 360 * 6.28);
			T34.at<double>(1, 0) = sin(theta.at<double>(3) / 360 * 6.28);
			T34.at<double>(1, 1) = cos(theta.at<double>(3) / 360 * 6.28);

			T45.at<double>(1, 1) = cos(theta.at<double>(4) / 360 * 6.28);
			T45.at<double>(1, 2) = sin(theta.at<double>(4) / 360 * 6.28);
			T45.at<double>(2, 1) = -sin(theta.at<double>(4) / 360 * 6.28);
			T45.at<double>(2, 2) = cos(theta.at<double>(4) / 360 * 6.28);

			T05 = T0s * Ts1 * T12 * T23 * T34 * T45;

			pRobotSurface->at<cv::Vec3d>(i, j)[0] = T05.at<double>(0, 3);
			pRobotSurface->at<cv::Vec3d>(i, j)[1] = T05.at<double>(1, 3);
			pRobotSurface->at<cv::Vec3d>(i, j)[2] = T05.at<double>(2, 3);
		}
}

void EyeControl::update(const cv::Mat& _leftEyeData, const cv::Mat& _rightEyeData, double* _currentAngles, int controlMode)
{
	if (_currentAngles[0] > mMaxAngle0)_currentAngles[0] = mMaxAngle0;
	else if (_currentAngles[0] < 0)_currentAngles[0] = 0;
	if (_currentAngles[1] > mMaxAngle1)_currentAngles[1] = mMaxAngle1;
	else if (_currentAngles[1] < 0)_currentAngles[1] = 0;
	pLeftEye->setEyeImage(_leftEyeData);
	pRightEye->setEyeImage(_rightEyeData);

	// get pupil center

	pRightEye->pupilDetect();
	mRightEyeCenter = pRightEye->getPupilCenter();
	pLeftEye->pupilDetect();
	mLeftEyeCenter = pLeftEye->getPupilCenter();

	cv::Point2d BiMeanDirection;
	cv::Point3d controlDirection;

	cv::Point pupilDirectionL, pupilDirectionR, depthDireciton;
	pupilDirectionL = mLeftEyeCenter - mLeftEyeCalibCenter;
	pupilDirectionR = mRightEyeCenter - mRightEyeCalibCenter;

	BiMeanDirection = (pupilDirectionL + pupilDirectionR) / 2;

	//    double deptIndex = 2 ;


	int minDisI = 1000;
	int minDisJ = 1000;
	double minDistance = 10000;

	/*feedback control: head pose to the object, use eye to guide the robot to the object,
	* as which means that the robot move just towards the  opposite direction of the
	* pupilDirection
	*/
	if (0 != controlMode)
	{
		controlDirection.y = BiMeanDirection.x;
		controlDirection.x = BiMeanDirection.y;
		// controlDirection.z = -deptIndex*(pupilDirectionL.x - pupilDirectionR.x);
		controlDirection.z = 0;
	}

	/*feedforward control: consider the movement of the eye as a direction of exactly the
	* movement of the robot
	*/
	else
	{
		controlDirection.y = -BiMeanDirection.x;
		controlDirection.x = -BiMeanDirection.y;
		// controlDirection.z = deptIndex*(pupilDirectionL.x - pupilDirectionR.x);
		controlDirection.z = 0;
	}
	//current point in the workspace index

	double controlSpeed = (mMaxSpeed - mMinSpeed) * mLambdaSpeed + mMinSpeed;
	controlDirection = controlDirection / norm(controlDirection) * controlSpeed;

	int i = _currentAngles[0] / mMaxAngle0 * (mSteps0 - 1);
	int j = _currentAngles[1] / mMaxAngle1 * (mSteps1 - 1);

	/*search the nearest point in the robot workspace in a neighr area of 1/20 of the total workspace to the next controled point
	init the current point as the nearest point*/
	const int radius = 10;
	int iRange[2] = { (i - mSteps0 / radius) < 0 ? 0 : (i - mSteps0 / radius), (i + mSteps0 / radius) < mSteps0 ? (i + mSteps0 / radius) : mSteps0 };
	int jRange[2] = { (j - mSteps1 / radius) < 0 ? 0 : (j - mSteps1 / radius), (j + mSteps1 / radius) < mSteps1 ? (j + mSteps1 / radius) : mSteps1 };
	for (int tempI = iRange[0]; tempI < iRange[1]; tempI++) {
		for (int tempJ = jRange[0]; tempJ < jRange[1]; tempJ++) {
			if (tempI == i&&tempJ == j)
				continue;
			if (norm(pRobotSurface->at<cv::Point3d>(i, j) + controlDirection - pRobotSurface->at<cv::Point3d>(tempI, tempJ)) < minDistance) {
				minDisI = tempI;
				minDisJ = tempJ;
				minDistance = norm(pRobotSurface->at<cv::Point3d>(i, j) + controlDirection - pRobotSurface->at<cv::Point3d>(tempI, tempJ));
			}
		}
	}
	mAngle0 = minDisI * mMaxAngle0 / mSteps0;
	mAngle1 = minDisJ * mMaxAngle1 / mSteps1;
}