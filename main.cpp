#include <stdio.h>
#include <iostream>
#include <fstream>
#include "fileHelper.h"
#include "matchingAlgo.h"

#include <opencv2/imgcodecs.hpp>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;


int main()
{
    char filename[] = "test.csv";
    // char dirname[] = "training/";
    char matchAlgo = 'b';
    // appendFeatureVec(filename, dirname, matchAlgo);

    Mat target = imread("training/pic.0080.jpg");
    
    vector<float> rowFeature;
    lawsMatch(target, rowFeature);
    baselineMatch(target, rowFeature);
    vector<string> res = readFeatureVec(filename, rowFeature, matchAlgo);

    for (int i = 0; i < res.size(); i++)
    {
        cout << res[i] << endl;
    }

    return 0;
}