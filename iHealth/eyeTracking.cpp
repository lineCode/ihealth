#include "eyeTracking.h"
#include <assert.h>
EyeTracking::EyeTracking() :
mRoi(cv::Rect(0, 0, 0, 0)),
mPupilCenter(cv::Point2d(0, 0)),
mMatchFlag(0),
mPatchThre(0),
mPupilMissCount(0),
mPupilSizeMin(1),
mPupilSizeMax(INT_MAX)
{
	initTemplate();
	pEyeImage = NULL;
}

EyeTracking::~EyeTracking()
{
	if (pEyeImage != NULL)
		delete pEyeImage;
}

int EyeTracking::pupilDetect()
{
	int method = cv::TemplateMatchModes::TM_CCOEFF_NORMED; //template match method setting
	// if the roi rectangle is not valid, return
	if (mRoi.width == 0 || mRoi.height == 0) {
		mRoi.x = 0;
		mRoi.y = 0;
		mRoi.width = mWidth;
		mRoi.height = mHeight;
		return -1;
	}
	cv::Mat tmp = pEyeImage->clone();
	cv::Mat roiImage;
	roiImage = tmp(mRoi);
	cv::Mat obj;
	mTemplateImage.copyTo(obj);


	cv::Mat matchResult, scaledObj;        // template matching feature map , and the scaled template
	double minVal, maxVal;                 // the max and min matching value in the feature map
	cv::Point minLoc, maxLoc;              // the position of the worst matching and the best matching
	cv::Mat matchPatch;                    // the best match patch in the roi img
	cv::Rect matchRect;                    // the outer rectangle of the match patch img
	double pupilSizeRatioRef2Roi = 1;      // the ratio of the matching template size to the roi img
	double pupilCheckValue;                // check the area perimeter ratio of the match patch
	cv::Point pupilCenterInMatchPatch;
	cv::Moments pupilCenterMoment;         // get the pupil center cordinates in the match patch

	int shortLen =
		(roiImage.size().width < roiImage.size().height)
		?
		roiImage.size().width
		:
		roiImage.size().height;

	if (roiImage.channels() > 1) {
		cv::cvtColor(roiImage, roiImage, cv::COLOR_BGR2GRAY);
	}
	if (obj.channels() > 1) {
		cv::cvtColor(obj, obj, cv::COLOR_BGR2GRAY);
	}

	pupilSizeRatioRef2Roi = (2 * mPupilSizeMax > shortLen) ? 1 : (2 * mPupilSizeMax / shortLen);

	//scale the obj template to the proper size in the ROI image of eye image
	while (1) {
		pupilSizeRatioRef2Roi *= 0.75; // scaling the template obj from 0.95 size of roiImg till a size of 10 pixel,0.95 times step by step to match the pupil best
		cv::resize(obj, scaledObj,
			cv::Size(int(pupilSizeRatioRef2Roi / 2.0 * shortLen) * 2 + 1, int(pupilSizeRatioRef2Roi / 2.0 * shortLen) * 2 + 1),
			0, 0, cv::INTER_LINEAR);
		if (scaledObj.size().width < 2 * mPupilSizeMin || scaledObj.size().height < 2 * mPupilSizeMin)
		{
			mMatchFlag = 0;
			mPupilMissCount++;
			// If the pupil was not found for 30 frames ,set the search area the whole original img and the path threshold history
			if (mPupilMissCount > 30) {
				mPupilMissCount = 0;
				mPatchThre = 0;
				mRoi = cv::Rect(0, 0, pEyeImage->cols, pEyeImage->rows);
				mPupilSizeMin = 15;
				mPupilSizeMax = 10000;
			}
			return -1;
		}

		vector<vector<cv::Point>> contours; // the all contours(external,without hole in one contour) in the binarized img
		double maxArea = 0;                 // the area of the connect-component with the max area in the best match patch
		std::vector<cv::Point> maxContour;  // the contour of the max area connect-component
		int morphOpenSize;                  //open morph operation in the best match patch to avoid noise and eyelash
		cv::matchTemplate(roiImage, scaledObj, matchResult, method); //match the scaled template in the roi image

		// find out the position of the most matching patch
		cv::minMaxLoc(matchResult, &minVal, &maxVal, &minLoc, &maxLoc); //find the location of the best match patch
		matchRect.x = maxLoc.x;
		matchRect.y = maxLoc.y;
		matchRect.width = scaledObj.size().width;
		matchRect.height = scaledObj.size().height;
		roiImage(matchRect).copyTo(matchPatch); // clone the best match patch to a new img:matchPatch
		if (!mPatchThre) {
			cv::threshold(matchPatch, matchPatch, 0, 255, cv::THRESH_OTSU); // binarize the best match patch, assign 1 to pupil area element and 0 to the other
			matchPatch = 255 - matchPatch;
		}
		else {
			cv::threshold(matchPatch, matchPatch, mPatchThre, 255, cv::THRESH_BINARY_INV);
		}
		morphOpenSize = (scaledObj.size().width / 10 < 1) ? 1 : scaledObj.size().width / 10; // limit the morphOpenSize as 1/5 size of the pupil
		morphologyEx(matchPatch, matchPatch, cv::MORPH_OPEN,
			cv::getStructuringElement(cv::MORPH_RECT, cv::Size(morphOpenSize, morphOpenSize)),
			cv::Point(-1, -1), 1,
			cv::BORDER_CONSTANT,
			cv::morphologyDefaultBorderValue());

		// find out the maxArea contour
		cv::Mat matchPatchClone = matchPatch.clone();
		cv::findContours(matchPatchClone, contours, CV_RETR_EXTERNAL, cv::CHAIN_APPROX_NONE); // find the contours in the binarized best match

		for (size_t i = 0; i < contours.size(); i++) // search for the max area contour in the match patch
		{
			double area = cv::contourArea(contours[i]);
			if (area > maxArea)
			{
				maxArea = area;
				maxContour = contours[i];
			}
		}
		// If the ratio of the max area of a connected component to the area of whole match patch is less than a limit,
		// match failing.
		if (maxArea < matchPatch.rows * matchPatch.cols * 0.6 * 3.14 / 16) continue;

		// If the max area of black blob is to closer to the
		// edge of the match patch, match failing.
		cv::Rect convexRect = boundingRect(maxContour);
		if (convexRect.x < 0.1 * matchPatch.cols ||
			convexRect.y < 0.1 * matchPatch.rows ||
			convexRect.x + convexRect.width > 0.9 * matchPatch.cols ||
			convexRect.y + convexRect.height > 0.9 * matchPatch.rows)
			continue;

		// get the convex hull of the max area of black blob in the match patch
		// and check if the hull is almost an ellipse
		// if not so similar to, match failing
		std::vector<cv::Point> maxContourHull;
		convexHull(maxContour, maxContourHull, false, true);
		double hullArea = contourArea(maxContourHull);
		double hullArcLength = arcLength(maxContourHull, true);
		pupilCheckValue = hullArea * 4 * 3.14 / (hullArcLength * hullArcLength);

		if (pupilCheckValue < 0.7) continue;
		else {
			mMatchFlag = 1; //calculate the pupil center in the match patch
			pupilCenterMoment = moments(maxContourHull);
			pupilCenterInMatchPatch.x = pupilCenterMoment.m10 / pupilCenterMoment.m00;
			pupilCenterInMatchPatch.y = pupilCenterMoment.m01 / pupilCenterMoment.m00;

			mPupilSizeMin = 0.2 * matchPatch.rows;
			mPupilSizeMax = 1.5 * matchPatch.rows;
			mPupilMissCount = 0;

			//set the match patch threshold to the average value  of the balck blob of the max area in the match patch, additional plus a offset 10.
			cv::Scalar meanValInPupilBlob;
			std::vector<vector<cv::Point>> maskContour(1);
			maskContour[0] = maxContour;
			cv::Mat mask(matchRect.height, matchRect.width, CV_8UC1, cv::Scalar(0));
			drawContours(mask, maskContour, -1, cv::Scalar(255), CV_FILLED);

			meanValInPupilBlob = mean(roiImage(matchRect), mask);

			mPatchThre = meanValInPupilBlob.val[0] + 15;
			mPupilCenter.x = mRoi.x + matchRect.x + pupilCenterInMatchPatch.x;
			mPupilCenter.y = mRoi.y + matchRect.y + pupilCenterInMatchPatch.y;

			if (mRoi.width == mWidth && mRoi.height == mHeight) {
				mRoi.x = (matchRect.x - 0.5 * matchRect.width) < 0 ? 0 : (matchRect.x - 0.5 * matchRect.width);
				mRoi.y = (matchRect.y - 0.5 * matchRect.height) < 0 ? 0 : (matchRect.y - 0.5 * matchRect.height);
				mRoi.width = (matchRect.x + 1.5 * matchRect.width) > mWidth ? (mWidth - mRoi.x) : matchRect.x + 1.5 * matchRect.width - mRoi.x;
				mRoi.height = (matchRect.y + 1.5 * matchRect.height) > mHeight ? (mHeight - mRoi.y) : matchRect.y + 1.5 * matchRect.height - mRoi.y;
			}
			else {
				int tempX = (mRoi.x + matchRect.x - 0.5*matchRect.width) < 0 ?
					0 : ((matchRect.x - 0.5*matchRect.width) < 0 ?
					(mRoi.x + matchRect.x - 0.5*matchRect.width) : mRoi.x);
				int tempY = (mRoi.y + matchRect.y - 0.5*matchRect.height) < 0 ?
					0 : ((matchRect.y - 0.5*matchRect.height) < 0 ?
					(mRoi.y + matchRect.y - 0.5*matchRect.height) : mRoi.y);
				int tempWidth = (mRoi.x + matchRect.x + 1.5*matchRect.width) > mWidth ?
					(mWidth - tempX) : ((matchRect.x + 1.5*matchRect.width) > mRoi.width ?
					(mRoi.x + matchRect.x + 1.5*matchRect.width - tempX) :
					(mRoi.x + mRoi.width - tempX));
				int tempHeight = (mRoi.y + matchRect.y + 1.5*matchRect.height) > mHeight ?
					(mHeight - tempY) : ((matchRect.y + 1.5*matchRect.height) > mRoi.height ?
					(mRoi.y + matchRect.y + 1.5*matchRect.height - tempY) :
					(mRoi.y + mRoi.height - tempY));
				mRoi.x = tempX;
				mRoi.y = tempY;
				mRoi.width = tempWidth;
				mRoi.height = tempHeight;
			}
			break;
		}
	}
	return 0;
}

void EyeTracking::setEyeImage(const cv::Mat& _image)
{
	if (pEyeImage == NULL) {
		mWidth = _image.cols;
		mHeight = _image.rows;
		pEyeImage = new cv::Mat(_image.size(), CV_8UC3);
	}
	*pEyeImage = _image.clone();
}

const cv::Mat EyeTracking::getEyeImage() const
{
	return cv::Mat(*pEyeImage);
}

void EyeTracking::initTemplate()
{
	this->mTemplateImage = cv::Mat::ones(201, 201, CV_8UC1) * 255;
	circle(this->mTemplateImage, cv::Point(101, 101), 50, 0, -1, cv::LINE_8);
}

int EyeTracking::setTemplate(const std::string _templateFile)
{
	this->mTemplateImage = cv::imread(_templateFile, 0);
	if (mTemplateImage.data == NULL) return -1;
	return 0;
}

const cv::Point2d EyeTracking::getPupilCenter() const
{
	return this->mPupilCenter;
}
