#pragma execution_character_set("utf-8")
#include "controlwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

ControlWindow::ControlWindow(QWidget *parent) : QWidget(parent)
{
    initUI();
    initConnect();
    initData();
}

ControlWindow::~ControlWindow()
{
    isOpen = false;
    showWindow->deleteLater();
}

void ControlWindow::slotOpenVideo()
{
    if(!isOpen)
    {
        int errorCode = showWindow->showVideo();
        if(errorCode == 0)
        {
            isOpen = true;
            openVideoButton->setText(tr("关闭视频"));
        }
        else
        {
            commandText->append(errorCodeString.getErrCodeString(errorCode));
            QMessageBox::information(this, tr("打开视频"), tr("打开RealSense失败，请检查RealSense摄像头"));
        }
    }
    else
    {
        if(isRecord)
        {
            slotRecordVideo();
        }
        showWindow->closeShowVideo();
        isOpen = false;
        openVideoButton->setText(tr("打开视频"));
    }
    updateRecordButton();
}

void ControlWindow::slotRecordVideo()
{
    if(recordVideoButton->text().startsWith(tr("开始录制")))
    {
        showWindow->setIsRecordVideo(true);
        recordVideoButton->setText(tr("停止录制"));
        isRecord = true;
    }
    else if(recordVideoButton->text().startsWith(tr("停止录制")))
    {
        showWindow->setIsRecordVideo(false);
        recordVideoButton->setText(tr("开始录制"));
        isRecord = false;
    }
}

void ControlWindow::slotClearText()
{
    qDebug()<< commandText->toPlainText() << endl;
    commandText->clear();
}

void ControlWindow::slotVideoMessage(QString message)
{
    commandText->append(message);
}

void ControlWindow::updateRecordButton()
{
    if(isOpen)
    {
        recordVideoButton->setEnabled(true);
    }
    else
    {
        recordVideoButton->setEnabled(false);
        recordVideoButton->setText(tr("开始录制"));
    }
}


void ControlWindow::initUI()
{
    openVideoButton = new QPushButton(tr("打开RealSense"));//打开摄像头按钮
    recordVideoButton = new QPushButton(tr("开始录制"));
    recordVideoButton->setEnabled(false);
    clearTextButton = new QPushButton(tr("清空文本信息"));

    mainLayout = new QVBoxLayout();//主布局

    QHBoxLayout *topLayout=new QHBoxLayout();
    topLayout->setAlignment(Qt::AlignCenter);
    topLayout->setSpacing(50);
    topLayout->addWidget(openVideoButton);
    topLayout->addWidget(recordVideoButton);
    topLayout->addWidget(clearTextButton);

    showWindow = new RealSenseVideoShowWindow(this);

    commandText=new QTextBrowser;
    commandText->setFixedHeight(60);
    commandText->setReadOnly(true);

    mainLayout->setMargin(10); //设置这个对话框的边距
    mainLayout->setSpacing(10);  //设置各个控件之间的边距
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(showWindow);
    mainLayout->addWidget(commandText);
    this->setLayout(mainLayout);
    //this->setMaximumSize(700,520);
    this->setMinimumSize(1200, 700);
    this->setWindowTitle(tr("RGB-D图像采集"));
}

void ControlWindow::initConnect()
{
    connect(openVideoButton, &QPushButton::clicked, this, &ControlWindow::slotOpenVideo);
    connect(recordVideoButton, &QPushButton::clicked, this, &ControlWindow::slotRecordVideo);
    connect(clearTextButton, &QPushButton::clicked, this, &ControlWindow::slotClearText);
    connect(showWindow, &RealSenseVideoShowWindow::signalVideoMessage, this, &ControlWindow::slotVideoMessage);
}

void ControlWindow::initData()
{
    isOpen = false;
    isRecord = false;
}
