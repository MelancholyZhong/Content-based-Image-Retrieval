#ifndef HISTOGRAMS
#define HISTOGRAMS

int colorHistogram(cv::Mat &src, std::vector<float> &feature);

int magnitudeHistogram(cv::Mat &src, std::vector<float> &feature);

int objectSpatial(cv::Mat &src, std::vector<float> &feature);

int spacialVariance(cv::Mat &src, std::vector<float> &feature);

int twoComposite(std::vector<float> &feature1, std::vector<float> &feature2);

#endif