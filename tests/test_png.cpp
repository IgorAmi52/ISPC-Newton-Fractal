#include "image_png.hpp"
#include <vector>
#include <fstream>
#include <cassert>
#include <iostream>

int main() {
    int w = 2, h = 2;
    std::vector<float> rgb = {1,0,0, 0,1,0, 0,0,1, 1,1,1};
    write_png("test.png", w, h, rgb);
    std::ifstream f("test.png", std::ios::binary);
    assert(f.good());
    f.seekg(0, std::ios::end);
    size_t size = f.tellg();
    assert(size > 0);
    std::cout << "test_png passed\n";
    return 0;
}
