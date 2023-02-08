#include <stdio.h>
#include <iostream>
#include "fileHelper.h"

using namespace std;

int main(){
    char filename[] = "test.csv";
    char dirname[] = "training/";
    char matchAlgo[] = "123";
    appendFeatureVec(filename, dirname, matchAlgo);
    return 0;
}