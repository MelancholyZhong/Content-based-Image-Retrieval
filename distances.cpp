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

float colorDis(vector<float> &target, vector<float> &candidate){
    float distance = intersection(target, candidate);
    return 1-distance; //the high the intersection, the lower the distance
}

float magnitude_color(std::vector<float> &target, std::vector<float> &candidate){
    float twoIntersection = intersection(target, candidate);
    float distance = 0.5*twoIntersection; //here target and candidate feature, are a composite of colorhistogram and magnitude. *0.5 means both of them weight 0.5
    return 1-distance;
}

float similarObject(std::vector<float> &target, std::vector<float> &candidate){
    //here target and candidate feature, are a composite of colorhistogram and its spacial variance(each 8*8*8). 
    int size = target.size()/2;
    std::vector<float> targetColor(target.begin(),target.begin()+size);
    std::vector<float> targetSpacial(target.begin()+size, target.end());
    std::vector<float> candidateColor(candidate.begin(),candidate.begin()+size);
    std::vector<float> candidateSpacial(candidate.begin()+size, candidate.end());
    float distance = 0;
    for(int i=0; i<size; i++){
        float similarity;
        if(std::isnan(targetSpacial[i]) || targetSpacial[i]==0 || std::isnan(candidateSpacial[i]) || candidateSpacial[i]==0){
            similarity = 0.0;
        }else{
            similarity = std::min(targetSpacial[i], candidateSpacial[i])/std::max(targetSpacial[i], candidateSpacial[i]);
        }
        distance += std::min(targetColor[i], candidateColor[i])*similarity;
    }
    return 1-distance;
}


