#include <algorithm>
#include <vector>
#include <iostream>
#include "config.hpp"
#include "roots.hpp"
#include "host_render.hpp"
#include "image_ppm.hpp"
#include "image_png.hpp"

void write_image(const Params& params, const std::vector<float>& outRGB) {
    std::string outPath = params.outPath;
    if (params.outputFormat == "png") {
        if (outPath.size() < 4 || outPath.substr(outPath.size()-4) != ".png")
            outPath += ".png";
        write_png(outPath.c_str(), params.width, params.height, outRGB);
    } else {
        if (outPath.size() < 4 || outPath.substr(outPath.size()-4) != ".ppm")
            outPath += ".ppm";
        write_ppm(outPath.c_str(), params.width, params.height, outRGB);
    }
    std::cout << "Image written to: " << outPath << std::endl;
}


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

    write_image(params, outRGB);
    return 0;
}