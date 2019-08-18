#include "realsensevideothread.h"
#include <iostream>
#include <QDebug>
#include "imagebuffer.h"

RealSenseVideoThread::RealSenseVideoThread()
{
    init();
    std::cout << "RealSenseVideoThread()" << std::endl;
}

RealSenseVideoThread::~RealSenseVideoThread()
{
    std::cout << "~RealSenseVideoThread()" << std::endl;
}

void RealSenseVideoThread::run()
{
    cv::Mat stitchFrame;
    cv::Mat depthShowData;
    cv::Mat tempFrame;
    rs2::align alignTo(RS2_STREAM_COLOR);
    while(isStart)
    {
        if(isOpen)
        {
            if(isReadVideo)
            {
                try{
                    rs2::frameset data = pipe.wait_for_frames(); // Wait for next set of frames from the camera
                    // Make sure the frameset is spatialy aligned
                    // (each pixel in depth image corresponds to the same pixel in the color image)
                    rs2::frameset alignedData = alignTo.process(data);
                    rs2::frame depthImage = alignedData.get_depth_frame();
                    rs2::frame rgbImage = alignedData.get_color_frame();
                    readDepthFrame = imageConvertor.depthFrameToMeters(pipe, depthImage);
                    //readDepthFrame = imageConvertor.frameToMat(depthImage);
                    readRGBFrame = imageConvertor.frameToMat(rgbImage);

                    depthShowData = imageConvertor.scaleDepthMat(readDepthFrame);
                    setImageBuffer(readRGBFrame, depthShowData);

                    // Colorize depth image with white being near and black being far
                    // This will take advantage of histogram equalization done by the colorizer
                    rs2::frame colorDepth = depthImage.apply_filter(colorMap);
                    depthShowData = imageConvertor.frameToMat(colorDepth);
                    stitchFrame = imageConvertor.RGBImageStitchDepth(readRGBFrame, depthShowData);
                    cv::cvtColor(stitchFrame, tempFrame, cv::COLOR_BGR2RGB);//Qt中支持的是RGB图像, OpenCV中支持的是BGR
                    frameToimage = QImage((uchar*)(tempFrame.data), tempFrame.cols, tempFrame.rows, QImage::Format_RGB888);
                    emit signalVideoData(frameToimage, isOpen);
                }
                catch (const rs2::error & e)
                {
                    std::cout << "RealSense read error:" << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
                    isReadVideo = false;
                    emit signalVideoMessage(errorCodeString.getErrCodeString(-42));
                }
            }
        }
    }
    try{
        // Start streaming with default recommended configuration
        pipe.stop();
    }catch (const rs2::error & e)
    {
        std::cout << "RealSense stop error: " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
        emit signalVideoMessage(errorCodeString.getErrCodeString(-41));
    }
    isReadVideo = false;
    isOpen = false;
    emit signalVideoData(QImage(), isOpen);
}

//开始运行线程
int RealSenseVideoThread::startThread()
{
    try{
        // Start streaming with default recommended configuration
        realSenseProfile = pipe.start(realSenseConfig);
        rs2::video_stream_profile rgbProfile = realSenseProfile.get_stream(RS2_STREAM_COLOR).as<rs2::video_stream_profile>();
        rs2_intrinsics intrinsics = rgbProfile.get_intrinsics();
        emit signalVideoMessage(QString("open camera, intrinsics: fx=%1 fy=%2 cx=%3 cy=%4")\
                                .arg(intrinsics.fx).arg(intrinsics.fy).arg(intrinsics.ppx).arg(intrinsics.ppy));
        isStart = true;
        isOpen = true;
        frameNumber = 0;
    }catch (const rs2::error & e)
    {
        isOpen = false;
        std::cout << "RealSense start error: " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
        return -40;
    }
    return 0;
}

//结束运行线程
int RealSenseVideoThread::stopThread()
{
    isStart = false;
    return 0;
}

//开始播放
void RealSenseVideoThread::startShowVideo()
{
    isReadVideo = true;
}

//停止播放
void RealSenseVideoThread::stopShowVideo()
{
    isReadVideo = false;
}

void RealSenseVideoThread::setIsRecord(bool isRecord)
{
    this->isRecordVideo = isRecord;
}

//是否打开视频
bool RealSenseVideoThread::getIsOpenVideo()
{
    return isOpen;
}

cv::Size RealSenseVideoThread::getVideoSize()
{
    cv::Size imageSize(0, 0);
    if(isOpen)
    {
        rs2::video_stream_profile rgbProfile = realSenseProfile.get_stream(RS2_STREAM_COLOR).as<rs2::video_stream_profile>();
        imageSize.width = rgbProfile.width();
        imageSize.height = rgbProfile.height();
    }
    return imageSize;
}

 int RealSenseVideoThread::getFrameNumber()
 {
    return this->frameNumber;
 }

void RealSenseVideoThread::setImageBuffer(cv::Mat &rgbFrame, cv::Mat &depthFrame)
{
    if(isRecordVideo)
    {
        ImageBuffer::freeBuffer.tryAcquire(1, 40);
        ImageBuffer::imageData[frameNumber] = rgbFrame.clone();
        ImageBuffer::depthData[frameNumber] = depthFrame.clone();
        ImageBuffer::usedBuffer.release();
        imageWriter.saveImage(rgbFrame, "result_rbg.png");
        imageWriter.saveImage(readDepthFrame, "result.depth.png");
    }
    else
    {
        ImageBuffer::imageData[frameNumber] = rgbFrame.clone();
        ImageBuffer::depthData[frameNumber] = depthFrame.clone();
    }
    frameNumber = (frameNumber + 1) % BUFFER_SIZE;
}

void RealSenseVideoThread::init()
{
    isStart = false;//开始运行线程
    isReadVideo = false;//是否读取视频
    isRecordVideo = false;
    isOpen = false;
    frameNumber = 0;

    frameToimage.load(":/images/images/play.png");

    realSenseConfig.enable_stream(RS2_STREAM_COLOR, 1280, 720, RS2_FORMAT_RGB8, 30);
    realSenseConfig.enable_stream(RS2_STREAM_DEPTH, 1280, 720, RS2_FORMAT_Z16, 30);
    colorMap.set_option(RS2_OPTION_COLOR_SCHEME, 2);
}
