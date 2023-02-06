#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include "complex.h"

float magnitude_color(float colorDis, float magDis){
    return (colorDis+magDis)/2;
}