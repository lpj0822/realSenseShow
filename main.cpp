#pragma execution_character_set("utf-8")
#include "controlwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    app.setApplicationVersion("1.0");
    app.setApplicationName("RGBD图像显示");

    //load qss file
    QFile file(":/style/style/style.qss");
    file.open(QFile::ReadOnly);
    QString qss = QLatin1String(file.readAll());
    qApp->setStyleSheet(qss);

    ControlWindow w;
    w.show();

    return app.exec();
}
