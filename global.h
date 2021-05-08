#ifndef GLOBAL_H
#define GLOBAL_H

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <QImage>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/videoio.hpp>

#if $CV_VERSION_MAJOR > 2
#include <opencv2/imgproc/types_c.h>
#include <opencv2/videoio/videoio_c.h>
#endif

#define time_diff(ts, te)     ((te - ts)/getTickFrequency())

void msleep(unsigned int msec);
QImage mat2qimage(const cv::Mat &mat);
cv::Mat qimage2mat(const QImage &qimage);

#endif // GLOBAL_H
