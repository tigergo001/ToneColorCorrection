/**
*	@file		ToneColorCorrect_cv.h
*	@author		zhuqingquan
*	@brief		����OpenCV��ʵ��ͼƬ��ɫ���ڹ��ܣ���������RGB���Աȶȡ����Ͷȡ�ɫ��
*	@created	2018/09/28
**/
#pragma once

#include <opencv2/opencv.hpp> //Include file for every supported OpenCV function

/**
 * @name	ColorEffectMatrix
 * @brief	������ɫ������������ʹ�õ���ɫ��Ч����
 **/
class ColorEffectMatrix
{
public:
	/**
	 *	@name		ColorEffectMatrix
	 *	@brief		���캯��
	 *	@param[in]	int type	Ӧ�ô���������صĸ�ʽ������RGB����RGBA
	 *				�ò�����Mat���캯���е�type����ʹ����ͬ��ֵ
	 **/
	ColorEffectMatrix(int type);
	
	/**
	*	@name		~ColorEffectMatrix
	*	@brief		����
	**/
	~ColorEffectMatrix();

	/**
	*	@name		getMatrix
	*	@brief		��ȡ���յ���ɫ��Ч����
	*	@return		cv::Mat ��ɫ��Ч����
	**/
	cv::Mat getMatrix() const;
	
	/**
	*	@name		scale
	*	@brief		�������죬���Ա任�������ڵ��ڶԱȶ�
	*	@param[in]	float r ��ɫ����ֵ�����޶�ȡֵ��Χ
	*	@param[in]	float g ��ɫ����ֵ�����޶�ȡֵ��Χ
	*	@param[in]	float b ��ɫ����ֵ�����޶�ȡֵ��Χ
	*	@param[in]	float shift r,g,b��ɫƫ������ȡֵ��Χ��[-1, 1]
	*	@return		bool true--�ɹ� false--ʧ�ܣ���������
	**/
	bool scale(float r, float g, float b, float shift = 0.0f);

	/**
	*	@name		constrast
	*	@brief		�Աȶȵ���
	*	@param[in]	float constrast �Աȶȵ���ǿ�ȣ����޶�ȡֵ��Χ
	*	@param[in]	float shift r,g,b��ɫƫ������ȡֵ��Χ��[-1, 1]
	*	@return		bool true--�ɹ� false--ʧ�ܣ���������
	**/
	bool constrast(float constrast, float shift = 0.0f);

	/**
	*	@name		offset
	*	@brief		��ɫƫ��
	*	@param[in]	float r ��ɫƫ������ȡֵ��Χ��[-1, 1]
	*	@param[in]	float g ��ɫƫ������ȡֵ��Χ��[-1, 1]
	*	@param[in]	float b ��ɫƫ������ȡֵ��Χ��[-1, 1]
	*	@return		bool true--�ɹ� false--ʧ�ܣ���������
	**/
	bool offset(float r, float g, float b);

private:
	cv::Mat m_colorMatrix;
};

int tcc_ColorCorrect(const cv::Mat& src, cv::Mat& dst, cv::InputArray matrix);