#include "savevideothread.h"
#include <iostream>
#include <QDir>
#include <QDateTime>
#include "imagebuffer.h"

SaveVideoThread::SaveVideoThread()
{
    init();
    std::cout << "SaveVideoThread()" << std::endl;
}

SaveVideoThread::~SaveVideoThread()
{
    isStart=false;
    if(outputVideo)
    {
        outputVideo->closeWriteVideo();
        delete outputVideo;
        outputVideo = NULL;
    }
    if(depthOutputVideo)
    {
        depthOutputVideo->closeWriteVideo();
        delete depthOutputVideo;
        depthOutputVideo = NULL;
    }
    std::cout << "~SaveVideoThread()" << std::endl;
}

void SaveVideoThread::run()
{
    int deltaTime = 0;
    int errorCode = 0;
    QDateTime firstTime = QDateTime::currentDateTime();
    while (isStart)
    {
        deltaTime = firstTime.secsTo(QDateTime::currentDateTime());
        ImageBuffer::usedBuffer.acquire();
        errorCode = outputVideo->saveVideo(ImageBuffer::imageData[frameNumber]);
        errorCode = depthOutputVideo->saveVideo(ImageBuffer::depthData[frameNumber]);
        frameNumber = (frameNumber + 1) % BUFFER_SIZE;
        ImageBuffer::freeBuffer.release();
        if(errorCode != 0)
        {
            emit signalSaveVideoMessage(errorCodeString.getErrCodeString(errorCode));
        }
    }
    closeVideoWrite();
    emit signalSaveVideoMessage(QString("save video path: %1").arg(fileName));
}

//初始化保存数据参数
int SaveVideoThread::initSaveVideoData(const QString &fileNameDir, const cv::Size imageSize, const int frameNumber)
{
    int errCode=0;
    QDir makeDir;
    QString saveName = QTime::currentTime().toString("hhmmsszzz");
    QString depthFileName = fileNameDir + "/" + saveName + "_depth.avi";
    this->fileName = fileNameDir + "/" + saveName + "_rgb.avi";
    this->size = imageSize;
    this->frameNumber = frameNumber;
    int releaseSize = ImageBuffer::freeBuffer.available();
    int availableSize = ImageBuffer::usedBuffer.available();
    if(!makeDir.exists(fileNameDir))
    {
        if(!makeDir.mkpath(fileNameDir))
        {
            std::cout << "make dir fail!" << "fileName:" << fileName.toStdString() << std::endl;
            return -11;
        }
    }
    ImageBuffer::freeBuffer.release(BUFFER_SIZE - releaseSize);
    ImageBuffer::usedBuffer.acquire(availableSize);
    closeVideoWrite();
    errCode = outputVideo->initSaveVideoData(fileName.toStdString().c_str(), size, fps, codec, isColor);
    errCode = depthOutputVideo->initSaveVideoData(depthFileName.toStdString().c_str(), size, fps, codec, false);
    return errCode;
}

void SaveVideoThread::startThread()
{
    isStart = true;
}

void SaveVideoThread::stopThread()
{
    isStart = false;
    ImageBuffer::usedBuffer.release();
}

//关闭视频写入
void SaveVideoThread::closeVideoWrite()
{
    outputVideo->closeWriteVideo();
    depthOutputVideo->closeWriteVideo();
}

void SaveVideoThread::init()
{
    outputVideo = new MyVideoWriter();
    depthOutputVideo = new MyVideoWriter();
    fileName = "";
    size = cv::Size(640, 480);//保存视频文件的大小
    fps = 20;//帧率
    codec = cv::VideoWriter::fourcc('X','V','I','D');//视频编码格式
    isColor = true;//是否是彩色

    isStart = false;
    frameNumber = 0;
}

