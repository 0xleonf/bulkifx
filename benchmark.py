#!/usr/bin/env python3
"""Benchmark Bulkifx vs Pillow untuk grayscale + output PNG.

Cara pakai:
  python3 benchmark.py [INPUT]            -> suite in-process (best/median/worst)
  python3 benchmark.py bulkifx IN OUT     -> satu kali eksekusi (untuk hyperfine)
  python3 benchmark.py pillow  IN OUT     -> satu kali eksekusi (untuk hyperfine)
"""
import sys
import tempfile
import time
from pathlib import Path

WARMUP = 5
RUNS = 30


def bulkifx_gray(inp: str, out: str):
    from ImageProc import Grayscale
    Grayscale(inp, out)


def pillow_gray(inp: str, out: str):
    from PIL import Image
    Image.open(inp).convert("LA").save(out)


def make_real_png(inp: str) -> str:
    """Buat PNG asli dari input agar decoder setara di kedua library."""
    from PIL import Image
    p = Path(tempfile.gettempdir()) / "bulkifx_bench_input.png"
    Image.open(inp).save(p)
    return str(p)


def timeit(fn, *args):
    for _ in range(WARMUP):
        fn(*args)
    times = []
    for _ in range(RUNS):
        t0 = time.perf_counter()
        fn(*args)
        times.append(time.perf_counter() - t0)
    times.sort()
    best, median, worst = times[0], times[len(times) // 2], times[-1]
    return best * 1000, median * 1000, worst * 1000


def suite(inp: str):
    from PIL import Image
    outdir = Path(tempfile.mkdtemp(prefix="bulkifx_bench_"))
    cases = [
        ("original input", inp),
        ("PNG  re-encode", make_real_png(inp)),
    ]
    print(f"warmup {WARMUP}x, {RUNS} runs per case\n")
    print(f"{'case':<22} {'lib':<9} {'best':>9} {'median':>9} {'worst':>9}")
    for label, inp in cases:
        w, h = Image.open(inp).size
        print(f"-- {label} ({w}x{h})")
        for lib, fn in (("bulkifx", bulkifx_gray), ("pillow", pillow_gray)):
            b, m, wst = timeit(fn, inp, str(outdir / f"out_{lib}.png"))
            print(f"{'':<22} {lib:<9} {b:>7.1f}ms {m:>7.1f}ms {wst:>7.1f}ms")
    print(
        "\nCatatan keadilan:\n"
        "- Bulkifx menulis grayscale RGBA (4 kanal), Pillow 'LA' (2 kanal)\n"
        "  -> Pillow melakukan lebih sedikit kerja encoding PNG.\n"
        "- Decoder JPEG berbeda (Go vs libjpeg), keduanya lossy.\n"
        "- Output ditulis ke disk pada kedua sisi (termasuk fsync OS cache)."
    )


def single_run():
    lib, inp, out = sys.argv[1], sys.argv[2], sys.argv[3]
    fn = bulkifx_gray if lib == "bulkifx" else pillow_gray
    fn(inp, out)


if __name__ == "__main__":
    if len(sys.argv) == 4:
        single_run()
    else:
        inp = sys.argv[1] if len(sys.argv) == 2 else "image.png"
        suite(inp)
