#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include "texture.h"

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

//magnitude histogram calculations

int magnitudeHistogramCalc(cv::Mat &src, cv::Mat &feature){
    int size[] = {8, 8, 8};
    feature = cv::Mat::zeros(3, size, CV_32FC1);

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

float magnitudeHistogramDis(cv::Mat &target, cv::Mat &candidate ){
    float distance = 0;
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            float *tptr = target.ptr<float>(i,j);
             float *cptr = candidate.ptr<float>(i,j);
            for(int k=0; k<8; k++){
               distance += std::min(tptr[k], cptr[k]);
            }
        }
    }
    return distance;
}

