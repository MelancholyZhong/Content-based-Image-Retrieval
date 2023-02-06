#ifndef TEXTURE
#define TEXTURE

int magnitudeHistogramCalc(cv::Mat &src, cv::Mat &feature);

float magnitudeHistogramDis(cv::Mat &target, cv::Mat &candidate);

#endif