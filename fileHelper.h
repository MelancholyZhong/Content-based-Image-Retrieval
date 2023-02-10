/*
    Helper functions to deal with files
    Created by Hui Hu for CS 5330 Computer Vision Spring 2023
*/

#ifndef FILE_HELPER
#define FILE_HELPER

#include <vector>
#include <string>

using namespace std;

int appendFeatureVec(std::string filename, std::string dirname, std::string matchingAlgo);

std::vector<std::string> readFeatureVec(std::string filename, std::vector<float> &targetVec, std::string matchingAlgo, int N = 3);

std::string convertToString(char *a);
char* convertToCharArray(std::string s);
void displayImages(std::vector<string> imageVec, std::string filename);

#endif
