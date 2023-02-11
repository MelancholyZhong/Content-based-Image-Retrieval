#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include "distances.h"

using namespace std;
//Yao Zhong

//The helper function used to calculate the intersections
float intersection(vector<float> &target, vector<float> &candidate){
    float distance = 0;
    int size = candidate.size();
    for(int i=0; i<size; i++){
        distance += std::min(target[i], candidate[i]);
    }
    return distance;
}

//Task2: The function used to calculate the color histogram distance
float colorDis(vector<float> &target, vector<float> &candidate){
    float distance = intersection(target, candidate);
    return 1-distance; //the high the intersection, the lower the distance
}

//Task4: Calculate the composite distance of color and magnitude histogram. (metric: intersection)
float magnitude_color(std::vector<float> &target, std::vector<float> &candidate, float weight1, float weight2){
    int size = target.size()/2;
    //here target and candidate feature, are a composite of colorhistogram and its magnitude(each 8*8*8).
    std::vector<float> targetColor(target.begin(),target.begin()+size);
    std::vector<float> targetMag(target.begin()+size, target.end());
    std::vector<float> candidateColor(candidate.begin(),candidate.begin()+size);
    std::vector<float> candidateMag(candidate.begin()+size, candidate.end());
    float intersection1 = intersection(targetColor, candidateColor);
    float intersection2 = intersection(targetMag, candidateMag);
    float distance = weight1*intersection1 + weight2*intersection2; //here target and candidate feature, are a composite of colorhistogram and magnitude. *0.5 means both of them weight 0.5
    return 1-distance;
}

//Task5: Calculate the composite distance of color histogram and the spatial variance.
float similarObject(std::vector<float> &target, std::vector<float> &candidate){
    //here target and candidate feature, are a composite of colorhistogram and its spacial variance(each 8*8*8). 
    int size = target.size()/2;
    std::vector<float> targetColor(target.begin(),target.begin()+size);
    std::vector<float> targetSpacial(target.begin()+size, target.end());
    std::vector<float> candidateColor(candidate.begin(),candidate.begin()+size);
    std::vector<float> candidateSpacial(candidate.begin()+size, candidate.end());
    float intersection1 = intersection(targetColor, candidateColor);
    float intersection2 = 0;
    int binCount = 0; //count the valid bins(for normalization later)
    for(int i=0; i<size; i++){
        if(std::isnan(targetSpacial[i]) || targetSpacial[i]==0 || std::isnan(candidateSpacial[i]) || candidateSpacial[i]==0){
            continue;
        }else{
            //here calculated the overlap between the two standard deviations(to get thier similarity)
            intersection2 += std::min(targetSpacial[i], candidateSpacial[i])/std::max(targetSpacial[i], candidateSpacial[i]);
            binCount += 1; //count the valid bins(for normalization later)
        }
    }
    intersection2 = intersection2/binCount; //average overlap rate
    return 1-0.4*intersection1 - 0.6*intersection2;
}


