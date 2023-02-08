#ifndef COLOR_HISTOGRAM
#define COLOR_HISTOGRAM

int colorHistogramCalc(cv::Mat &src, cv::Mat &feature);

float colorHistogramDis(verctor<float> &target, verctor<float> &candidate);

#endif