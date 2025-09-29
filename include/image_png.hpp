#pragma once
#include <vector>
#include <string>
#include <cmath>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

inline void write_png(const char* path, int w, int h, const std::vector<float>& rgb) {
    std::vector<unsigned char> data(w * h * 3);
    for (int i = 0; i < w * h * 3; ++i) {
        float v = rgb[i];
        v = v < 0.f ? 0.f : (v > 1.f ? 1.f : v);
        data[i] = static_cast<unsigned char>(std::lround(v * 255.f));
    }
    stbi_write_png(path, w, h, 3, data.data(), w * 3);
}
