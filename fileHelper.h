/*
    File-related funtion for Content-based Image Retrieval (CBIR)
    Created by Hui Hu for CS 5330 Computer Vision Spring 2023

    Functions to write feature vector to CSV files and read them from CSV files
*/

#ifndef FILE_HELPER
#define FILE_HELPER

#include <string>
#include <vector>

using namespace std;

// Append the feature vector data to CSV file
int appendFeatureVec(std::string filename, std::string dirname, std::string matchingAlgo);

// Read the feature vector data from CSV file
std::vector<std::string> readFeatureVec(std::string filename, std::string targetname, std::vector<float> &targetVec, std::string matchingAlgo, int N = 3);

// Heleper function, convert string to character array and returns it
char *convertToCharArray(std::string s);

// Heleper function, convert character array to string and returns it
std::string convertToString(char *a);

// Display all images using opencv GUI
void displayImages(std::vector<string> imageVec, std::string filename);

#endif
