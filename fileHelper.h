/*
    Helper functions to deal with files
    Created by Hui Hu for CS 5330 Computer Vision Spring 2023
*/

#ifndef FILE_HELPER
#define FILE_HELPER

#include <vector>
#include <string>

using namespace std;

int appendFeatureVec(std::string filename, char *dirname, char* matchingAlgo);

vector<string> readFeatureVec(std::string filename, vector<float> &targetVec, char *matchingAlgo, int N = 3);

#endif
