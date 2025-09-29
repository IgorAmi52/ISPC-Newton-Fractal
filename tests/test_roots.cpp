#include "roots.hpp"
#include <cassert>
#include <cmath>
#include <iostream>

int main() {
    std::vector<float> re, im;
    precompute_roots(4, re, im);
    for (int i = 0; i < 4; ++i) {
        float mag = std::sqrt(re[i]*re[i] + im[i]*im[i]);
        assert(std::abs(mag - 1.0f) < 1e-5f); // svi koreni na jedinicnoj kruznici
    }
    std::cout << "test_roots passed\n";
    return 0;
}
