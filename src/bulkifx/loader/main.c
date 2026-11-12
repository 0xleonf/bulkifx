#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "bulkifx/loader/loader.h"

// Contoh fungsi dari custom library C Anda
void proses_oleh_library_custom(const uint8_t* data, int width, int height, int stride) {
    // Lakukan pemrosesan gambar di custom library C Anda
    printf("Memproses gambar ukuran %dx%d dengan stride %d\n", width, height, stride);
}

int main() {
    uint8_t* img_data = NULL;
    int width, height, stride, channels;

    // 1. Load pakai OpenCV (melalui wrapper C++)
    if (!load_image_cv("whh.jpg", &img_data, &width, &height, &stride, &channels)) {
        printf("Gagal membaca gambar!\n");
        return -1;
    }

    // 2. Serahkan ke custom library C Anda
    proses_oleh_library_custom(img_data, width, height, stride);

    // 3. Bersihkan memori
    free_image_data(img_data);

    return 0;
}
