#include "ColorCorrectForm.h"
#include <QtUiTools/quiloader.h>
#include <qfile.h>
#include <Windows.h>
#include "ToneColorCorrect_cv.h"

#pragma comment(lib, "winmm.lib")

ColorCorrectForm::ColorCorrectForm(QWidget* parent)
: QWidget(parent)
, m_formWidget(nullptr)
, m_resultPreviewWidget(nullptr), m_resultPreviewLabel(nullptr)	
, m_slider_R(nullptr), m_slider_G(nullptr), m_slider_B(nullptr)
, m_srcImg(NULL)
{
	QFile uifile("demo.ui");
	uifile.open(QIODevice::ReadOnly);

	QUiLoader loader;
	QWidget* formWidget = loader.load(&uifile, parent);
	QWidget* central = formWidget->findChild<QWidget*>("centralwidget");
	if (central)
	{
		m_formWidget = formWidget;
		m_resultPreviewWidget = m_formWidget->findChild<QFrame*>("resultPreviewFrame");
		m_resultPreviewLabel = m_formWidget->findChild<QLabel*>("resultPreviewLabel");
		m_slider_R = m_formWidget->findChild<QSlider*>("slider_r");
		m_slider_G = m_formWidget->findChild<QSlider*>("slider_g");
		m_slider_B = m_formWidget->findChild<QSlider*>("slider_b");
		connect(m_slider_R, SIGNAL(valueChanged(int)), this, SLOT(valueChanged_r(int)));
		connect(m_slider_G, SIGNAL(valueChanged(int)), this, SLOT(valueChanged_g(int)));
		connect(m_slider_B, SIGNAL(valueChanged(int)), this, SLOT(valueChanged_b(int)));
		m_formWidget->show();
	}
}

void ColorCorrectForm::valueChanged_r(int val)
{
	applyColorMatrix();
}

void ColorCorrectForm::valueChanged_g(int val)
{
	applyColorMatrix();
}

void ColorCorrectForm::valueChanged_b(int val)
{
	applyColorMatrix();
}

float ColorCorrectForm::getOffsetVal_R()
{
	float normOffset = (m_slider_R->value() - m_slider_R->minimum()) / (float)(m_slider_R->maximum() - m_slider_R->minimum());
	// 假设[-1,1]之间伸缩，[-1,0]区间为缩小 [0, 1.0]之间为放大
	float fval = -1.0 + (normOffset * 2);
	return fval;
}

float ColorCorrectForm::getOffsetVal_G()
{
	float normOffset = (m_slider_G->value() - m_slider_G->minimum()) / (float)(m_slider_G->maximum() - m_slider_G->minimum());
	// 假设[-1,1]之间伸缩，[-1,0]区间为缩小 [0, 1.0]之间为放大
	float fval = -1.0 + (normOffset * 2);
	return fval;
}

float ColorCorrectForm::getOffsetVal_B()
{
	float normOffset = (m_slider_B->value() - m_slider_B->minimum()) / (float)(m_slider_B->maximum() - m_slider_B->minimum());
	// 假设[-1,1]之间伸缩，[-1,0]区间为缩小 [0, 1.0]之间为放大
	float fval = -1.0 + (normOffset * 2);
	return fval;
}

void ColorCorrectForm::applyColorMatrix()
{
	float offset_r = getOffsetVal_R();
	float offset_g = getOffsetVal_G();
	float offset_b = getOffsetVal_B();
	ColorEffectMatrix effectMtx(CV_8UC4);
	//effectMtx.scale(fval, 1, 1);
	effectMtx.offset(offset_r, offset_g, offset_b);

	uchar* pbuffer = m_resultImage.bits();
	m_resultImgMat = cv::Mat(cv::Size(m_resultImage.width(), m_resultImage.height()), CV_8UC4, pbuffer);
	DWORD startTS = timeGetTime();
	tcc_ColorCorrect(m_rgbaImgMat, m_resultImgMat, effectMtx.getMatrix());
	DWORD endTS = timeGetTime();
	DWORD spend = endTS - startTS;

	QPixmap resultPixmap = QPixmap::fromImage(m_resultImage);
	m_resultPreviewLabel->setPixmap(resultPixmap);
	//m_resultPreviewLabel->update();
}

void ColorCorrectForm::setSrcImg(const QImage& srcPixmap)
{
	m_srcImg = (QImage*)&srcPixmap;
	m_resultImage = srcPixmap.copy();
	QPixmap resultPixmap = QPixmap::fromImage(m_resultImage);
	if (m_resultPreviewLabel)
	{
		m_resultPreviewLabel->setPixmap(resultPixmap);
	}
	int width = resultPixmap.width();
	int height = resultPixmap.height();
	m_rgbaImgMat = cv::Mat(cv::Size(width, height), CV_8UC4, m_srcImg->bits());
	m_resultImgMat = cv::Mat(cv::Size(width, height), CV_8UC4, m_resultImage.bits());
}