//
//  Created by Yao Zhong on 1/20/23.
//

#include <iostream>

#include <opencv2/opencv.hpp>

#include "histograms.h"
#include "distances.h"

using namespace std;

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

        std::cout << "tangent angle distance between them is: "<< std::endl;
        
        // cv::Mat feature1;
        // colorHistogramCalc(target, feature1);
        // cv::Mat feature2;
        // colorHistogramCalc(candidate, feature2);

        // float colorDis = colorHistogramDis(feature1, feature2);
        // std::cout << "color histogram distance between them is: "<< colorDis << std::endl;

        std::vector<float> feature3;
        magnitudeHistogram(target, feature3);
        std::vector<float> feature4;
        magnitudeHistogram(candidate, feature4);
        float magDis = intersection(feature3, feature4);
        std::cout << "magnitude histogram distance between them is: "<< magDis << std::endl;

        // float mag_color = magnitude_color(colorDis, magDis);
        // std::cout << "magnitude_color distance between them is: "<< mag_color << std::endl;
                
        // vector<float> feature6;
        // tangentAngle(candidate, feature6);
        // for(auto i:feature6){
        //         std::cout<<i<<" ";
        // }
        // std::cout<<std::endl;
        // float taDis = intersection(feature5, feature6);
        // std::cout << "tangent angle distance between them is: "<< taDis << std::endl;

        return 0;       
}