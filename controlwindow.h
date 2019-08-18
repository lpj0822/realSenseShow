#ifndef CONTROLWINDOW_H
#define CONTROLWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QTimer>
#include <QTextBrowser>
#include <QVector>
#include <QPoint>
#include "utility/errorcodestring.h"
#include "realsensevideoshowwindow.h"

class ControlWindow : public QWidget
{
    Q_OBJECT

public:
    ControlWindow(QWidget *parent = 0);
    ~ControlWindow();

public slots:
    void slotOpenVideo();
    void slotRecordVideo();
    void slotClearText();

    void slotVideoMessage(QString message);

private:

    void updateRecordButton();

private:

    QPushButton *openVideoButton;
    QPushButton *recordVideoButton;
    QPushButton *clearTextButton;

    QTextBrowser *commandText;//输出黑匣子指令
    QVBoxLayout *mainLayout;//主布局

    ErrorCodeString errorCodeString;
    RealSenseVideoShowWindow *showWindow;

    bool isOpen;//是否打开视频
    bool isRecord;

    void initUI();
    void initData();
    void initConnect();
};

#endif // CONTROLWINDOW_H
