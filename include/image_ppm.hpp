#pragma once
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cmath>


inline void write_ppm(const char* path, int w, int h, const std::vector<float>& rgb) {
    FILE* f = std::fopen(path, "wb");
    if (!f) { std::perror("fopen"); std::exit(1); }
    std::fprintf(f, "P6\n%d %d\n255\n", w, h);
    auto clamp01 = [](float x){ return x < 0.f ? 0.f : (x > 1.f ? 1.f : x); };
    for (int i = 0; i < w*h; ++i) {
        unsigned char r = (unsigned char)std::lround(clamp01(rgb[3*i+0]) * 255.f);
        unsigned char g = (unsigned char)std::lround(clamp01(rgb[3*i+1]) * 255.f);
        unsigned char b = (unsigned char)std::lround(clamp01(rgb[3*i+2]) * 255.f);
        std::fputc(r, f); std::fputc(g, f); std::fputc(b, f);
    }
    std::fclose(f);
}