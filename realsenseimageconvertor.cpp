#include "realsenseimageconvertor.h"
#include <cassert>

RealSenseImageConvertor::RealSenseImageConvertor()
{

}

RealSenseImageConvertor::~RealSenseImageConvertor()
{

}

// Convert rs2::frame to cv::Mat
cv::Mat RealSenseImageConvertor::frameToMat(const rs2::frame& image)
{
    auto vf = image.as<rs2::video_frame>();
    const int w = vf.get_width();
    const int h = vf.get_height();

    if (image.get_profile().format() == RS2_FORMAT_BGR8)
    {
        return cv::Mat(cv::Size(w, h), CV_8UC3, (void*)image.get_data(), cv::Mat::AUTO_STEP);
    }
    else if (image.get_profile().format() == RS2_FORMAT_RGB8)
    {
        auto r = cv::Mat(cv::Size(w, h), CV_8UC3, (void*)image.get_data(), cv::Mat::AUTO_STEP);
        cv::cvtColor(r, r, CV_RGB2BGR);
        return r;
    }
    else if (image.get_profile().format() == RS2_FORMAT_Z16)
    {
        return cv::Mat(cv::Size(w, h), CV_16UC1, (void*)image.get_data(), cv::Mat::AUTO_STEP);
    }
    else if (image.get_profile().format() == RS2_FORMAT_Y8)
    {
        return cv::Mat(cv::Size(w, h), CV_8UC1, (void*)image.get_data(), cv::Mat::AUTO_STEP);
    }

    throw std::runtime_error("Frame format is not supported yet!");
}

// Converts depth frame to a matrix of doubles with distances in meters
cv::Mat RealSenseImageConvertor::depthFrameToMeters(const rs2::pipeline& pipe, const rs2::depth_frame& f)
{
    cv::Mat dm = frameToMat(f);
    dm.convertTo(dm, CV_64F);
    auto depth_scale = pipe.get_active_profile()
        .get_device()
        .first<rs2::depth_sensor>()
        .get_depth_scale();
    dm = dm * depth_scale;
    return dm;
}

cv::Mat RealSenseImageConvertor::RGBImageStitchDepth(const cv::Mat &rgbImage, const cv::Mat &depthImage)
{
    assert(rgbImage.rows == depthImage.rows);
    const int width = rgbImage.cols + depthImage.cols;
    const int height = rgbImage.rows;
    cv::Rect roi1(0, 0, rgbImage.cols, height);
    cv::Rect roi2(rgbImage.cols, 0, depthImage.cols, height);
    cv::Mat result = cv::Mat::zeros(cv::Size(width, height), CV_8UC3);
    rgbImage.copyTo(result(roi1));
    depthImage.copyTo(result(roi2));
    return result;
}

cv::Mat RealSenseImageConvertor::scaleDepthMat(cv::Mat &depthFrame)
{
    const int  width = depthFrame.cols;
    const int height = depthFrame.rows;
    cv::Mat result = cv::Mat::zeros(cv::Size(width, height), CV_8UC1);
    for(int row = 0; row < height; row++)
    {
        double *src = depthFrame.ptr<double>(row);
        uchar *dst = result.ptr<uchar>(row);
        for(int col = 0; col < width; col++)
        {
            int temp = (int)(src[col] * 50);
            if(temp > 255)
            {
                temp = 255;
            }
            dst[col] = (uchar)temp;
        }
    }
    return result;
}
