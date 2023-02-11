/**
    Main function for Content-based Image Retrieval (CBIR)
    Created by Hui Hu and Yao Zhong for CS 5330 Computer Vision Spring 2023

    The main function to get top matched images after giving some target image, image directory and matchin method
*/

// basic
#include <fstream>
#include <iostream>
#include <stdio.h>
// opencv
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
// custom functios
#include "fileHelper.h"
#include "histograms.h"
#include "matchingAlgo.h"

using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {

  // Variables to save the basic info about one task
  cv::Mat target;    // target image
  string targetname; // the name of target image in database
  string database;   // the directory of images
  string method;     // the method of mataching algorithm
  int N;             // the size of matched images

  // Get values from arguments
  if (argc < 5) {
    std::cout << "please input correct arguments" << std::endl;
    exit(-2);
  } else {
    targetname = convertToString(argv[1]);
    database = convertToString(argv[2]);
    string targetDir = database + "/" + targetname;
    target = cv::imread(targetDir);
    database = convertToString(argv[2]);
    method = convertToString(argv[3]);
    char *outputN = argv[4];
    N = std::stoi(outputN);
  }

  if (target.empty()) {
    std::cout << "Can't read the image" << std::endl;
    exit(-1);
  }

  // char filename[] = getFileName(dirname, matchAlgo);

  // Get feature vector of target image
  vector<float> featureVec;
  std::string databaseFeature = database + "_" + method + ".csv";
  ifstream csvExists;
  csvExists.open(databaseFeature);

  // Check whether we have computed feature vector before
  if (!csvExists) {
    // If not, extract feature vector of images in directory and store it on CSV files
    appendFeatureVec(databaseFeature, database, method);
  }

  // Get the feature vector of target image according to provided method
  if (method == "baseline") {
    baselineMatch(target, featureVec);
  } else if (method == "color") {
    colorHistogram(target, featureVec);
  } else if (method == "magnitude") {
    colorHistogram(target, featureVec);
    vector<float> featureVec2;
    magnitudeHistogram(target, featureVec2);
    twoComposite(featureVec, featureVec2);
  } else if (method == "objectSpacial") {
    objectSpatial(target, featureVec);
  } else if (method == "spacialVariance") {
    colorHistogram(target, featureVec);
    vector<float> featureVec2;
    spacialVariance(target, featureVec2);
    twoComposite(featureVec, featureVec2);
  } else if (method == "laws") {
    lawsMatch(target, featureVec);
  } else if (method == "multiHistogram") {
    multiHistogramMatch(target, featureVec);
  } else if (method == "coMatrix") {
    comatrixMatch(target, featureVec);
  } else if (method == "gradient") {
    gradientMatch(target, featureVec);
  } else {
    std::cout << "Please input correct method" << std::endl;
    exit(-1);
  }

  // Get feature vector of all images in directory and
  // return the names of N most close images
  vector<string> res = readFeatureVec(databaseFeature, targetname, featureVec, method, N);

  // print the top matched image
  for (int i = 0; i < res.size(); i++) {
    cout << res[i] << endl;
  }

  // Display matched images using opencv GUI
  displayImages(res, database);

  return 0;
}