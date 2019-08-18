#ifndef SAVEVIDEOTHREAD_H
#define SAVEVIDEOTHREAD_H

#include <QThread>
#include <QString>
#include "utility/myvideowriter.h"
#include "utility/errorcodestring.h"

class SaveVideoThread : public QThread
{
    Q_OBJECT
public:
    SaveVideoThread();
    ~SaveVideoThread();

    int initSaveVideoData(const QString &fileNameDir, const cv::Size imageSize, const int frameNumber);//初始化保存数据参数

    void startThread();//开始线程
    void stopThread();//结束线程

    void closeVideoWrite();//关闭视频写入

signals:
    void signalSaveVideoMessage(QString info);

protected:
    void run();

private:
    ErrorCodeString errorCodeString;
    MyVideoWriter *outputVideo;
    MyVideoWriter *depthOutputVideo;
    QString fileName;//保存的文件名

    bool isStart;//是否允许线程

    cv::Size size;//保存视频文件的大小
    double fps;//帧率
    int codec;//视频编码格式
    bool isColor;//是否是彩色

    int frameNumber;

private:
    void init();
};

#endif // SAVEVIDEOTHREAD_H
