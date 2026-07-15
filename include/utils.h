#ifndef UTILS_H
#define UTILS_H

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


#endif // !UTILS_H
