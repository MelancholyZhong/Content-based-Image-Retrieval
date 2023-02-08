/**
    Matching Algorithm for Content-based Image Retrieval (CBIR)
    Created by Hui Hu for CS 5330 Computer Vision Spring 2023
*/

#include <stdlib.h>
#include <string>
#include <vector>
// opencv

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

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

int toRowVec(Mat &feature, vector<float> &rowFeature)
{
    int r0 = feature.rows / 2 - 5;
    int c0 = feature.cols / 2 - 5;
    int d, c, r;
    const Mat_<Vec3b> featureVec = feature;
    rowFeature = {};

    for (d = 0; d < feature.channels(); d++)
    {
        for (r = 0; r < 9; r++)
        {
            for (c = 0; c < 9; c++)
            {
                rowFeature.push_back((float)featureVec(r + r0, c + c0)[d]);
            }
        }
    }

    return 0;
}

int histogramMatching()
{
    return 0;
}

int multiHistogramMatching()
{
    return 0;
}

std::vector<string> getFristN(int N)
{
    vector<string> test = {"123", "321"};
    return test;
}