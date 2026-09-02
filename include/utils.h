#ifndef UTILS_H
#define UTILS_H
#include "../include/core.h"

/*
 * @brief
 * Produce sigma value based on radius
 */
float sigma(int radius);

/*
 * @brief
 * Produce gaussian blur value for kernel (separable kernel)
 */
float gauss(int x, int radius);

int clamp(int x, int min, int max);

void transpose(Image *img);

#endif // !UTILS_H
