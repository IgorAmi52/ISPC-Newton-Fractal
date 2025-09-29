#pragma once
#include <vector>
#include "config.hpp"


void render_image(const Params& p,
    const std::vector<float>& rootsRe,
    const std::vector<float>& rootsIm,
    std::vector<float>& outRGB);