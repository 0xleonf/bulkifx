#include <opencv2/opencv.hpp>
#include "bulkifx/loader/loader.h"
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

bool load_image_cv(const char* filepath, uint8_t** out_data, int* width, int* height, int* stride, int* channels) {
    cv::Mat img = cv::imread(filepath, cv::IMREAD_COLOR);
    if (img.empty()) {
        return false;
    }

    *width = img.cols;
    *height = img.rows;
    *stride = (int)img.step;
    *channels = img.channels();

    size_t data_size = img.rows * img.step;
    *out_data = (uint8_t*)malloc(data_size);
    memcpy(*out_data, img.data, data_size);

    return true;
}

void free_image_data(uint8_t* data) {
    free(data);
}

#ifdef __cplusplus
}
#endif
