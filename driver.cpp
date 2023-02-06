//
//  Created by Yao Zhong on 1/20/23.
//

#include <iostream>

#include <opencv2/opencv.hpp>

#include "colorHistogram.h"
#include "texture.h"
#include "complex.h"

int main(int argc, char *argv[]) {
        cv::Mat target;
        cv::Mat candidate;
        if(argc <3){
                exit(-2);
        }else{
                char* targetDir = argv[1];
                target = cv::imread(targetDir);
                char* candiDir = argv[2];
                candidate = cv::imread(candiDir);
        }
        
        if(target.empty() || candidate.empty()){
                std::cout << "Can't read the image" << std::endl;
                exit(-1);
        }
        
        cv::Mat feature1;
        colorHistogramCalc(target, feature1);
        cv::Mat feature2;
        colorHistogramCalc(candidate, feature2);
        float colorDis = colorHistogramDis(feature1, feature2);
        std::cout << "color histogram distance between them is: "<< colorDis << std::endl;

        cv::Mat feature3;
        magnitudeHistogramCalc(target, feature3);
        cv::Mat feature4;
        magnitudeHistogramCalc(candidate, feature4);
        float magDis = magnitudeHistogramDis(feature3, feature4);
        std::cout << "magnitude histogram distance between them is: "<< magDis << std::endl;

        float mag_color = magnitude_color(colorDis, magDis);
        std::cout << "magnitude_color distance between them is: "<< mag_color << std::endl;

        return 0;       
}