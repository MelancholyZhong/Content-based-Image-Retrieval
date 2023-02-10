#ifndef DISTANCES
#define DISTANCES

using namespace std;

float intersection(std::vector<float> &target, std::vector<float> &candidate);

float colorDis(vector<float> &target, vector<float> &candidate);

float magnitude_color(std::vector<float> &target, std::vector<float> &candidate);

float similarObject(std::vector<float> &target, std::vector<float> &candidate);

#endif