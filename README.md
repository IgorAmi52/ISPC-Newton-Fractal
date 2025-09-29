# Table of Contents

1. [Overview](#whats-a-newton-fractal-in-one-paragraph)
2. [Highlights](#highlights)
3. [Build](#build)
4. [Run & Parameters](#run)
5. [Implementation](#how-its-implemented-short)
6. [Tests](#tests)
7. [License](#license)

# ISPC‑Newton‑Fractal

Small, fast renderer of **Newton fractals** for $f(z)=z^n-1$, written with **ISPC** (SIMD on CPU) + a tiny task runtime (multi‑core). Output to **PNG** or **PPM**, with an optional **benchmark** mode.

---

## What’s a Newton fractal?

Take Newton’s method in the complex plane:
$z_{k+1} = z_k - \frac{f(z_k)}{f'(z_k)}$
For $f(z)=z^n-1$ there are exactly $n$ roots on the unit circle. For each starting point $z_0$ (i.e., each pixel), iterate until convergence. Color the pixel by **which root** it converged to (hue) and **how fast** it converged (brightness). The boundary between basins of attraction forms a beautiful fractal.

---

## Highlights

* **Two levels of parallelism**

  * **Tasks (multi‑core):** the image is split into row blocks and processed concurrently.
  * **SIMD (ISPC `foreach`):** columns inside a block are vectorized.
* **Configurable**: degree $n$, iterations, image size, complex‑plane bounding box, epsilon.
* **PNG & PPM** output.
* **Benchmark mode** to measure throughput.

---

## Build

**Requirements**

* C++17 compiler, **CMake >= 3.20**
* **ISPC** in your `PATH` (e.g. `ispc --version` works)
* No extra libs for PNG: project vendors `stb_image_write.h`

**Clone & build**

```bash
git clone https://github.com/IgorAmi52/ISPC-Newton-Fractal.git
cd ISPC-Newton-Fractal

# Default build is for x86_64 + AVX2 (most modern Intel/AMD CPUs):
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

> You can override the ISPC target: `-DISPC_TARGET=avx2-i32x8`, `avx512skx-i32x16`, `sse4-i32x4`, `neon-i32x4`, …

---

## Run

**Basic render**

```bash
./build/newton --n 5 --maxIters 60 --width 1200 --height 1200 \
  --xmin -2 --xmax 2 --ymin -2 --ymax 2 \
  --eps 1e-6 --outPath out --format png
```

**PPM output**

```bash
./build/newton --format ppm --outPath out
```

**Benchmark mode**

```bash
./build/newton --benchmark --n 7 --maxIters 80 --width 2048 --height 2048 --outPath bench
```

### Parameters


* `--n <int>`: degree in $z^n-1$ (number of roots & hues)
* `--maxIters <int>`: max Newton iterations per pixel
* `--eps <float>`: convergence threshold on $|f(z)|$
* `--width <int>`: image width
* `--height <int>`: image height
* `--xmin <float>`: left bound of complex plane
* `--xmax <float>`: right bound of complex plane
* `--ymin <float>`: bottom bound of complex plane
* `--ymax <float>`: top bound of complex plane
* `--outPath <path>`: output file name (extension auto-added)
* `--format <png|ppm>`: output format (default: png)
* `--benchmark`: run benchmark mode (prints timing)
* `--benchMinRows <int>`: minimum rows per task for benchmarking (default: 8)
* `--benchMaxRows <int>`: maximum rows per task for benchmarking (default: 128)
* `--benchStepRows <int>`: step size for rows per task in benchmarking (default: 8)

Defaults are sensible; pass only what you want to tweak.

---

## How it’s implemented (short)

* **ISPC kernel** (`ispc/fractal.ispc`)

  * Maps each pixel $(x,y)$ to $z_0$ in the given bounding box.
  * Iterates Newton; stops on `eps` or `iters`.
  * Chooses the nearest of the $n$ precomputed roots; maps `rootIdx` → **hue** and `iterations` → **value** (brightness), converts HSV → RGB.
* **Helpers** (`ispc/helper.ispc`) – tiny complex math + one Newton step.
* **Host (C++)**

  * Root precompute on the unit circle, config parsing, image buffers.
  * Writers: **PPM** and **PNG** (via vendored `stb_image_write.h`).
  * Minimal **ISPC task runtime** (vendor) to enable `launch[]/task` multi‑core.

> Parallelism: **tasks** split rows → scale across cores; **foreach** vectorizes columns → scale within a core.

---

## Tests

If enabled, run:

```bash
ctest --test-dir build --output-on-failure
```

---


## License

MIT License.
