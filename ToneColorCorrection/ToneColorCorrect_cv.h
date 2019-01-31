/**
*	@file		ToneColorCorrect_cv.h
*	@author		zhuqingquan
*	@brief		基于OpenCV库实现图片颜色调节功能，包括调节RGB、对比度、饱和度、色度
*	@created	2018/09/28
**/
#pragma once

#include <opencv2/opencv.hpp> //Include file for every supported OpenCV function

/**
 * @name	ColorEffectMatrix
 * @brief	生成颜色调节运算中所使用的颜色特效矩阵
 **/
class ColorEffectMatrix
{
public:
	/**
	 *	@name		ColorEffectMatrix
	 *	@brief		构造函数
	 *	@param[in]	int type	应用此运算的像素的格式，比如RGB或者RGBA
	 *				该参数与Mat构造函数中的type参数使用相同的值
	 **/
	ColorEffectMatrix(int type);
	
	/**
	*	@name		~ColorEffectMatrix
	*	@brief		析构
	**/
	~ColorEffectMatrix();

	/**
	*	@name		getMatrix
	*	@brief		获取最终的颜色特效矩阵
	*	@return		cv::Mat 颜色特效矩阵
	**/
	cv::Mat getMatrix() const;
	
	/**
	*	@name		scale
	*	@brief		像素拉伸，线性变换，可用于调节对比度
	*	@param[in]	float r 红色拉伸值，不限定取值范围
	*	@param[in]	float g 绿色拉伸值，不限定取值范围
	*	@param[in]	float b 蓝色拉伸值，不限定取值范围
	*	@param[in]	float shift r,g,b颜色偏移量，取值范围：[-1, 1]
	*	@return		bool true--成功 false--失败，参数错误
	**/
	bool scale(float r, float g, float b, float shift = 0.0f);

	/**
	*	@name		constrast
	*	@brief		对比度调节
	*	@param[in]	float constrast 对比度调整强度，不限定取值范围
	*	@param[in]	float shift r,g,b颜色偏移量，取值范围：[-1, 1]
	*	@return		bool true--成功 false--失败，参数错误
	**/
	bool constrast(float constrast, float shift = 0.0f);

	/**
	*	@name		offset
	*	@brief		颜色偏移
	*	@param[in]	float r 红色偏移量，取值范围：[-1, 1]
	*	@param[in]	float g 绿色偏移量，取值范围：[-1, 1]
	*	@param[in]	float b 蓝色偏移量，取值范围：[-1, 1]
	*	@return		bool true--成功 false--失败，参数错误
	**/
	bool offset(float r, float g, float b);

private:
	cv::Mat m_colorMatrix;
};

int tcc_ColorCorrect(const cv::Mat& src, cv::Mat& dst, cv::InputArray matrix);