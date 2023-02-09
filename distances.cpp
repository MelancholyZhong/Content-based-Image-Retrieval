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
    return 1- distance;
}

float magnitude_color(std::vector<float> &targetColor, std::vector<float> &candidateColor, std::vector<float> &targetMag, std::vector<float> &candidateMag){
    float colorDis = intersection(targetColor, candidateColor);
    float magDis = intersection(targetMag, candidateMag);
    float distance = 0.5*colorDis + 0.5*magDis;
    return distance;
}

float similarObject(std::vector<float> &targetColor, std::vector<float> &candidateColor, std::vector<float> &targetSpacial, std::vector<float> &candidateSpacial){
    int size = targetColor.size();
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


