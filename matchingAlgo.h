/**
    Matching Algorithm for Content-based Image Retrieval (CBIR)
*/

#ifndef MATCHING_ALGO
#define MATCHING_ALGO

#include <vector>
#include <string>
using namespace std;

// Use the 9x9 square in the middle of the image as a feature vector
// Use sum-of-squared-difference as the distance metric
int baselineMatch(Mat &src, Mat_<Vec3b> featureVec);


int histogramMatching();

int multiHistogramMatching();

vector<string> getFristN(int N=3);

#endif