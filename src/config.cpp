#include "config.hpp"
#include <cstring>
#include <cstdlib>

Params parse_args(int argc, char* argv[]) {
    Params params;
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--width") == 0 && i+1 < argc)
            params.width = std::atoi(argv[++i]);
        else if (std::strcmp(argv[i], "--height") == 0 && i+1 < argc)
            params.height = std::atoi(argv[++i]);
        else if (std::strcmp(argv[i], "--n") == 0 && i+1 < argc)
            params.n = std::atoi(argv[++i]);
        else if (std::strcmp(argv[i], "--maxIters") == 0 && i+1 < argc)
            params.maxIters = std::atoi(argv[++i]);
        else if (std::strcmp(argv[i], "--eps") == 0 && i+1 < argc)
            params.eps = std::atof(argv[++i]);
        else if (std::strcmp(argv[i], "--xmin") == 0 && i+1 < argc)
            params.xmin = std::atof(argv[++i]);
        else if (std::strcmp(argv[i], "--xmax") == 0 && i+1 < argc)
            params.xmax = std::atof(argv[++i]);
        else if (std::strcmp(argv[i], "--ymin") == 0 && i+1 < argc)
            params.ymin = std::atof(argv[++i]);
        else if (std::strcmp(argv[i], "--ymax") == 0 && i+1 < argc)
            params.ymax = std::atof(argv[++i]);
        else if (std::strcmp(argv[i], "--outPath") == 0 && i+1 < argc)
            params.outPath = argv[++i];
        else if (std::strcmp(argv[i], "--benchmark") == 0)
            params.benchmark = true;
        else if (std::strcmp(argv[i], "--benchMinRows") == 0 && i+1 < argc)
            params.benchMinRows = std::atoi(argv[++i]);
        else if (std::strcmp(argv[i], "--benchMaxRows") == 0 && i+1 < argc)
            params.benchMaxRows = std::atoi(argv[++i]);
        else if (std::strcmp(argv[i], "--benchStepRows") == 0 && i+1 < argc)
            params.benchStepRows = std::atoi(argv[++i]);
    }
    return params;
}
