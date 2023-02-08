/*
    Helper functions to deal with files
*/

#ifndef FILE_HELPER
#define FILE_HELPER

#include <vector>
#include <string>

using namespace std;

int appendFeatureVec(char *filename, char *dirname, char matchingAlgo);

vector<string> readFeatureVec( char *filename, vector<float> targetVec, char* matchingAlgo, int N=3);

#endif
