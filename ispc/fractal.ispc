#include "helper.ispc"

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
                   float * uniform outRGB)
{
    foreach (i = 0 ... width * height) {
        int y = i / width; 
        int x = i - y * width;

        Complex z;
        z.re = xmin + ((float)x) * (xmax - xmin) / (float)(width  - 1);
        z.im = ymin + ((float)y) * (ymax - ymin) / (float)(height - 1);

        int   it   = 0;
        float eps2 = eps * eps;

        for (; it < maxIters; ++it) {
            NewtonOut step = newton_step(z, n);
            if (step.f2 < eps2) break;
            z.re = step.nre; 
            z.im = step.nim;
        }

        int   rootIdx = 0;
        float bestD2  = 1e30f;
        for (uniform int j = 0; j < n; ++j) {
            float rr = rootsRe[j], ii = rootsIm[j];
            float dr = z.re - rr,  di = z.im - ii;
            float d2 = dr*dr + di*di;
            if (d2 < bestD2) { bestD2 = d2; rootIdx = j; }
        }

        // HSV -> RGB
        float hue = (float)rootIdx / (float)n;
        float val = 1.0f - (float)it / (float)maxIters;
        float sat = 1.0f;

        float h = hue * 6.0f; 
        int   ci = (int)floor(h); 
        float f  = h - (float)ci;

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

        int base = 3 * i;
        outRGB[base + 0] = clamp(R, 0.0f, 1.0f);
        outRGB[base + 1] = clamp(G, 0.0f, 1.0f);
        outRGB[base + 2] = clamp(B, 0.0f, 1.0f);
    }
}
