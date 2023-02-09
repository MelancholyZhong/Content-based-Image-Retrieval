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
        
        std::vector<float> feature1;
        colorHistogram(target, feature1);
        std::vector<float> feature2;
        colorHistogram(candidate, feature2);
        float colorDis = intersection(feature1, feature2);
        std::cout << "color histogram distance between them is: "<< colorDis << std::endl;

        std::vector<float> feature3;
        magnitudeHistogram(target, feature3);
        std::vector<float> feature4;
        magnitudeHistogram(candidate, feature4);
        float mcDis = magnitude_color(feature1, feature2, feature3, feature4);
        std::cout << "magnitude color histogram distance between them is: "<< mcDis << std::endl;

       
        vector<float> feature5;
        objectSpatial(target, feature5);
        vector<float> feature6;
        objectSpatial(candidate, feature6);
        // std::cout<<"counted feature space"<<std::endl;
        // for(auto i:feature6){
        //         std::cout<< i <<" ";
        // }
        // std::cout<<std::endl;
        float taDis = intersection(feature5, feature6);
        std::cout << "tangent angle distance between them is: "<< taDis << std::endl;

        std::vector<float> feature7;
        spacialVariance(target, feature7);
        std::vector<float> feature8;
        spacialVariance(candidate, feature8);
        float sDis = similarObject(feature1, feature2, feature7,feature8);
        std::cout << "spacial distance between them is: "<< sDis << std::endl;
        

        return 0;       
}