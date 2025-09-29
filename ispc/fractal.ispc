#include "helper.ispc"

inline float clamp01(float x) { return x < 0.0f ? 0.0f : (x > 1.0f ? 1.0f : x); }

static task void render_rows(uniform int width,
                      uniform int height,
                      uniform float xmin,
                      uniform float xmax,
                      uniform float ymin,
                      uniform float ymax,
                      uniform int n,
                      uniform int maxIters,
                      uniform float eps,
                      uniform float * uniform rootsRe,
                      uniform float * uniform rootsIm,
                      float * uniform outRGB,
                      uniform int rowsPerTask)
{
    // Row range for this task
    uniform int startY = taskIndex * rowsPerTask;
    uniform int endY   = startY + rowsPerTask;
    if (endY > height) endY = height;

    for (uniform int y = startY; y < endY; ++y) {
        float z_im = ymin + (float)y * (ymax - ymin) / (float)(height - 1);

        foreach (x = 0 ... width) {
            float z_re = xmin + (float)x * (xmax - xmin) / (float)(width - 1);

            Complex z; z.re = z_re; z.im = z_im;

            int   it   = 0;
            float eps2 = eps * eps;

            // Newton-Raphson itteration
            for (; it < maxIters; ++it) { 
                NewtonOut step = newton_step(z, n);
                if (step.f2 < eps2) break;
                z.re = step.nre;
                z.im = step.nim;
            }

            // closest root
            int   rootIdx = 0;
            float bestD2  = 1e30f;
            for (uniform int j = 0; j < n; ++j) {
                float dr = z.re - rootsRe[j];
                float di = z.im - rootsIm[j];
                float d2 = dr*dr + di*di;
                if (d2 < bestD2) { bestD2 = d2; rootIdx = j; }
            }

            // Colors (HSV → RGB)
            float hue = (float)rootIdx / (float)n;
            float val = 1.0f - (float)it / (float)maxIters;
            float sat = 1.0f;

            float H = hue * 6.0f;
            int   ci = (int)floor(H);
            float f  = H - (float)ci;

            float p = val*(1.0f - sat);
            float q = val*(1.0f - sat*f);
            float t = val*(1.0f - sat*(1.0f - f));

            float R,G,B;
            if      (ci == 0) { R = val; G = t;   B = p; }
            else if (ci == 1) { R = q;   G = val; B = p; }
            else if (ci == 2) { R = p;   G = val; B = t; }
            else if (ci == 3) { R = p;   G = q;   B = val; }
            else if (ci == 4) { R = t;   G = p;   B = val; }
            else              { R = val; G = p;   B = q; }

            int idx = 3 * (y*width + x);
            outRGB[idx + 0] = clamp01(R);
            outRGB[idx + 1] = clamp01(G);
            outRGB[idx + 2] = clamp01(B);
        }
    }
}

// Initialize tasks: launches multiple tasks (multi-core), each processes a block of rows
export void render(uniform int width,
                   uniform int height,
                   uniform float xmin,
                   uniform float xmax,
                   uniform float ymin,
                   uniform float ymax,
                   uniform int n,
                   uniform int maxIters,
                   uniform float eps,
                   uniform float * uniform rootsRe,
                   uniform float * uniform rootsIm,
                   float * uniform outRGB,
                   uniform int rowsPerTask)
{
    if (rowsPerTask < 1) rowsPerTask = 1;

    uniform int numTasks = (height + rowsPerTask - 1) / rowsPerTask;
    if (numTasks < 1) numTasks = 1;

    launch[numTasks] render_rows(width, height,
                                 xmin, xmax, ymin, ymax,
                                 n, maxIters, eps,
                                 rootsRe, rootsIm,
                                 outRGB,
                                 rowsPerTask);
    sync; // wait for all tasks
}
