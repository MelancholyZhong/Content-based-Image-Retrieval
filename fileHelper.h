/*
    Helper functions to deal with files
    Created by Hui Hu for CS 5330 Computer Vision Spring 2023
*/

#ifndef FILE_HELPER
#define FILE_HELPER

#include <vector>
#include <string>

using namespace std;

int appendFeatureVec(std::string filename, string dirname, string matchingAlgo);

vector<string> readFeatureVec(std::string filename, vector<float> &targetVec, string matchingAlgo, int N = 3);

string convertToString(char *a);
char* convertToCharArray(string s);
void displayImages(vector<string> imageVec, string filename);

#endif
