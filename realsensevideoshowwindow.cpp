#pragma execution_character_set("utf-8")
#include "realsensevideoshowwindow.h"
#include <QVBoxLayout>
#include <QString>
#include <QDir>

RealSenseVideoShowWindow::RealSenseVideoShowWindow(QWidget *parent) : QWidget(parent)
{
    initData();
    initUI();
    initConnect();
    updateImage();
}

RealSenseVideoShowWindow::~RealSenseVideoShowWindow()
{
    if(saveVideoThread)
    {
        saveVideoThread->stopThread();
        saveVideoThread->wait();
        delete saveVideoThread;
        saveVideoThread = NULL;
    }
    if(videoThread)
    {
        videoThread->stopThread();
        videoThread->wait();
        delete videoThread;
        videoThread = NULL;
    }
}

int RealSenseVideoShowWindow::showVideo()
{
    int errorCode = videoThread->startThread();
    if(errorCode == 0)
    {
        videoThread->startShowVideo();
        videoThread->start();
    }
    return errorCode;
}
//关闭视频
void RealSenseVideoShowWindow::closeShowVideo()
{
    videoThread->stopThread();
}

void RealSenseVideoShowWindow::setIsRecordVideo(bool isRecord)
{
    if(videoThread->getIsOpenVideo())
    {
        videoThread->setIsRecord(isRecord);
        recordVideo(isRecord);
    }
}

//是否打开视频
bool RealSenseVideoShowWindow::getIsOpenVideo()
{
    return videoThread->getIsOpenVideo();
}

void RealSenseVideoShowWindow::slotVideoImage(QImage image, bool isOpen)
{
    this->currentImage = image;
    if(!isOpen)
    {
        currentImage = QImage(":/images/images/play.png");
    }
    updateImage();
}

void RealSenseVideoShowWindow::updateImage()
{
    imageShowLabel->setPixmap(QPixmap::fromImage(currentImage));
}

void RealSenseVideoShowWindow::recordVideo(bool isRecord)
{
    if(isRecord)
    {
        QString saveVideoDir = QDir::currentPath() + "/record";
        cv::Size imageSize = videoThread->getVideoSize();
        saveVideoThread->initSaveVideoData(saveVideoDir, imageSize, videoThread->getFrameNumber());
        saveVideoThread->startThread();
        saveVideoThread->start();
    }
    else
    {
        saveVideoThread->stopThread();
    }
}

void RealSenseVideoShowWindow::initData()
{
    currentImage = QImage(":/images/images/play.png");
    videoThread = new RealSenseVideoThread();
    saveVideoThread = new SaveVideoThread();
}

void RealSenseVideoShowWindow::initUI()
{
    imageShowText = new QLabel(tr("左图: RGB图 右图: Depth图"));
    imageShowText->setAlignment(Qt::AlignCenter);

    imageShowLabel = new QLabel();
    imageShowLabel->setAlignment(Qt::AlignCenter);
    imageShowScrollArea = new QScrollArea(this);
    imageShowScrollArea->setAlignment(Qt::AlignCenter);
    imageShowScrollArea->setWidgetResizable(true);
    imageShowScrollArea->viewport()->setBackgroundRole(QPalette::Dark);
    imageShowScrollArea->viewport()->setAutoFillBackground(true);
    //referenceScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);  //控件大小 小于 视窗大小时，默认不会显示滚动条
    //referenceScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);    //强制显示滚动条。
    imageShowScrollArea->setWidget(imageShowLabel);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(10);
    mainLayout->addWidget(imageShowScrollArea);
    mainLayout->addWidget(imageShowText);

    this->setLayout(mainLayout);
    this->setMinimumSize(1000, 500);
    this->setWindowTitle(tr("RGB-D图像采集"));
}

void RealSenseVideoShowWindow::initConnect()
{
    connect(videoThread, &RealSenseVideoThread::signalVideoData, this, &RealSenseVideoShowWindow::slotVideoImage);
    connect(videoThread, &RealSenseVideoThread::signalVideoMessage, this, &RealSenseVideoShowWindow::signalVideoMessage);
    connect(saveVideoThread, &SaveVideoThread::signalSaveVideoMessage, this, &RealSenseVideoShowWindow::signalVideoMessage);
}
