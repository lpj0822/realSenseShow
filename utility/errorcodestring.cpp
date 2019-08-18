#include "errorcodestring.h"
#include <iostream>

ErrorCodeString::ErrorCodeString(QObject *parent) : QObject(parent)
{
    init();
    std::cout<<"ErrorCodeString()"<<std::endl;
}

ErrorCodeString::~ErrorCodeString()
{
    std::cout<<"~ErrorCodeString()"<<std::endl;
}

void ErrorCodeString::setErrorCode(int code)
{
    this->errorCode=code;
}

int ErrorCodeString::getErrorCode()
{
    return errorCode;
}

//得到错误码对应的字符串
QString ErrorCodeString::getErrCodeString(int errCode)
{
    QString message="";
    switch(errCode)
    {
    case 0:
        message=QString("ErrorCode(0):not error!\n");
        break;
    case -1:
        message=QString("ErrorCode(-1):video not open!\n");
        break;
    case -2:
        message=QString("ErrorCode(-2):read next frame fail!\n");
        break;
    case -10:
        message=QString("ErrorCode(-10):image converted to PNG fail!\n");
        break;
    case -11:
        message=QString("ErrorCode(-11):make dir fail!\n");
        break;
    case -20:
        message=QString("ErrorCode(-20):init VideoWriter fail!\n");
        break;
    case -21:
        message=QString("ErrorCode(-21):write video data fail!\n");
        break;
    case -30:
        message=QString("ErrorCode(-30):loade cascadeClassifier fail!\n");
        break;
    case -40:
        message=QString("ErrorCode(-40):RealSense capture open fail!\n");
        break;
    case -41:
        message=QString("ErrorCode(-41):RealSense capture close fail!\n");
        break;
    case -42:
        message=QString("ErrorCode(-42):RealSense read data fail!\n");
        break;
     default:
        message=QString("ErrorCode(%1):nuknow error\n").arg(errCode);
    }
    this->errorCode=errCode;
    return message;
}

void ErrorCodeString::init()
{
    errorCode=0;
}
