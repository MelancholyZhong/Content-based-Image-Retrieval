#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include "colorHistogram.h"

int colorHistogramCalc(cv::Mat &src, cv::Mat &feature){
    int size[] = {8, 8, 8};
    feature = cv::Mat::zeros(3, size, CV_32FC1);

    //counting
    for(int i=0;i<src.rows; i++){
        cv::Vec3b *rptr = src.ptr<cv::Vec3b>(i);
        for(int j=0;j<src.cols;j++){
            int bin0 = rptr[j][0]/32;  //determin which bin it is, here are 8 bins for [0,255]
            int bin1 = rptr[j][1]/32;
            int bin2 = rptr[j][2]/32;
            float *fptr = feature.ptr<float>(bin0,bin1);
            fptr[bin2] += 1;
        }
    }

    //normalization
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            float *fptr = feature.ptr<float>(i,j);
            for(int k=0; k<8; k++){
                fptr[k] = fptr[k]/(src.rows*src.cols);
            }
        }
    }
    return 0;
}

float colorHistogramDis(verctor<float> &target, verctor<float> &candidate){
    float distance = 0;
    int size = candidate.size();
    for(int i=0; i<size; i++){
        distance += std::min(target[i], candidate[i]);
    }
    return distance;
}