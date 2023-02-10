/**
    Matching Algorithm for Content-based Image Retrieval (CBIR)
    Created by Hui Hu for CS 5330 Computer Vision Spring 2023
*/

// basic
#include <stdlib.h>
#include <string>
#include <vector>
#include <cmath>
// opencv
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "histograms.h"

using namespace std;
using namespace cv;

// Use the 9x9 square in the middle of the image as feature vector
int baselineMatch(Mat &src, vector<float> &feature)
{
    const Mat_<Vec3b> srcVec = src;
    feature = {}; // initialization
    int r0 = src.rows / 2 - 5;
    int c0 = src.cols / 2 - 5;
    int d, c, r;

    for (d = 0; d < src.channels(); d++)
    {
        for (r = 0; r < 9; r++)
        {
            for (c = 0; c < 9; c++)
            {
                feature.push_back((float)srcVec(r + r0, c + c0)[d]);
            }
        }
    }
    return 0;
}

// Use sum-of-squared-difference as the distance metric
float baselineDis(vector<float> target, vector<float> candidate)
{
    if (target.size() != candidate.size())
    {
        cout << target.size() << "-" << candidate.size() << endl;
        return 0;
    }
    float distance = 0.0;
    for (int i = 0; i < target.size(); i++)
    {
        distance += (target[i] - candidate[i]) * (target[i] - candidate[i]);
    }
    return distance;
}

// Use whole-image histogram and center-block histogram as feature vector
int multiHistogramMatch(Mat &src, vector<float> &feature)
{
    vector<float> wholeFeature;
    vector<float> centerFeature;

    colorHistogram(src, wholeFeature);
    colorHistogram(src, centerFeature);

    // initialize feature vector
    feature = {};
    // combine two feature vectors
    feature.insert(feature.end(), wholeFeature.begin(), wholeFeature.end());
    feature.insert(feature.end(), centerFeature.begin(), centerFeature.end());

    return 0;
}

// Use biased histogram intersection as the distance metric
float multiHistogramDis(vector<float> target, vector<float> candidate)
{
    float distance = 0;
    float wholeDis = 0;  // the distance between whole-image histograms
    float centerDis = 0; // the distance between center-block histograms
    float weight = 0.4;  // the weight of wholeDis, between 0 and 1

    int size = candidate.size() / 2;

    for (int i = 0; i < size; i++)
    {
        wholeDis += std::min(target[i], candidate[i]);
    }
    for (int i = size; i < 2 * size; i++)
    {
        centerDis += std::min(target[i], candidate[i]);
    }

    // Get the biased distance
    distance = weight * wholeDis + (1 - weight) * centerDis;

    return distance;
}

// Use histogram of gradient orientations as feature vector
int gradientMatch(Mat &src, vector<float> &feature)
{
    // Convert the image to grayscale
    Mat src_gray;
    cvtColor(src, src_gray, COLOR_BGR2GRAY);

    // Apply the Sobel Operator
    Mat Sx, Sy;
    Sobel(src_gray, Sx, CV_32F, 1, 0);
    Sobel(src_gray, Sy, CV_32F, 0, 1);

    // Calculates the rotation angle of 2D vectors
    Mat gradient;
    phase(Sx, Sy, gradient, true);

    // Get feature vector
    feature = vector<float>(360, 0.0);
    Mat_<float> gradientVec = gradient;
    int bin;
    for (int r = 0; r < gradient.rows; r++)
    {
        for (int c = 0; c < gradient.cols; c++)
        {
            bin = gradientVec(r, c) / 1;
            feature[bin] += 1;
        }
    }

    // // For display
    // Mat res;
    // normalize(gradient, res, 0.0, 255.0, cv::NORM_MINMAX, CV_8U);
    // imshow("gradient orientation", gradient);

    return 0;
}

// Use histogram of Laws filter responses as feature vector
int lawsMatch(Mat &src, vector<float> &feature)
{
    // Define the laws filter kernels (Level, Edge, Wave, Spot, Ripple)
    vector<vector<vector<float>>> matrix(5);
    matrix[0] = {{1, 4, 6, 4, 1},
                 {4, 16, 24, 16, 4},
                 {6, 24, 36, 24, 6},
                 {4, 16, 24, 16, 4},
                 {1, 4, 6, 4, 1}};

    matrix[1] = {{-1, -4, -5, -4, -1},
                 {-4, -16, -20, -16, -4},
                 {0, 0, 0, 0, 0},
                 {4, 16, 20, 16, 4},
                 {1, 4, 5, 4, 1}};

    matrix[2] = {{-1, -4, 0, 4, 1},
                 {-4, -16, 0, 16, 4},
                 {-5, -20, 0, 20, 5},
                 {-4, -16, 0, 16, 4},
                 {-1, -4, 0, 4, 1}};

    matrix[3] = {{-1, -4, -6, -4, -1},
                 {0, 0, 0, 0, 0},
                 {1, 4, 6, 4, 1},
                 {0, 0, 0, 0, 0},
                 {-1, -4, -6, -4, -1}};

    matrix[4] = {{1, 4, 0, -4, -1},
                 {4, 16, 0, -16, -4},
                 {5, 20, 0, -20, -5},
                 {4, 16, 0, -16, -4},
                 {1, 4, 0, -4, -1}};

    vector<Mat> kernel(5);

    for (int i = 0; i < 5; i++)
    {
        kernel[i] = Mat::zeros(5, 5, CV_32FC1);
        Mat_<float> kernelVec = kernel[i];
        for (int r = 0; r < 5; r++)
        {
            for (int c = 0; c < 5; c++)
            {
                kernelVec(r, c) = matrix[i][r][c] / 256;
            }
        }
    }

    //  Apply the laws filter kernels to the input image
    vector<Mat> features(5);
    for (int i = 0; i < 5; i++)
    {
        filter2D(src, features[i], -1, kernel[i]);
        cout << features[i].type() << endl; // CV_8U C3
    }
    return 0;
}

// Use histogram intersection as the distance metric
float histogramDis(vector<float> target, vector<float> candidate)
{
    if (candidate.size() != target.size())
    {
        return 0.0;
    }

    float distance = 0;
    for (int i = 0; i < target.size(); i++)
    {
        distance += min(target[i], candidate[i]);
    }

    return distance;
}

// Use features of co-occurrence matrices as feature vector
int comatrixMatch(Mat &src, vector<float> &feature)
{
    // Convert the image to grayscale
    Mat src_gray;
    cvtColor(src, src_gray, COLOR_BGR2GRAY);

    // Get co-occurrence matrix
    Mat hist;
    int histSize = 256;
    float range[] = {0, 256}; // the upper boundary is exclusive
    const float *histRange[] = {range};
    bool uniform = true, accumulate = false;
    calcHist(&src_gray, 1, 0, Mat(), hist, 1, &histSize, histRange, uniform, accumulate);

    // Get the feature vector
    // {energy, entropy, contrast, max_probability}
    feature = vector<float>(5, 0.0);
    Mat_<float> histVec = hist;
    float sumOfValue = 0.0;
    float mean = (histVec.rows - 1) * histVec.rows / 2;
    vector<float> prob_matrix = vector<float>(hist.rows, 0.0);

    for (int i = 0; i < hist.rows; i++)
    {
        feature[0] += histVec(i, 0) * histVec(i, 0); // energy
        sumOfValue += histVec(i, 0);
    }

    for (int i = 0; i < hist.rows; i++)
    {
        prob_matrix[i] = histVec(i, 0) / sumOfValue;
        feature[1] += (0 - prob_matrix[i] * (log(prob_matrix[i]) / log(2))); // entropy
        feature[2] += (i - mean) * (i - mean) * prob_matrix[i];
        if (prob_matrix[i] > feature[4])
        {
            feature[3] = prob_matrix[i]; // max_probability
        }
    }

    return 0;
}

// Use weighted sum as the distance metric
float comatrixDis(vector<float> target, vector<float> candidate)
{
    vector<float> weight = {0.25, 0.25, 0.25, 0.25};
    float distance = 0.0;
    for (int i = 0; i < target.size(); i++)
    {
        distance += abs(target[0] - candidate[0]) / target[0] * 100.0 * weight[i];
    }

    return distance;
}