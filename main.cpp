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
    string targetname;
    string database;
    string method;
    int N;
    if (argc < 5)
    {
        std::cout << "please input correct arguments" << std::endl;
        exit(-2);
    }
    else
    {
        targetname = convertToString(argv[1]);
        database = convertToString(argv[2]);
        string targetDir = database + "/" + targetname;
        target = cv::imread(targetDir);
        database = convertToString(argv[2]);
        method = convertToString(argv[3]);
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

    if (method == "baseline")
    {
        baselineMatch(target, featureVec);
    }
    else if (method == "color")
    {
        colorHistogram(target, featureVec);
    }
    else if (method == "magnitude")
    {
        colorHistogram(target, featureVec);
        vector<float> featureVec2;
        magnitudeHistogram(target, featureVec2);
        twoComposite(featureVec, featureVec2);
    }
    else if (method == "objectSpacial")
    {
        objectSpatial(target, featureVec);
    }
    else if (method == "spacialVariance")
    {
        colorHistogram(target, featureVec);
        vector<float> featureVec2;
        spacialVariance(target, featureVec2);
        twoComposite(featureVec, featureVec2);
    }
    else if (method=="laws")
    {
        lawsMatch(target, featureVec);
    }
    else if (method=="multiHistogram")
    {
        multiHistogramMatch(target, featureVec);
    }
    else if (method=="coMatrix")
    {
        comatrixMatch(target, featureVec);
    }
    else if (method=="gradient")
    {
        gradientMatch(target, featureVec);
    }
    else
    {
        std::cout << "Please input correct method"<<std::endl;
        exit(-1);
    }

    // Get feature vector of all images in directory and
    // return the names of N most close images
    vector<string> res = readFeatureVec(databaseFeature, targetname, featureVec, method, N);

    for(int i = 0; i < res.size(); i++){
        cout << res[i] << endl;
    }

    displayImages(res, database);

    return 0;
}