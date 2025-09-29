#ifndef HELPER_ISPC
#define HELPER_ISPC

struct Complex { float re; float im; };
struct NewtonOut { float nre; float nim; float f2; };

inline Complex cmul(Complex a, Complex b) {
    Complex o;
    o.re = a.re*b.re - a.im*b.im;
    o.im = a.re*b.im + a.im*b.re;
    return o;
}

inline Complex cdiv(Complex a, Complex b) {
    float denom = b.re*b.re + b.im*b.im + 1e-20f;
    Complex o;
    o.re = (a.re*b.re + a.im*b.im) / denom;
    o.im = (a.im*b.re - a.re*b.im) / denom;
    return o;
}

inline Complex pow_n(Complex z, uniform int n) {
    Complex p; p.re = 1.f; p.im = 0.f;
    for (int k = 0; k < n; ++k) p = cmul(p, z);
    return p;
}

inline NewtonOut newton_step(Complex z, uniform int n) {
    NewtonOut out;

    Complex zn = pow_n(z, n);

    Complex f; f.re = zn.re - 1.f; f.im = zn.im; // f(z) = z^n - 1
    out.f2 = f.re*f.re + f.im*f.im; // |f(z)|^2

    Complex znm1; // z^(n-1)
    if (n > 1) znm1 = pow_n(z, n-1); else { znm1.re = 1.f; znm1.im = 0.f; } // n=1 -> f'=1

    Complex df; df.re = n * znm1.re; df.im = n * znm1.im; // f'(z) = n*z^(n-1)

    Complex q = cdiv(f, df);               // q = f / f'

    out.nre = z.re - q.re;
    out.nim = z.im - q.im;
    return out;
}

#endif 
