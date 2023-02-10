#include <stdio.h>
#include <iostream>
#include <fstream>
#include "fileHelper.h"
#include "matchingAlgo.h"
#include "histograms.h"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    cv::Mat target = imread("training/pic.0080.jpg");
    string database = "training/";
    string method = "baseline";
    int N = 3;
    if (argc < 6)
    {
        std::cout << "please input correct arguments" << std::endl;
        // exit(-2);
    }
    else
    {
        char *targetDir = argv[1];
        target = cv::imread(targetDir);
        database = convertToString(argv[2]);
        method = convertToString(argv[3]);
        // char *method = argv[3];
        char *outputN = argv[4];
        N = std::stoi(outputN);
    }

    if (target.empty())
    {
        std::cout << "Can't read the image" << std::endl;
        exit(-1);
    }

    // char filename[] = getFileName(dirname, matchAlgo);

    // Get feature vector of target image
    vector<float> featureVec;
    std::string databaseFeature = database + "_" + method + ".csv";
    ifstream csvExists;
    csvExists.open(databaseFeature);

    if (!csvExists)
    {
        appendFeatureVec(databaseFeature, database, method);
    }

    if (method.compare("baseline") == 0)
    {
        baselineMatch(target, featureVec);
    }
    else if (method.compare("color") == 0)
    {
        colorHistogram(target, featureVec);
    }
    else if (method.compare("magnitude") == 0)
    {
        magnitudeHistogram(target, featureVec);
    }
    else if (method.compare("objectSpacial") == 0)
    {
        objectSpatial(target, featureVec);
    }
    else if (method.compare("spacialVariance") == 0)
    {
        spacialVariance(target, featureVec);
    }
    else if (method.compare("laws") == 0)
    {
        lawsMatch(target, featureVec);
    }
    else if (method.compare("multiHistogram") == 0)
    {
        multiHistogramMatch(target, featureVec);
    }
    else if (method.compare("coMatrix") == 0)
    {
        comatrixMatch(target, featureVec);
    }
    else if (method.compare("gradient") == 0)
    {
        gradientMatch(target, featureVec);
    }
    else
    {
        baselineMatch(target, featureVec);
    }

    // Get feature vector of all images in directory and
    // return the names of N most close images
    vector<string> res = readFeatureVec(databaseFeature, featureVec, method, N);

    for(int i = 0; i < res.size(); i++){
        cout << res[i] << endl;
    }
    displayImages(res, database);

    return 0;
}