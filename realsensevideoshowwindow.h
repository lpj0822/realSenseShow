#ifndef REALSENSEVIDEOSHOWWINDOW_H
#define REALSENSEVIDEOSHOWWINDOW_H

#include <QWidget>
#include <QTimer>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QScrollArea>
#include <QFileInfo>
#include "realsensevideothread.h"
#include "savevideothread.h"

class RealSenseVideoShowWindow : public QWidget
{
    Q_OBJECT
public:
    RealSenseVideoShowWindow(QWidget *parent = 0);
    ~RealSenseVideoShowWindow();

    int showVideo();//显示视频
    void closeShowVideo();//关闭视频

    void setIsRecordVideo(bool isRecord);

    bool getIsOpenVideo();//是否打开视频

signals:
    void signalVideoMessage(QString info);

public slots:
    void slotVideoImage(QImage image,bool isOpen);

private:
    void updateImage();
    void recordVideo(bool isRecord);

private:

    QImage currentImage;//获取的帧转化为QImage

    RealSenseVideoThread *videoThread;
    SaveVideoThread *saveVideoThread;

    QLabel *imageShowLabel;
    QLabel *imageShowText;
    QScrollArea *imageShowScrollArea;

private:
    void initData();
    void initUI();
    void initConnect();
};

#endif // REALSENSEVIDEOSHOWWINDOW_H
