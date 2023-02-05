//
//  Created by Yao Zhong on 1/20/23.
//

#include <iostream>

#include <opencv2/opencv.hpp>

#include "colorHistogram.h"

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
        float distance = colorHistogramDis(feature1, feature2);
        std::cout << "distance between them is: "<< distance << std::endl;

        
        return 0;       
}