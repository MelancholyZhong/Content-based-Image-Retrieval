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

void writeCSV(string filename, Mat m)
{
    ofstream myfile;
    myfile.open(filename.c_str());
    myfile << cv::format(m, cv::Formatter::FMT_CSV) << std::endl;
    myfile.close();
}

string getAbsolutePath(string relativePath)
{
    // get current directory path
    std::__fs::filesystem::path directoryPath = std::__fs::filesystem::current_path();
    string stringpath = directoryPath.generic_string();
    printf("%s\n", stringpath.c_str());
    // combine current directory path with relative path
    stringpath.append(relativePath);
    return stringpath;
}

int main()
{
    char filename[] = "test.csv";
    // char dirname[] = "training/";
    char matchAlgo = 'b';
    // appendFeatureVec(filename, dirname, matchAlgo);

    Mat target = imread("training/pic.0080.jpg");
    vector<float> rowFeature;
    toRowVec(target, rowFeature);
    vector<string> res = readFeatureVec(filename, rowFeature, matchAlgo);

    for(int i = 0; i < res.size(); i++){
        cout << res[i] << endl;
    }

    return 0;
}