
#include "config.hpp"
#include "roots.hpp"
#include "host_render.hpp"
#include "image_ppm.hpp"
#include <vector>
#include <iostream>

int main(int argc, char* argv[]) {
    Params params = parse_args(argc, argv);

    std::vector<float> rootsRe, rootsIm;
    precompute_roots(params.n, rootsRe, rootsIm);

    if (params.benchmark) {
        benchmark_render(params, rootsRe, rootsIm);
        return 0;
    }

    std::vector<float> outRGB(params.width * params.height * 3, 0.0f);
    render_image(params, rootsRe, rootsIm, outRGB);
    write_ppm(params.outPath.c_str(), params.width, params.height, outRGB);
    std::cout << "Image written to: " << params.outPath << std::endl;
    return 0;
}