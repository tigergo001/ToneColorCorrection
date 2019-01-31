// Demo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <QApplication>
#include "imageviewer.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	ImageViewer imageViewer;
	imageViewer.show();
	return app.exec();
}

