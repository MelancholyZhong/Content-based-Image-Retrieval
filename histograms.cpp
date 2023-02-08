#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include "histograms.h"

using namespace std;

//Solbel filters and magnitude filters from project #1 used in this problem

int sobel_calc(cv::Mat &src, cv::Mat &dst, int (&sobel)[3][3]){
    dst = cv::Mat::zeros(src.size(), CV_16SC3);
    for(int i=0;i<src.rows; i++){
        cv::Vec3s *drptr = dst.ptr<cv::Vec3s>(i); //https://docs.opencv.org/2.4/modules/core/doc/basic_structures.html#vec
        for(int j=0;j<src.cols;j++){
            if(i<=0 || i>=src.rows-1 || j<=0 || j>=src.cols-1){
                cv::Vec3b *srptr = src.ptr<cv::Vec3b>(i);
                drptr[j][0] = srptr[j][0]; //https://stackoverflow.com/questions/34851462/what-happens-when-show-a-signed-image-in-opencv
                drptr[j][1] = srptr[j][1];
                drptr[j][2] = srptr[j][2];
            }else{
                int channels[3] = {0,0,0};
                for(int channel = 0; channel<3;channel++){
                    for(int row = -1; row<2; row++){
                        cv::Vec3b *srptr = src.ptr<cv::Vec3b>(i+row);
                        for(int col = -1; col<2;col++){
                            channels[channel] += srptr[j+col][channel]*sobel[row+1][col+1];
                        }
                    }
                }
                drptr[j][0] = channels[0]/4;
                drptr[j][1] = channels[1]/4;
                drptr[j][2] = channels[2]/4;
            }
            
        }
    }
    return 0;
}

int sobelX3x3(cv::Mat &src, cv::Mat &dst ){
    int xsobel[3][3] = {
        {-1,0,1},
        {-2,0,2},
        {-1,0,1},
    } ;
    sobel_calc(src,dst, xsobel);
    return 0;
}

int sobelY3x3( cv::Mat &src, cv::Mat &dst ){
    int ysobel[3][3] = {
        {1,2,1},
        {0,0,0},
        {-1,-2,-1},
    } ;
    sobel_calc(src,dst, ysobel);
    return 0;
}

int magnitude( cv::Mat &sx, cv::Mat &sy, cv::Mat &dst ){
    dst = cv::Mat::zeros(sx.size(), CV_16SC3);
    for(int i=0;i<sx.rows; i++){
        cv::Vec3s *drptr = dst.ptr<cv::Vec3s>(i);
        cv::Vec3s *xrptr = sx.ptr<cv::Vec3s>(i);
        cv::Vec3s *yrptr = sy.ptr<cv::Vec3s>(i);
        for(int j=0;j<sx.cols;j++){
            drptr[j][0] = int(std::sqrt(xrptr[j][0]*xrptr[j][0]+yrptr[j][0]*yrptr[j][0]));
            drptr[j][1] = int(std::sqrt(xrptr[j][1]*xrptr[j][1]+yrptr[j][1]*yrptr[j][1]));
            drptr[j][2] = int(std::sqrt(xrptr[j][2]*xrptr[j][2]+yrptr[j][2]*yrptr[j][2]));
        }
    }
    cv::convertScaleAbs(dst,dst);
    return 0;
}


int magnitudeHistogram(cv::Mat &src, std::vector<float> &feature){
    int size = 8*8*8;
    feature = vector<float>(size, 0.0);

    //calculate the magnitude version of the image
    cv::Mat xSobel;
    cv::Mat ySobel;
    cv::Mat magnitudeImg;
    sobelX3x3(src,xSobel);
    sobelY3x3(src,ySobel);
    magnitude(xSobel, ySobel, magnitudeImg);

    //counting
    for(int i=0;i<src.rows; i++){
        cv::Vec3b *rptr = magnitudeImg.ptr<cv::Vec3b>(i);
        for(int j=0;j<src.cols;j++){
            int bin0 = rptr[j][0]/32;  //determin which bin it is, here are 8 bins for [0,255]
            int bin1 = rptr[j][1]/32;
            int bin2 = rptr[j][2]/32;
            int bin = bin0*8*8 + bin1*8 + bin2;
            feature[bin] += 1;
        }
    }

    //normalization
    for(int i=0; i<feature.size(); i++){
        feature[i] = feature[i]/(src.rows*src.cols);  
    }
    return 0;
}


int calculateBin(int centerX, int centerY, int x, int y){
    if(centerX == x){
        return 9;
    }
    double fraction = (double)(centerY-y)/(centerX-x);
    int bin = (int) (std::atan(fraction)*18/3.1415926)%18;
    return bin;
}

int tangentAngle(cv::Mat &src, vector<float> &feature){
    //calculate the magnitude version of the image
    cv::Mat xSobel;
    cv::Mat ySobel;
    cv::Mat magnitudeImg;
    sobelX3x3(src,xSobel);
    sobelY3x3(src,ySobel);
    magnitude(xSobel, ySobel, magnitudeImg);
    std::cout << "magnitude calculated"<< std::endl;

    feature = vector<float>(18, 0.0);
    int pixelCount = 0;
    int centerX = src.cols/2;
    int centerY = src.rows/2;

    for(int i=0;i<src.rows; i++){
        cv::Vec3b *rptr = magnitudeImg.ptr<cv::Vec3b>(i);
        for(int j=0;j<src.cols;j++){
           if(rptr[j][0]>15 && rptr[j][1]>15 && rptr[j][2]>15){
                pixelCount += 1;
                int bin = calculateBin(centerX, centerY, j, i);
                feature[bin] += 1;
           }
        }
    }
    std::cout << "calculated angles: "<< std::endl;

    for(int i=0; i<feature.size(); i++){
        feature[i] = feature[i]/pixelCount;
    }
    std::cout << "this feature finised "<< std::endl;


    return 0;
}


