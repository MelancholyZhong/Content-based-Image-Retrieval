/*
    File-related funtion for Content-based Image Retrieval (CBIR)
    Created by Hui Hu for CS 5330 Computer Vision Spring 2023

    Functions to write feature vector to CSV files and read them from CSV files
*/

// basic
#include <algorithm> // std::reverse
#include <cstring>
#include <dirent.h>
#include <iostream>
#include <map>
#include <queue>    // for min heap
#include <stdio.h>  /* printf, fgets */
#include <stdlib.h> /* atoi */
#include <string>
#include <vector>

// custom function
#include "distances.h"
#include "histograms.h"
#include "matchingAlgo.h"

// openCV
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// convert string to character array and returns it
char *convertToCharArray(string s) {
  char *char_arr = &s[0];
  return char_arr;
}

// convert character array to string and returns it
string convertToString(char *a) {
  string s(a);
  return s;
}

/*
  AppendFeatureVec function: append the feature vector data to CSV file
    - filename: the name of CSV file
    - dirname: the name of directory
    - matchingAlgo: chosen matching algorithm

  Date sample:
  "imagename,float,float,float,...float\n"
 */
int appendFeatureVec(std::string filename, string dirname, string matchingAlgo) {
  // Variables for CSV file
  char buffer[256];
  char mode[8];
  FILE *fp;
  bool reset = false;
  // Variables for Directory
  DIR *dirp;
  struct dirent *dp;
  // Temp variabls
  vector<float> feature;
  Mat candidate;
  char tmp[256];

  // Open the directory
  dirp = opendir(convertToCharArray(dirname));
  if (dirp == NULL) {
    printf("Cannot open directory %s\n", convertToCharArray(dirname));
    exit(-1);
  }

  // Open CSV file
  if (reset) {
    // If file exists, erase all the contents; if file doesn't exist, create new file
    strcpy(mode, "w");
  } else {
    // If file exists, start writing from the end; if file doesn't exist, create new file
    strcpy(mode, "a");
  }

  fp = fopen(convertToCharArray(filename), mode);

  if (!fp) {
    printf("Unable to open output file %s\n", convertToCharArray(filename));
    exit(-1);
  }

  // Loop over all the files in the image file listing
  while ((dp = readdir(dirp)) != NULL) {
    // check if the file is an image
    if (strstr(dp->d_name, ".jpg") ||
        strstr(dp->d_name, ".png") ||
        strstr(dp->d_name, ".ppm") ||
        strstr(dp->d_name, ".tif")) {
      // Build the overall filename
      strcpy(buffer, convertToCharArray(dirname));
      strcat(buffer, "/");
      strcat(buffer, dp->d_name);

      // To do, get the image feature vector and save it
      // printf("full path name: %s\n", buffer);
      // Write the imagename and the feature vector to the CSV file
      fwrite(dp->d_name, sizeof(char), strlen(dp->d_name), fp);

      // Get candidate image
      candidate = imread(buffer);

      if (matchingAlgo == "baseline") {
        baselineMatch(candidate, feature);
      } else if (matchingAlgo == "color") {
        colorHistogram(candidate, feature);
      } else if (matchingAlgo == "magnitude") {
        colorHistogram(candidate, feature);
        vector<float> feature2;
        magnitudeHistogram(candidate, feature2);
        twoComposite(feature, feature2);
      } else if (matchingAlgo == "objectSpacial") {
        objectSpatial(candidate, feature);
      } else if (matchingAlgo == "spacialVariance") {
        colorHistogram(candidate, feature);
        vector<float> feature2;
        spacialVariance(candidate, feature2);
        twoComposite(feature, feature2);
      } else if (matchingAlgo == "laws") {
        lawsMatch(candidate, feature);
      } else if (matchingAlgo == "multiHistogram") {
        multiHistogramMatch(candidate, feature);
      } else if (matchingAlgo == "coMatrix") {
        comatrixMatch(candidate, feature);
      } else if (matchingAlgo == "gradient") {
        gradientMatch(candidate, feature);
      } else {
        std::cout << "Please input correct method" << std::endl;
        exit(-1);
      }

      // Write feature vector to CSV file

      for (int i = 0; i < feature.size(); i++) {
        // Write formatted data to string
        sprintf(tmp, ",%.4f", feature[i]);
        fwrite(tmp, sizeof(char), strlen(tmp), fp);
      }
      fwrite("\n", sizeof(char), 1, fp); // EOL
    }
  }

  // Close CSV file
  fclose(fp);

  return 0;
}

// reads a string from a CSV file
int getstring(FILE *fp, char os[]) {
  int p = 0;
  int eol = 0;

  for (;;) {
    char ch = fgetc(fp);
    if (ch == ',') {
      break;
    } else if (ch == '\n' || ch == EOF) {
      eol = 1;
      break;
    }
    os[p] = ch;
    p++;
  }
  os[p] = '\0';

  return (eol); // return true if it reaches the end of the line or the file (eol)
}

// read one float value from a CSV file
int getfloat(FILE *fp, float *v) {
  char s[256];
  int p = 0;
  int eol = 0;

  for (;;) {
    char ch = fgetc(fp);
    if (ch == ',') {
      break;
    } else if (ch == '\n' || ch == EOF) {
      eol = 1;
      break;
    }

    s[p] = ch;
    p++;
  }
  s[p] = '\0';  // terminator
  *v = atof(s); // convert string to float

  return (eol); // return true if eol
}

// Global variable
map<string, float> dict; // store (image_name, distance) pairs

// To compare two iamges according to their distances with target image
class Comparator {
public:
  int operator()(const string &name1, const string &name2) {
    return dict[name1] < dict[name2];
  }
};

/*
  readFeatureVec function: read the feature vector data from CSV file
    - filename: the name of CSV file
    - targetname: the name of target image
    - targetVec: the feature vactor of target image
    - matchingAlgo: chosen matching algorithm
    - N: the number of top matched images
 */
vector<string> readFeatureVec(std::string filename, std::string targetname, vector<float> &targetVec, string matchingAlgo, int N = 3) {
  vector<string> result = {};
  priority_queue<string, vector<string>, Comparator> minHeap;
  FILE *fp;

  char imagename[256];
  float distance, value;
  vector<float> featureVec;

  // Open CSV file
  fp = fopen(convertToCharArray(filename), "r");
  if (!fp) {
    printf("Unable to open feature file\n");
    return result;
  }

  // Read file
  for (;;) {
    featureVec = {};

    // Read image name, if reached the last line, exit loop
    if (getstring(fp, imagename))
      break;
    if (targetname.compare(imagename) == 0) {
      continue;
    }

    // Read feature vector
    for (;;) {
      float eol = getfloat(fp, &value);
      featureVec.push_back(value);
      if (eol)
        break;
    }

    // Get distance according to provided match algorithm
    if (matchingAlgo.compare("baseline") == 0) {
      distance = baselineDis(targetVec, featureVec);
    } else if (matchingAlgo.compare("color") == 0) {
      distance = colorDis(targetVec, featureVec);
    } else if (matchingAlgo.compare("magnitude") == 0) {
      distance = magnitude_color(targetVec, featureVec, 0.5, 0.5);
    } else if (matchingAlgo.compare("objectSpacial") == 0) {
      distance = 1 - intersection(targetVec, featureVec);
    } else if (matchingAlgo.compare("spacialVariance") == 0) {
      distance = similarObject(targetVec, featureVec);
    } else if (matchingAlgo.compare("laws") == 0) {
      distance = histogramDis(targetVec, featureVec);
    } else if (matchingAlgo.compare("multiHistogram") == 0) {
      distance = histogramDis(targetVec, featureVec);
    } else if (matchingAlgo.compare("coMatrix") == 0) {
      distance = comatrixDis(targetVec, featureVec);
    } else if (matchingAlgo.compare("gradient") == 0) {
      distance = histogramDis(targetVec, featureVec);
    } else {
      distance = baselineDis(targetVec, featureVec);
    }

    // Add new (image_name, distance) pair to dict
    dict[imagename] = distance;

    // Add new distance to min-Heap
    if (abs(distance) > 0.001) // Avoid target image itself
    {
      minHeap.push(imagename);
    }
    if (minHeap.size() > N) {
      minHeap.pop();
    }
  }

  // Close CSV file
  fclose(fp);

  // Return names of the N most close images
  while (!minHeap.empty()) {
    result.push_back(minHeap.top());
    minHeap.pop();
  }

  reverse(result.begin(), result.end());

  return result;
}

// Display all images
// Creared by Yao Zhong
void displayImages(vector<string> imageVec, string dirPath) {
  string imagePath;

  // Get images
  vector<Mat> images(imageVec.size());
  for (int i = 0; i < imageVec.size(); i++) {
    imagePath = dirPath + "/" + imageVec[i];
    images[i] = imread(imagePath);
  }

  cv::String windowName = "imgWindow";
  cv::namedWindow(windowName);
  int i = 0;
  cv::imshow(windowName, images[i]);
  char keyCode = cv::waitKey(0);
  while (keyCode != 'q') {
    if (keyCode == 'n' && i < images.size() - 1) {
      i++;
      cv::imshow(windowName, images[i]);
    } else if (keyCode == 'p' && i > 0) {
      i--;
      cv::imshow(windowName, images[i]);
    }
    keyCode = cv::waitKey(0);
  }
  cv::destroyWindow(windowName);
  return;
}