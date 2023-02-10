/*
    Created by Hui Hu for CS 5330 Computer Vision Spring 2023
*/

#include <stdio.h>  /* printf, fgets */
#include <stdlib.h> /* atoi */
#include <cstring>
#include <map>
#include <vector>
#include <iostream>
#include <dirent.h>
#include "matchingAlgo.h"
#include "histograms.h"
#include "distances.h"
#include <queue>     // for min heap
#include <algorithm> // std::reverse
#include <string>

// openCV
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
// #include <opencv2/imgcodecs.hpp>
// #include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

char *convertToCharArray(string s)
{
    char *char_arr = &s[0];
    return char_arr;
}

// converts character array to string and returns it
string convertToString(char *a)
{
    string s(a);
    // int size = sizeof(a) / sizeof(char);
    // int i;
    // string s = "";
    // for (i = 0; i < size; i++)
    // {
    //     s = s + a[i];
    // }
    return s;
}

/*
  AppendFeatureVec function: append the feature vector data to CSV file
    - filename: the name of CSV file
    - dirname: the name of directory
    - matchingAlgo: the code for chosen matching algorithm

  Date sample:
  "imagename,float,float,float,...float\n"
 */
int appendFeatureVec(std::string filename, string dirname, string matchingAlgo)
{
    // Variables for CSV file
    char buffer[256];
    char mode[8];
    FILE *fp;
    bool reset = false;
    // Variables for Directory
    DIR *dirp;
    struct dirent *dp;
    // Temp variabls
    vector<float> feature;
    Mat candidate;
    char tmp[256];

    // Open the directory
    dirp = opendir(convertToCharArray(dirname));
    if (dirp == NULL)
    {
        printf("Cannot open directory %s\n", convertToCharArray(dirname));
        exit(-1);
    }

    // Open CSV file
    if (reset)
    {
        // If file exists, erase all the contents; if file doesn't exist, create new file
        strcpy(mode, "w");
    }
    else
    {
        // If file exists, start writing from the end; if file doesn't exist, create new file
        strcpy(mode, "a");
    }

    fp = fopen(convertToCharArray(filename), mode);

    if (!fp)
    {
        printf("Unable to open output file %s\n", convertToCharArray(filename));
        exit(-1);
    }

    // Loop over all the files in the image file listing
    while ((dp = readdir(dirp)) != NULL)
    {
        // check if the file is an image
        if (strstr(dp->d_name, ".jpg") ||
            strstr(dp->d_name, ".png") ||
            strstr(dp->d_name, ".ppm") ||
            strstr(dp->d_name, ".tif"))
        {
            // Build the overall filename
            strcpy(buffer, convertToCharArray(dirname));
            strcat(buffer, "/");
            strcat(buffer, dp->d_name);

            // To do, get the image feature vector and save it
            // printf("full path name: %s\n", buffer);
            // Write the imagename and the feature vector to the CSV file
            fwrite(dp->d_name, sizeof(char), strlen(dp->d_name), fp);

            // Get candidate image
            candidate = imread(buffer);

            if (matchingAlgo == "baseline")
            {
                baselineMatch(candidate, feature);
            }
            else if (matchingAlgo == "color")
            {
                colorHistogram(candidate, feature);
            }
            else if (matchingAlgo == "magnitude")
            {
                colorHistogram(candidate, feature);
                vector<float> feature2;
                magnitudeHistogram(candidate, feature2);
                twoComposite(feature, feature2);
            }
            else if (matchingAlgo == "objectSpacial")
            {
                objectSpatial(candidate, feature);
            }
            else if (matchingAlgo == "spacialVariance")
            {
                colorHistogram(candidate, feature);
                vector<float> feature2;
                spacialVariance(candidate, feature2);
                twoComposite(feature, feature2);
            }
            else if (matchingAlgo=="laws")
            {
                lawsMatch(candidate, feature);
            }
            else if (matchingAlgo=="multiHistogram")
            {
                multiHistogramMatch(candidate, feature);
            }
            else if (matchingAlgo=="coMatrix")
            {
                comatrixMatch(candidate, feature);
            }
            else if (matchingAlgo=="gradient")
            {
                gradientMatch(candidate, feature);
            }
            else
            {
                std::cout << "Please input correct method"<<std::endl;
                exit(-1);
            }

            // Write feature vector to CSV file

            for (int i = 0; i < feature.size(); i++)
            {
                // Write formatted data to string
                sprintf(tmp, ",%.4f", feature[i]);
                fwrite(tmp, sizeof(char), strlen(tmp), fp);
            }
            fwrite("\n", sizeof(char), 1, fp); // EOL
        }
    }

    // Close CSV file
    fclose(fp);

    return 0;
}

/*
  reads a string from a CSV file. the 0-terminated string is returned in the char array os.
  The function returns false if it is successfully read. It returns true if it reaches the end of the line or the file.
 */
int getstring(FILE *fp, char os[])
{
    int p = 0;
    int eol = 0;

    for (;;)
    {
        char ch = fgetc(fp);
        if (ch == ',')
        {
            break;
        }
        else if (ch == '\n' || ch == EOF)
        {
            eol = 1;
            break;
        }
        os[p] = ch;
        p++;
    }
    os[p] = '\0';

    return (eol); // return true if eol
}

/*
  Utility function for reading one float value from a CSV file

  The value is stored in the v parameter

  The function returns true if it reaches the end of a line or the file
 */
int getfloat(FILE *fp, float *v)
{
    char s[256];
    int p = 0;
    int eol = 0;

    for (;;)
    {
        char ch = fgetc(fp);
        if (ch == ',')
        {
            break;
        }
        else if (ch == '\n' || ch == EOF)
        {
            eol = 1;
            break;
        }

        s[p] = ch;
        p++;
    }
    s[p] = '\0';  // terminator
    *v = atof(s); // convert string to float

    return (eol); // return true if eol
}

/*
  Given a file with the format of a string as the first column and
  floating point numbers as the remaining columns, this function
  returns the filenames as a std::vector of character arrays, and the
  remaining data as a 2D std::vector<float>.

  filenames will contain all of the image file names.
  data will contain the features calculated from each image.

  If echo_file is true, it prints out the contents of the file as read
  into memory.

  The function returns a non-zero value if something goes wrong.
 */

// Global variable
map<string, float> dict; // store (image_name, distance) pairs

// To compare two iamges according to their distances with target image
class Comparator
{
public:
    int operator()(const string &name1, const string &name2)
    {
        return dict[name1] < dict[name2];
    }
};


vector<string> readFeatureVec(std::string filename, vector<float> &targetVec, string matchingAlgo, int N = 3)
{
    vector<string> result = {};
    priority_queue<string, vector<string>, Comparator> minHeap;
    FILE *fp;

    char imagename[256];
    float distance, value;
    vector<float> featureVec;

    // Open CSV file
    fp = fopen(convertToCharArray(filename), "r");
    if (!fp)
    {
        printf("Unable to open feature file\n");
        return result;
    }

    // Read file
    for (;;)
    {
        featureVec = {};

        // Read image name, if reached the last line, exit loop
        if (getstring(fp, imagename))
            break;

        // Read feature vector
        for (;;)
        {
            float eol = getfloat(fp, &value);
            featureVec.push_back(value);
            if (eol)
                break;
        }

        // Get distance

        if (matchingAlgo.compare("baseline") == 0)
        {
            distance = baselineDis(targetVec, featureVec);
        }
        else if (matchingAlgo.compare("color") == 0)
        {
            distance = colorDis(targetVec, featureVec); // colorHistogram(targetVec, featureVec);
        }
        else if (matchingAlgo.compare("magnitude") == 0)
        {
            distance = magnitude_color(targetVec, featureVec); // magnitudeHistogram(targetVec, featureVec);
        }
        else if (matchingAlgo.compare("objectSpacial") == 0)
        {
            distance = 1 - intersection(targetVec, featureVec);
        }
        else if (matchingAlgo.compare("spacialVariance") == 0)
        {
            distance = similarObject(targetVec, featureVec); // spacialVariance(targetVec, featureVec);
        }
        else if (matchingAlgo.compare("laws") == 0)
        {
            distance = histogramDis(targetVec, featureVec);
        }
        else if (matchingAlgo.compare("multiHistogram") == 0)
        {
            distance = histogramDis(targetVec, featureVec);
        }
        else if (matchingAlgo.compare("coMatrix") == 0)
        {
            distance = comatrixDis(targetVec, featureVec);
        }
        else if (matchingAlgo.compare("gradient") == 0)
        {
            distance = histogramDis(targetVec, featureVec);
        }
        else
        {
            distance = baselineDis(targetVec, featureVec);
        }

        // Add new (image_name, distance) pair to dict
        dict[imagename] = distance;

        // Add new distance to min-Heap
        if (distance > 0.001) // Avoid target image itself
        {
            minHeap.push(imagename);
        }
        if (minHeap.size() > N)
        {
            minHeap.pop();
        }
    }

    // Close CSV file
    fclose(fp);

    // Return names of the N most close images
    while (!minHeap.empty())
    {
        result.push_back(minHeap.top());
        minHeap.pop();
    }

    reverse(result.begin(), result.end());

    return result;
}

// Display all images horizontally
void displayImages(vector<string> imageVec, string dirPath)
{
    string imagePath;

    // Get images
    vector<Mat> images(imageVec.size());
    for (int i = 0; i < imageVec.size(); i++)
    {
        imagePath = dirPath + "/"+ imageVec[i];
        images[i] = imread(imagePath);
    }

    cv::String windowName = "imgWindow";
    cv::namedWindow(windowName);
    int i=0;
    cv::imshow(windowName, images[i]);
    char keyCode = cv::waitKey(0);
    while(keyCode!='q'){
        if(keyCode == 'n' && i<images.size()-1){
            i++;
            cv::imshow(windowName, images[i]);
        }else if(keyCode == 'p' && i>0){
            i--;
            cv::imshow(windowName, images[i]);
        }
        keyCode = cv::waitKey(0);
    }
    cv::destroyWindow(windowName);
    return;
}

/*
void ShowManyImages(string title, int nArgs, ...)
{
    int size;
    int i;
    int m, n;
    int x, y;

    // w - Maximum number of images in a row
    // h - Maximum number of images in a column
    int w, h;

    // scale - How much we have to resize the image
    float scale;
    int max;

    // Determine the size of the image,
    // and the number of rows/cols
    // from number of arguments
    if (nArgs == 1)
    {
        w = h = 1;
        size = 300;
    }
    else if (nArgs == 2)
    {
        w = 2;
        h = 1;
        size = 300;
    }
    else if (nArgs == 3 || nArgs == 4)
    {
        w = 2;
        h = 2;
        size = 300;
    }
    else if (nArgs == 5 || nArgs == 6)
    {
        w = 3;
        h = 2;
        size = 200;
    }
    else if (nArgs == 7 || nArgs == 8)
    {
        w = 4;
        h = 2;
        size = 200;
    }
    else
    {
        w = 4;
        h = 3;
        size = 150;
    }

    // Create a new 3 channel image
    Mat DispImage = Mat::zeros(Size(100 + size * w, 60 + size * h), CV_8UC3);

    // Used to get the arguments passed
    va_list args;
    va_start(args, nArgs);

    // Loop for nArgs number of arguments
    for (i = 0, m = 20, n = 20; i < nArgs; i++, m += (20 + size))
    {
        Mat img = va_arg(args, Mat);

        // Find the width and height of the image
        x = img.cols;
        y = img.rows;

        // Find whether height or width is greater in order to resize the image
        max = (x > y) ? x : y;

        // Find the scaling factor to resize the image
        scale = (float)((float)max / size);

        // Used to Align the images
        if (i % w == 0 && m != 20)
        {
            m = 20;
            n += 20 + size;
        }

        // Set the image ROI to display the current image
        // Resize the input image and copy the it to the Single Big Image
        Rect ROI(m, n, (int)(x / scale), (int)(y / scale));
        Mat temp;
        resize(img, temp, Size(ROI.width, ROI.height));
        temp.copyTo(DispImage(ROI));
    }

    // Create a new window, and show the Single Big Image
    namedWindow(title, 1);
    imshow(title, DispImage);
    waitKey();

    // End the number of arguments
    va_end(args);
}

*/