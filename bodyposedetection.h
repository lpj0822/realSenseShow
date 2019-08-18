#ifndef BODYPOSEDETECTION_H
#define BODYPOSEDETECTION_H

#include <QObject>

class BodyPoseDetection : public QObject
{
    Q_OBJECT
public:
    BodyPoseDetection(QObject *parent = nullptr);
    ~BodyPoseDetection();

signals:

public slots:

private:

};

#endif // BODYPOSEDETECTION_H
