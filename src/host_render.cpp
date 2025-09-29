#include "../include/host_render.hpp"
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cmath>
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
        outRGB.data());
}