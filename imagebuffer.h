#ifndef IMAGEBUFFER_H
#define IMAGEBUFFER_H

#include <QSemaphore>
#include <opencv2/core.hpp>

#define BUFFER_SIZE 10

class ImageBuffer
{
public:
    ImageBuffer();
    ~ImageBuffer();

    static QSemaphore freeBuffer;
    static QSemaphore usedBuffer;

    static cv::Mat depthData[BUFFER_SIZE];
    static cv::Mat imageData[BUFFER_SIZE];
};

#endif // IMAGEBUFFER_H
