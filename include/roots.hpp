#include <vector>

// Roots for function z^n - 1 = 0
// Calculated by changing angle in polar representation
// from 0 to 2*PI in n equal steps
inline void precompute_roots(int n, std::vector<float>& re, std::vector<float>& im) {
    if (n <= 0) { re.clear(); im.clear(); return; }
    re.resize(n); im.resize(n);

    const double PI = 3.141592653589793238462643383279502884;
    const double ang = 2.0 * PI / static_cast<double>(n);

    const double c = std::cos(ang);
    const double s = std::sin(ang);

    double rr = 1.0, ii = 0.0;

    for (int j = 0; j < n; ++j) {
    re[j] = static_cast<float>(rr);
    im[j] = static_cast<float>(ii);

    const double nr = rr * c - ii * s;
    const double ni = rr * s + ii * c;
    rr = nr; ii = ni;
    }
}