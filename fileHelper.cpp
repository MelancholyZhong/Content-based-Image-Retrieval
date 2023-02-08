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
#include <queue> // for min heap

// openCV
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>


using namespace std;
using namespace cv;

/*
  AppendFeatureVec function: append the feature vector data to CSV file
    - filename: the name of CSV file
    - dirname: the name of directory
    - matchingAlgo: the code for chosen matching algorithm

  Date sample:
  "imagename,float,float,float,...float\n"
 */
int appendFeatureVec(char *filename, char *dirname, char matchingAlgo)
{
    // Variables for CSV file
    char buffer[256];
    char mode[8];
    FILE *fp;
    bool reset = false;
    vector<int> image_data = {1, 2, 3, 4};
    Mat featureVec;
    // Variables for Directory
    DIR *dirp;
    struct dirent *dp;

    // Open the directory
    dirp = opendir(dirname);
    if (dirp == NULL)
    {
        printf("Cannot open directory %s\n", dirname);
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

    fp = fopen(filename, mode);

    if (!fp)
    {
        printf("Unable to open output file %s\n", filename);
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
            strcpy(buffer, dirname);
            strcat(buffer, "/");
            strcat(buffer, dp->d_name);

            // To do, get the image feature vector and save it
            // printf("full path name: %s\n", buffer);
            // Write the imagename and the feature vector to the CSV file
            fwrite(dp->d_name, sizeof(char), strlen(dp->d_name), fp);


            // Get the feature vector
            switch (matchingAlgo)
            {
            case 'b':
                
                break;
            default:
                break;
            }

            // Write feature vector to CSV file
            char tmp[256];
            for (int i = 0; i < image_data.size(); i++)
            {
                // Write formatted data to string
                sprintf(tmp, ",%.4f", (float)image_data[i]);
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

int readFeatureVec(char *filename, std::vector<char *> &filenames, std::vector<std::vector<float>> &data, int echo_file)
{
}

// Global variable
map<string, float> dict; // store (image_name, distance) pairs

// To compare two points
class Comparator
{
public:
    int operator()(const string &name1, const string &name2)
    {
        return dict[name1] < dict[name2];
    }
};

vector<string> readFeatureVec(char *filename, vector<float> &targetVec, char matchingAlgo, int N = 3)
{
    vector<string> result = {};
    priority_queue<string, vector<string>, Comparator> minHeap;
    FILE *fp;

    char imagename[256];
    float distance, value;
    vector<float> featureVec;

    // Open CSV file
    fp = fopen(filename, "r");
    if (!fp)
    {
        printf("Unable to open feature file\n");
        return -1;
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
        switch (matchingAlgo)
        {
        case 'b':
            distance = 1;
            break;
        default:
            distance = 3;
            break;
        }

        // Add new (image_name, distance) pair to dict
        dict[imagename] = distance;

        // Add new distance to min-Heap
        minHeap.push(imagename);
        if (minHeap.size() > N)
        {
            minHeap.pop();
        }
    }

    // Close CSV file
    fclose(fp);
    printf("Finished reading CSV file\n");

    // Return names of the N most close images 
    while(!minHeap.empty()){
        result.push_back(minHeap.top());
        minHeap.pop();
    }

    return result;
}
