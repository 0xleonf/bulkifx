#pragma once
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

bool load_image_cv(const char* filepath, uint8_t** out_data, int* width, int* height, int* stride, int* channels);
void free_image_data(uint8_t* data);

#ifdef __cplusplus
}
#endif

