#ifndef DISTANCES
#define DISTANCES

float intersection(std::vector<float> &target, std::vector<float> &candidate);

float magnitude_color(std::vector<float> &targetColor, std::vector<float> &candidateColor, std::vector<float> &targetMag, std::vector<float> &candidateMag);

float similarObject(std::vector<float> &targetColor, std::vector<float> &candidateColor, std::vector<float> &targetSpacial, std::vector<float> &candidateSpacial);

#endif