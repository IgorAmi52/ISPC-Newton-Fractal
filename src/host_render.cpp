#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <iostream>
#include "../include/host_render.hpp"
#include "fractal_ispc.h"


void render_image(const Params& p,
    const std::vector<float>& rootsRe,
    const std::vector<float>& rootsIm,
    std::vector<float>& outRGB)
{
    outRGB.assign(3 * p.width * p.height, 0.f);

    ispc::render(p.width, p.height,
        p.xmin, p.xmax, p.ymin, p.ymax,
        p.n, p.maxIters, p.eps,
        const_cast<float*>(rootsRe.data()),
        const_cast<float*>(rootsIm.data()),
        outRGB.data(),
        p.benchMinRows);
}


void benchmark_render(const Params& p,
    const std::vector<float>& rootsRe,
    const std::vector<float>& rootsIm)
{
    std::vector<float> outRGB(p.width * p.height * 3, 0.0f);
    std::cout << "\nBenchmarking Newton Fractal ISPC" << std::endl;
    std::cout << "Image size: " << p.width << "x" << p.height << ", n: " << p.n << ", eps: " << p.eps << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << " rowsPerTask | maxIters |   time (ms)" << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;

    double bestTime = 1e30;
    int bestRows = 0;

    for (int rows = p.benchMinRows; rows <= p.benchMaxRows; rows += p.benchStepRows) {
        auto start = std::chrono::high_resolution_clock::now();
        ispc::render(p.width, p.height,
            p.xmin, p.xmax, p.ymin, p.ymax,
            p.n, p.maxIters, p.eps,
            const_cast<float*>(rootsRe.data()),
            const_cast<float*>(rootsIm.data()),
            outRGB.data(),
            rows);
        auto end = std::chrono::high_resolution_clock::now();
        double ms = std::chrono::duration<double, std::milli>(end - start).count();
        std::cout << std::setw(11) << rows << " | " << std::setw(8) << p.maxIters << " | " << std::setw(10) << std::fixed << std::setprecision(2) << ms << std::endl;
        if (ms < bestTime) {
            bestTime = ms;
            bestRows = rows;
        }
    }
    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "Best: rowsPerTask=" << bestRows << ", maxIters=" << p.maxIters << ", time=" << std::fixed << std::setprecision(2) << bestTime << " ms" << std::endl;
}