#pragma once
#include <string>

struct Params {
    int width = 1200;
    int height = 1200;
    int n = 5; 
    int maxIters = 50;
    float eps = 1e-6f;
    float xmin = -2.f, xmax = 2.f;
    float ymin = -2.f, ymax = 2.f;
    std::string outPath = "newton.ppm";
};

Params parse_args(int argc, char* argv[]);