# Bulkifx

Image processing library: **Go** handles image I/O (reading/writing images, automatic memory allocation via GC) while **C** executes the image processing algorithms (grayscale, etc.). A Python wrapper is provided via ctypes.

```text
Python (ctypes) ──> Go (cgo, .so) ──> C (static lib)
  read/write img      algorithm
```

## Installation

Prerequisites:

- GCC/Clang
- CMake >= 3.10
- Go >= 1.27
- Python 3 (ctypes is part of the stdlib, no external dependencies)

## Build

One script for everything (C static lib via CMake + Go c-shared `.so`):

```bash
./build.sh
```

Output: `build/bulkifx.so` and `build/libbulkifx_static.a`.

## Usage

From Python:

```python
from ImageProc import Grayscale

Grayscale("input.png", "gray_output.png")
```

Or run the example:

```bash
python3 main.py input.png output.png
```

> Output is always in PNG format (Go's encoder writes the result).

## Testing

```bash
cmake -S . -B build -DBUILD_TESTING=ON
ctest --test-dir build
```

## Benchmark

Comparing Bulkifx vs Pillow:

```bash
python3 benchmark.py [INPUT]                       # in-process (best/median/worst)
hyperfine --warmup 3 \
  'python3 benchmark.py bulkifx input.png out.png' \
  'python3 benchmark.py pillow input.png out.png'  # end-to-end
```
