#include "config.hpp"
#include <cassert>
#include <iostream>

int main() {
    const char* argv[] = { "prog", "--width", "100", "--height", "200", "--n", "3", "--outPath", "test.ppm" };
    Params p = parse_args(9, const_cast<char**>(argv));
    assert(p.width == 100);
    assert(p.height == 200);
    assert(p.n == 3);
    assert(p.outPath == "test.ppm");
    std::cout << "test_config passed\n";
    return 0;
}
