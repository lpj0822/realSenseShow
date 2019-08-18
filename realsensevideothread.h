#ifndef REALSENSEVIDEOTHREAD_H
#define REALSENSEVIDEOTHREAD_H

#include <QThread>
#include <QImage>
#include "utility/errorcodestring.h"
#include "utility/myimagewriter.h"
#include "realsenseimageconvertor.h"

class RealSenseVideoThread : public QThread
{
    Q_OBJECT
public:
    RealSenseVideoThread();
    ~RealSenseVideoThread();

    int startThread();
    int stopThread();

    void startShowVideo();
    void stopShowVideo();

    void setIsRecord(bool isRecord);

    bool getIsOpenVideo();
    cv::Size getVideoSize();
     int getFrameNumber();

signals:
    void signalVideoData(QImage image, bool isOpenVideo);
    void signalVideoMessage(QString info);

protected:
    void run();

private:
    void setImageBuffer(cv::Mat &rgbFrame, cv::Mat &depthFrame);

private:

    int frameNumber;
    QImage frameToimage;
    cv::Mat readRGBFrame;
    cv::Mat readDepthFrame;
    bool isOpen;

    bool isStart;
    bool isReadVideo;
    bool isRecordVideo;

    rs2::config realSenseConfig;
    // Declare depth colorizer for pretty visualization of depth data
    rs2::colorizer colorMap;
    // Declare RealSense pipeline, encapsulating the actual device and sensors
    rs2::pipeline pipe;
    rs2::pipeline_profile realSenseProfile;

    ErrorCodeString errorCodeString;
    RealSenseImageConvertor imageConvertor;

    MyImageWriter imageWriter;

private:
    void init();

};

#endif // REALSENSEVIDEOTHREAD_H
