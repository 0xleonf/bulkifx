# Bulkifx

Bulkifx is an image processing library written in C with an easy-to-use Python FFI wrapper (using `ctypes`). It is designed for fast, bulk image manipulations and transformations.

> [!WARNING]
> **Project Status: Early Stage / Tahap Awal**
> Proyek ini masih berada dalam tahap pengembangan awal. Struktur direktori, antarmuka fungsi (API), dan dokumentasi dapat berubah sewaktu-waktu tanpa pemberitahuan sebelumnya.
> 
> *This project is in its early stages of development. The directory structure, APIs, and documentation are subject to change.*

---

## Project Structure (Struktur Proyek)

Berikut adalah struktur direktori dari proyek Bulkifx:

```text
bulkifx/
├── CMakeLists.txt         # Konfigurasi build CMake untuk library C dan executable
├── main.c                 # Driver program utama dalam C (development/testing)
├── main.py                # Contoh script utama penggunaan library via Python
├── pillow_benchmark.py    # Script benchmark pembanding menggunakan Pillow
├── assets/                # Folder penyimpanan aset gambar (opsional)
├── build/                 # Folder hasil kompilasi (output build C)
├── bulkifxApp/            # Wrapper Python FFI menggunakan ctypes
│   ├── __init__.py        # Python package initialization
│   ├── core.py            # Abstraksi fungsi (load, save, convert) untuk Python
│   └── ffi.py             # Definisi tipe struct C dan loading shared library (.so)
├── include/               # Header files C (.h)
│   ├── bulk.h             # Deklarasi fungsi pemrosesan bulk (tahap awal)
│   ├── core.h             # Struktur data Image dan fungsi I/O inti
│   ├── img_proc.h         # Deklarasi fungsi manipulasi citra (seperti grayscale)
│   ├── stb_image.h        # Header-only library untuk membaca gambar (STB)
│   └── stb_image_write.h  # Header-only library untuk menulis gambar (STB)
└── src/                   # Source files C (.c)
    ├── bulk.c             # Implementasi pemrosesan bulk (tahap awal)
    ├── core.c             # Implementasi manipulasi memori dan I/O gambar
    └── img_proc.c         # Implementasi pengolahan gambar (grayscale, dll)
```

---

## Prerequisites (Prasyarat)

Sebelum menjalankan proyek ini, pastikan Anda telah menginstal dependensi berikut:
- **GCC / Clang** (C Compiler)
- **CMake** (minimal versi 3.10)
- **Python 3** (berserta dependensi Python)
- **Pillow** (diperlukan hanya untuk menjalankan benchmark)
- **hyperfine** (diperlukan hanya untuk mengukur benchmark performa)

Untuk memasang `hyperfine` dan `Pillow` di Ubuntu/Debian:
```bash
sudo apt update
sudo apt install hyperfine python3-pillow -y
```

---

## Build & Run (Cara Kompilasi & Menjalankan)

### 1. Kompilasi Shared Library C
Lakukan kompilasi untuk menghasilkan berkas binari `libbulkifx.so` di dalam direktori `build/`:

```bash
# Masuk ke direktori build
cd build

# Lakukan konfigurasi build CMake
cmake ..

# Kompilasi project
make
```

### 2. Jalankan Program Python
Setelah proses kompilasi selesai, kembali ke root direktori dan jalankan contoh program Python:

```bash
# Kembali ke root direktori
cd ..

# Pastikan Anda sudah meletakkan gambar masukan (contoh: spon.jpg) di root direktori
python3 main.py
```
Script di atas akan memuat gambar `spon.jpg`, mengubahnya ke grayscale menggunakan library C, lalu menyimpannya sebagai `gray_output.jpg`.

---

## Benchmarking (Komparasi Performa)

Anda dapat membandingkan performa antara library **Bulkifx** dengan **Pillow** menggunakan tool `hyperfine`:

```bash
hyperfine --warmup 3 'python3 main.py' 'python3 pillow_benchmark.py'
```
