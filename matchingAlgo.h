/**
    Matching Algorithm for Content-based Image Retrieval (CBIR)
    Created by Hui Hu for CS 5330 Computer Vision Spring 2023
*/

#ifndef MATCHING_ALGO
#define MATCHING_ALGO

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

using namespace std;
using namespace cv;

// Use the 9x9 square in the middle of the image as a feature vector

int baselineMatch(Mat &src, vector<float> &feature);

// Use sum-of-squared-difference as the distance metric
float baselineDis(vector<float> target, vector<float> candidate);

int toRowVec(Mat &feature, vector<float> &rowFeature);

int histogramMatching();

int multiHistogramMatching();


#endif