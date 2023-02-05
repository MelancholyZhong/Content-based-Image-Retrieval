#ifndef COLOR_HISTOGRAM
#define COLOR_HISTOGRAM

int colorHistogramCalc(cv::Mat &src, cv::Mat &feature);

float colorHistogramDis(cv::Mat &target, cv::Mat &candidate);

#endif