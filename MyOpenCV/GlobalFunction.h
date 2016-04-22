#pragma once
#include <stdafx.h>
#include <opencv.hpp>
#include <Stopwatch.h>

using namespace cv;


QImage  cvMat2QImage(const cv::Mat &);

Mat QImage2cvMat(QImage &);

QFileInfoList GetFileList(QString);

Mat DetectFace(Mat &, CvHaarClassifierCascade*, QTextEdit*);