HEADERS     = 	imageviewer.h \
				ColorCorrectForm.h
				
SOURCES       = imageviewer.cpp \
                Demo.cpp \
				ColorCorrectForm.cpp 
				
QT += widgets
QT += printsupport
QT += uitools

INCLUDEPATH += ../ToneColorCorrection \
				D:/3rdLibs/opencv/include 
				
LIBS += -L"../bin/debug" -lToneColorCorrection -L"D:/3rdLibs/opencv/x86/vc14/lib" -lopencv_core330d -lopencv_highgui330d

#opencv_highgui330d.lib
#opencv_imgcodecs330d.lib
#opencv_imgproc330d.lib

CONFIG(debug, debug|release){
	DESTDIR = ../bin/debug
} else {
	DESTDIR = ../bin/release
}

# install
target.path = ../
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS imageviewer.pro
sources.path = ./
INSTALLS += target sources

#symbian: include($$QT_SOURCE_TREE/examples/symbianpkgrules.pri)

#wince*: {
#   DEPLOYMENT_PLUGIN += qjpeg qmng qgif
#}
