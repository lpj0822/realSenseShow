#ifndef REALSENSEIMAGECONVERTOR_H
#define REALSENSEIMAGECONVERTOR_H

#include <librealsense2/rs.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

class RealSenseImageConvertor
{
public:
    RealSenseImageConvertor();
    ~RealSenseImageConvertor();

    // Convert rs2::frame to cv::Mat
    cv::Mat frameToMat(const rs2::frame& image);
    //Converts depth frame to a matrix of doubles with distances in meters
    cv::Mat depthFrameToMeters(const rs2::pipeline& pipe, const rs2::depth_frame& f);

    cv::Mat RGBImageStitchDepth(const cv::Mat &rgbImage, const cv::Mat &depthImage);

    cv::Mat scaleDepthMat(cv::Mat &depthFrame);
};

#endif // REALSENSEIMAGECONVERTOR_H
