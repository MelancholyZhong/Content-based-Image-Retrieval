#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include "distances.h"

using namespace std;

float intersection(vector<float> &target, vector<float> &candidate){
    float distance = 0;
    int size = candidate.size();
    for(int i=0; i<size; i++){
        distance += std::min(target[i], candidate[i]);
    }
    return distance;
}


