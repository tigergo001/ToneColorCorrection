#include "ToneColorCorrect_cv.h"
#include <opencv2/core/saturate.hpp>

ColorEffectMatrix::ColorEffectMatrix(int type)
{
	int cn = CV_MAT_CN(type);
	int depth = CV_MAT_DEPTH(type);
	m_colorMatrix = cv::Mat::eye(cn + 1, cn + 1, CV_32FC1);
}

ColorEffectMatrix::~ColorEffectMatrix()
{
}

cv::Mat ColorEffectMatrix::getMatrix() const
{
	return m_colorMatrix;
}

bool ColorEffectMatrix::scale(float r, float g, float b, float shift)
{
	if (shift<-1.0f || shift>1.0f)
		return false;
	cv::Mat tmp = cv::Mat::eye(m_colorMatrix.rows, m_colorMatrix.cols, m_colorMatrix.type());

	tmp.at<float>(0, 0) = b;
	tmp.at<float>(1, 1) = g;
	tmp.at<float>(2, 2) = r;
	tmp.at<float>(tmp.rows - 1, 0) = shift;
	tmp.at<float>(tmp.rows - 1, 1) = shift;
	tmp.at<float>(tmp.rows - 1, 2) = shift;

	m_colorMatrix = m_colorMatrix * tmp;
	return true;
}

bool ColorEffectMatrix::constrast(float constrast, float shift)
{
	return false;
}

bool ColorEffectMatrix::offset(float r, float g, float b)
{
	if (r<-1.0f || r>1.0f)
		return false;
	if (g<-1.0f || g>1.0f)
		return false;
	if (b<-1.0f || b>1.0f)
		return false;
	cv::Mat tmp = cv::Mat::eye(m_colorMatrix.rows, m_colorMatrix.cols, m_colorMatrix.type());
	tmp.at<float>(tmp.rows - 1, 2) = r;
	tmp.at<float>(tmp.rows - 1, 1) = g;
	tmp.at<float>(tmp.rows - 1, 0) = b;
	m_colorMatrix = m_colorMatrix * tmp;
	return true;
}

int tcc_ColorCorrect(const cv::Mat& src, cv::Mat& dst, cv::InputArray matrix)
{
	int type = src.type();
	int depth = CV_MAT_DEPTH(type);
	int cn = CV_MAT_CN(type);
	int srcMemType = CV_MAT_TYPE(type);

	int dstType = dst.type();
	int depthDst = CV_MAT_DEPTH(dstType);
	int cnDst = CV_MAT_CN(dstType);
	
	int mtxType = matrix.type();
	int mtxDepth = CV_MAT_DEPTH(mtxType);
	int mtxCN = CV_MAT_CN(mtxType);
	int paramType = CV_MAT_TYPE(mtxType);
	if (type != dstType)
		return -1;
	cv::Mat paramMtx = matrix.getMat();
	if ((srcMemType%8) == 0)
	{
		// 因为颜色变换矩阵的RGB offset取值范围是[-1, 1]
		// 如果要将矩阵应用于8U的数据则需要转换到[-255, 255]区间内
		paramMtx.at<float>(paramMtx.rows - 1, 0) = paramMtx.at<float>(paramMtx.rows - 1, 0) * 255;
		paramMtx.at<float>(paramMtx.rows - 1, 1) = paramMtx.at<float>(paramMtx.rows - 1, 1) * 255;
		paramMtx.at<float>(paramMtx.rows - 1, 2) = paramMtx.at<float>(paramMtx.rows - 1, 2) * 255;
	}
	cv::Mat srcMtx = cv::Mat::ones(1, cn + 1, CV_32FC1);
	//srcMtx.create(1, 4, CV_32FC1);
	cv::Mat dstMtx = cv::Mat::ones(1, cnDst + 1, CV_32FC1);
	//dstMtx.create(1, 4, CV_32FC1);
	for (int i = 0; i < src.rows; i++)
	{
		const uchar* pSrcRow = src.ptr(i);
		uchar* pDstRow = dst.ptr(i);
		for (int j = 0; j < src.cols; j++)
		{
			//cv::Mat srcMtx_1(1, cn + 1, CV_32FC1, (char*)pSrcRow, 1);
			srcMtx.at<float>(0, 0) = (float)pSrcRow[0];
			srcMtx.at<float>(0, 1) = (float)pSrcRow[1];
			srcMtx.at<float>(0, 2) = (float)pSrcRow[2];
			srcMtx.at<float>(0, 3) = (float)pSrcRow[3];
			dstMtx = srcMtx * paramMtx;
			pDstRow[0] = cv::saturate_cast<uchar>(dstMtx.at<float>(0, 0));
			pDstRow[1] = cv::saturate_cast<uchar>(dstMtx.at<float>(0, 1));
			pDstRow[2] = cv::saturate_cast<uchar>(dstMtx.at<float>(0, 2));
			pDstRow[3] = cv::saturate_cast<uchar>(dstMtx.at<float>(0, 3));
			pSrcRow += 4;
			pDstRow += 4;
		}
	}
	return 0;
}