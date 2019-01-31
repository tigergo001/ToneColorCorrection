// ToneColorCorrection.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp> //Include file for every supported OpenCV function

using namespace cv;

Mat getHistImg(const MatND& hist)
{
	double maxVal = 0;
	double minVal = 0;

	//找到直方图中的最大值和最小值
	minMaxLoc(hist, &minVal, &maxVal, 0, 0);
	int histSize = hist.rows;
	Mat histImg(histSize, histSize, CV_8U, Scalar(255));
	// 设置最大峰值为图像高度的90%
	int hpt = static_cast<int>(0.9*histSize);

	for (int h = 0; h<histSize; h++)
	{
		float binVal = hist.at<float>(h);
		int intensity = static_cast<int>(binVal*hpt / maxVal);
		line(histImg, Point(h, histSize), Point(h, histSize - intensity), Scalar::all(0));
	}

	return histImg;
}

int main(int argc, const char* argv[])
{
	cv::String fileName = argv[1];
	Mat img = imread(fileName);
	if (img.size().width <= 0 || img.size().height <= 0)
	{
		std::cout << "Can not open img: " << argv[1] << std::endl;
		cv::waitKey(0);
		return 0;
	}

	Mat grayImg;
	cvtColor(img, grayImg, CV_BGR2GRAY);

	const int channelIndex[1] = { 0 };
	const int histSize[1] = { 256 };
	float hrange[2] = { 0, 255 };
	const float* ranges[1] = { hrange };

	MatND hist;
	calcHist(&grayImg, 1, channelIndex, Mat(), hist, 1, histSize, ranges);

	Mat histImg = getHistImg(hist);

	cv::namedWindow("origin-img", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("gray-img", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("hist-img", cv::WINDOW_AUTOSIZE);
	imshow("origin-img", img);
	imshow("gray-img", grayImg);
	imshow("hist-img", histImg);
	
	cv::waitKey(0);

    return 0;
}

