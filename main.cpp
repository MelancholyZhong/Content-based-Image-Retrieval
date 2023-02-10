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
    cv::Mat target;
    if (argc < 6)
    {
        std::cout << "please input correct arguments" << std::endl;
        exit(-2);
    }
    else
    {
        char *targetDir = argv[1];
        target = cv::imread(targetDir);
        char *database = argv[2];
        char *method = argv[3];
        char *outputN = argv[4];
        int N = std::stoi(outputN);
    }

    if(target.empty()){
        std::cout << "Can't read the image" << std::endl;
        exit(-1);
    }

    // char filename[] = getFileName(dirname, matchAlgo);

    // Get feature vector of target image
    vector<float> featureVec;
    std::string databaseFeature = database + "_" + method + ".csv";
    ifstream csvExists;
    csvExists.open(databaseFeature);
    if(!csvExists){
        appendFeatureVec(databaseFeature, database, method);
    }

    switch (method)
    {
    case "baseline":
        baselineMatch(target, featureVec);
        break;
    case "color":
        colorHistogram(target, featureVec);
        break;
    case "magnitude":
        magnitudeHistogram(target, featureVec);
        break;
    case "objectSpacial":
        objectSpatial(target, featureVec);
        break;
    case "spacialVariance":
        spacialVariance(target, featureVec);
        break;
    default:
        baselineMatch(target, featureVec);
        break;
    }

    // Get feature vector of all images in directory and
    // return the names of N most close images
    vector<string> res = readFeatureVec(databaseFeature, featureVec, method, N);

    return 0;
}