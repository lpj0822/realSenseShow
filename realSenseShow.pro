QT       += core gui
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = realSenseShow
TEMPLATE = app

include(utility/utility.pri)

SOURCES += main.cpp\
        controlwindow.cpp \
    realsensevideoshowwindow.cpp \
    realsensevideothread.cpp \
    realsenseimageconvertor.cpp \
    imagebuffer.cpp \
    savevideothread.cpp \
    bodyposedetection.cpp

HEADERS  += controlwindow.h \
    realsensevideoshowwindow.h \
    realsensevideothread.h \
    realsenseimageconvertor.h \
    imagebuffer.h \
    savevideothread.h \
    bodyposedetection.h

RESOURCES += \
    style.qrc \
    images.qrc
	
RC_ICONS = appico.ico

INCLUDEPATH+= D:\opencv\opencv342\build\include
INCLUDEPATH+= D:\RealSense\include

LIBS += D:\opencv\opencv342\build\x64\vc14\lib\opencv_world342.lib
LIBS += D:\RealSense\lib\x64\realsense2.lib
