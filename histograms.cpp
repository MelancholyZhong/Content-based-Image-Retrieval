#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include "histograms.h"

using namespace std;


int colorHistogram(cv::Mat &src, std::vector<float> &feature, bool center){
    int size = 8*8*8;
    feature = vector<float>(size, 0.0);
    int r = center ? src.rows / 3 : 0;
    int c = src.cols / 3;

    //counting
    for(int i= 0 + r; i<src.rows - r ; i++){
        cv::Vec3b *rptr = src.ptr<cv::Vec3b>(i);
        for(int j=0 + c;j<src.cols - c;j++){
            int bin0 = rptr[j][0]/32;  //determin which bin it is, here are 8 bins for [0,255]
            int bin1 = rptr[j][1]/32;
            int bin2 = rptr[j][2]/32;
            int bin = bin0*8*8 + bin1*8 + bin2;
            feature[bin] += 1;
        }
    }

     //normalization
    for(int i=0; i<feature.size(); i++){
        feature[i] = feature[i]/((src.rows - 2 * r) * (src.cols - 2 * c));  
    }
    return 0;
}


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

// a feature that descibes the main color's dstribution in the space
int objectSpatial(cv::Mat &src, std::vector<float> &feature){
    //Calculate the primary colors in the image
    std::vector<float> colorDensity;
    colorHistogram(src, colorDensity);
    int mainColor = 0;
    float maxDistribution = 0.0;
    for(int i=0; i<colorDensity.size(); i++){
        if(colorDensity[i]>maxDistribution){
            mainColor = i;
            maxDistribution = colorDensity[i];
        }
    }

    //calculate how this color is preaded in this image(16 bins in both x and y)
    std::vector<float> spacialSpread = std::vector<float>(32, 0.0);
    int xBinLength = (src.cols+15)/16; //+15 to make sure the 16 bins contains all pixels.
    int yBinLength = (src.rows+15)/16;
    int pixelCount = 0;
    //counting
    for(int i=0;i<src.rows; i++){
        cv::Vec3b *rptr = src.ptr<cv::Vec3b>(i);
        int binY = i/yBinLength;
        for(int j=0;j<src.cols;j++){
            int binX = j/xBinLength;
            int bin0 = rptr[j][0]/32;  //determin which color bin it is, here are 8 bins for [0,255]
            int bin1 = rptr[j][1]/32;
            int bin2 = rptr[j][2]/32;
            int bin = bin0*8*8 + bin1*8 + bin2;
            if(bin == mainColor){
                pixelCount += 1;
                spacialSpread[binX] += 1;
                spacialSpread[16+binY] += 1;
            }
        }
    }

    //normalization
    for(int i=0; i<spacialSpread.size(); i++){
        spacialSpread[i] = spacialSpread[i]/(pixelCount*2);
    }

    feature = spacialSpread;
    return 0;
}


int spacialVariance(cv::Mat &src, std::vector<float> &feature){
    int size = 8*8*8;
    std::vector<float> m00= vector<float>(size, 0.0); //m00 is the pixel count for each color region
    std::vector<float> m10= vector<float>(size, 0.0); //m10 is the x total value for each color region, used to caluate x_av
    std::vector<float> m01= vector<float>(size, 0.0); //m01 similar to m10
    std::vector<float> miu22 = vector<float>(size, 0.0); //miu22 is to hold the total distance from each pixel to its average center.

    for(int i=0;i<src.rows; i++){
        cv::Vec3b *rptr = src.ptr<cv::Vec3b>(i);
        for(int j=0;j<src.cols;j++){
            int bin0 = rptr[j][0]/32;  //determin which bin it is, here are 8 bins for [0,255]
            int bin1 = rptr[j][1]/32;
            int bin2 = rptr[j][2]/32;
            int bin = bin0*8*8 + bin1*8 + bin2;
            m00[bin] += 1;
            m10[bin] += i;
            m01[bin] += j;
        }
    }
    
    
    std::vector<float> x_av = std::vector<float>(size, 0.0);
    std::vector<float> y_av = std::vector<float>(size, 0.0);
    for(int i=0; i<size; i++){
        x_av[i] = m10[i]/m00[i];
        y_av[i] = m01[i]/m00[i];
    }

    for(int i=0;i<src.rows; i++){
        cv::Vec3b *rptr = src.ptr<cv::Vec3b>(i);
        for(int j=0;j<src.cols;j++){
            int bin0 = rptr[j][0]/32;  //determin which bin it is, here are 8 bins for [0,255]
            int bin1 = rptr[j][1]/32;
            int bin2 = rptr[j][2]/32;
            int bin = bin0*8*8 + bin1*8 + bin2;
            miu22[bin] += ((i-x_av[bin])*(i-x_av[bin]) + (j-y_av[bin])*(j-y_av[bin]));
        }
    }

    feature = std::vector<float>(size, 0.0);
    for(int i=0; i<size; i++){
        feature[i] = miu22[i]/m00[i];
    }
    
    return 0;
}

int twoComposite(std::vector<float> &feature1, std::vector<float> &feature2){
    feature1.insert(feature1.end(), feature2.begin(), feature2.end());
    return 0;
}