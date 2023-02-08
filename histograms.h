#ifndef HISTOGRAMS
#define HISTOGRAMS

int colorHistogram(cv::Mat &src, std::vector<float> &feature);

int magnitudeHistogram(cv::Mat &src, std::vector<float> &feature);

int objectSpatial(cv::Mat &src, std::vector<float> &feature);

#endif