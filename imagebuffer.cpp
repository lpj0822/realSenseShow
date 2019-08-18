#include "imagebuffer.h"

QSemaphore ImageBuffer::freeBuffer(BUFFER_SIZE);
QSemaphore ImageBuffer::usedBuffer(0);
cv::Mat ImageBuffer::imageData[BUFFER_SIZE] = {};
cv::Mat ImageBuffer::depthData[BUFFER_SIZE] = {};

ImageBuffer::ImageBuffer()
{

}

ImageBuffer::~ImageBuffer()
{

}
