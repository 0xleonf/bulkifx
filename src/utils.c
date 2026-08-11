#include "../include/utils.h"
#include <math.h>
#include <stdint.h>
#define PI 3.14159

float sigma(int radius) { return 0.3f * (radius - 1) + 0.8f; }

float gauss(int x, int radius) {
  float s = sigma(radius);
  float exponent = (x * x) / (2 * (s * s));
  return (1 / (2 * PI * (s * s))) * exp(exponent);
}

int clamp(int x, int min, int max) {
  if (x < min)
    return min;
  if (x > max)
    return max;
  return x;
}
