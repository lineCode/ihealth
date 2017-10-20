#pragma once
#include <opencv2/opencv.hpp>
#define _EYE_DEBUG_ 0
using namespace std;

class EyeTracking
{
public:
	EyeTracking();
	~EyeTracking();
	int pupilDetect();

	void setEyeImage(const cv::Mat& _image);
	const cv::Mat getEyeImage() const;

	void initTemplate(); // the default function to set template in the beginning
	int setTemplate(const std::string _templateFile); // you can set template by your hand.

	const cv::Point2d getPupilCenter() const;
private:
	int mWidth;
	int mHeight;
	cv::Mat *pEyeImage;
	cv::Mat mTemplateImage;
	cv::Rect mRoi;
	cv::Point2d mPupilCenter;
	int mMatchFlag;      // if find eye. set it to 1
	int mPatchThre;      // patch threshold
	int mPupilMissCount; // record the count of pupil missing during a period
	int mPupilSizeMin;
	int mPupilSizeMax;
};
