// ToneColorCorrection.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <opencv2/opencv.hpp> //Include file for every supported OpenCV function
#include <opencv2/imgproc.hpp>
#include "ToneColorCorrect_cv.h"

using namespace cv;

int testColorCorrection(int argc, const char* argv[]);

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

int affineImg(int argc, const char* argv[])
{
	CommandLineParser parser(argc, argv, "{@input | ../data/lena.jpg | input image}");
	Mat src = imread(parser.get<String>("@input"));
	if (src.empty())
	{
		std::cout << "Could not open or find the image!\n" << std::endl;
		std::cout << "Usage: " << argv[0] << " <Input image>" << std::endl;
		return -1;
	}
	Point2f srcTri[3];
	srcTri[0] = Point2f(0.f, 0.f);
	srcTri[1] = Point2f(src.cols - 1.f, 0.f);
	srcTri[2] = Point2f(0.f, src.rows - 1.f);
	Point2f dstTri[3];
	dstTri[0] = Point2f(0.f, src.rows*0.33f);
	dstTri[1] = Point2f(src.cols*0.85f, src.rows*0.25f);
	dstTri[2] = Point2f(src.cols*0.15f, src.rows*0.7f);
	Mat warp_mat = getAffineTransform(srcTri, dstTri);
	Mat warp_dst = Mat::zeros(src.rows, src.cols, src.type());
	warpAffine(src, warp_dst, warp_mat, warp_dst.size());
	Point center = Point(warp_dst.cols / 2, warp_dst.rows / 2);
	double angle = -50.0;
	double scale = 0.6;
	Mat rot_mat = getRotationMatrix2D(center, angle, scale);
	Mat warp_rotate_dst;
	warpAffine(warp_dst, warp_rotate_dst, rot_mat, warp_dst.size());
	imshow("Source image", src);
	imshow("Warp", warp_dst);
	imshow("Warp + Rotate", warp_rotate_dst);
	waitKey();
	return 0;
}

int main(int argc, const char* argv[])
{
	return testColorCorrection(argc, argv);

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

int testColorCorrection(int argc, const char* argv[])
{
	/*
	Mat tmp(1, 4, CV_32FC1);
	size_t elemSize = tmp.elemSize();
	tmp.at<float>(0, 0) = 32;
	tmp.at<float>(0, 1) = 64;
	tmp.at<float>(0, 2) = 128;
	tmp.at<float>(0, 3) = 0xff;
	Vec4b vTmp(32, 64, 128, 0xff);
	float colorMtxTmp[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	Mat paramMtxTmp(4, 4, CV_32FC1, colorMtxTmp);
	Mat rt = tmp * paramMtxTmp;
	int rows = rt.rows;
	int cols = rt.cols;
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			float val = rt.at<float>(i, j);
			printf(" %f", val);
		}
		printf("\n");
	}
	*/
	cv::String fileName = argv[1];
	Mat img = imread(fileName);
	if (img.size().width <= 0 || img.size().height <= 0)
	{
		std::cout << "Can not open img: " << argv[1] << std::endl;
		cv::waitKey(0);
		return 0;
	}
	size_t t = img.total();
	Mat rgbaImg;
	cvtColor(img, rgbaImg, CV_RGB2RGBA);
	//imshow("rgbaimg", rgbaImg);
	Mat resultImg;
	resultImg.create(rgbaImg.rows, rgbaImg.cols, CV_8UC4);
/*	float colorMtx[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	Mat paramMtx(4, 4, CV_32FC1, colorMtx);*/
	ColorEffectMatrix effectMtx(rgbaImg.type());
	effectMtx.scale(1.2, 1.2, 1.2);
	tcc_ColorCorrect(rgbaImg, resultImg, /*paramMtx*/effectMtx.getMatrix());
	imshow("result", resultImg);
	cv::waitKey(0);
	return 0;
}
