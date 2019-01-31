#pragma once
#ifndef _UI_COLOR_CORRECT_FORM_H_
#define _UI_COLOR_CORRECT_FORM_H_

#include <QMainWindow>
#include <QLabel>
#include <QFrame>
#include <qimage.h>
#include <qslider.h>
#include <opencv2/opencv.hpp> //Include file for every supported OpenCV function

class ColorCorrectForm : public QWidget
{
	Q_OBJECT
public:
	ColorCorrectForm(QWidget* parent = NULL);

	void setSrcImg(const QImage& srcPixmap);
private slots:
	void valueChanged_r(int val);
	void valueChanged_g(int val);
	void valueChanged_b(int val);
private:
	float getOffsetVal_R();
	float getOffsetVal_G();
	float getOffsetVal_B();

	void applyColorMatrix();

	QWidget* m_formWidget;
	QFrame* m_resultPreviewWidget;
	QLabel* m_resultPreviewLabel;
	QSlider* m_slider_R;
	QSlider* m_slider_G;
	QSlider* m_slider_B;

//	QPixmap m_resultPixmap;
	QImage m_resultImage;
	QImage* m_srcImg;
	cv::Mat m_rgbaImgMat;
	cv::Mat m_resultImgMat;
};

#endif//_UI_COLOR_CORRECT_FORM_H_
