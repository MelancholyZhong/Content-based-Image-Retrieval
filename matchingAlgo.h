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

// Use the 9x9 square in the middle of the image as feature vector
int baselineMatch(Mat &src, vector<float> &feature);

// Use sum-of-squared-difference as the distance metric
float baselineDis(vector<float> target, vector<float> candidate);

// Use whole-image histogram and center-block histogram as feature vector
int multiHistogramMatch(Mat &src, vector<float> &feature);

// Use biased histogram intersection as the distance metric
float multiHistogramDis(vector<float> target, vector<float> candidate);

// Use histogram of gradient orientations as feature vector
int gradientMatch(Mat &src, vector<float> &feature);

// Use histogram of Laws filter responses as feature vector
int lawsMatch(Mat &src, vector<float> &feature);

// Use histogram intersection as the distance metric
float histogramDis(vector<float> target, vector<float> candidate);

// Use features of co-occurrence matrices as feature vector
int comatrixMatch(Mat &src, vector<float> &feature);

// Use weighted sum as the distance metric
float comatrixDis(vector<float> target, vector<float> candidate);

// Display all images horizontally
void displayImages(vector<string> imageVec, char* filename);

#endif