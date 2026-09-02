#include "../include/core.h"
#include <check.h>
#include <stdint.h>
#include <stdlib.h>

START_TEST(test_convo_horizontal) {
  Image *input = malloc(sizeof(Image));
  input->width = 4;
  input->height = 4;
  input->R = malloc(4 * 4 * sizeof(int8_t));

  int8_t _input[4 * 4] = {0, 0, 0, 0, 0, 1, 2, 0, 0, 3, 4, 0, 0, 0, 0, 0};
  for (int i = 0; i < (4 * 4); i++) {
    input->R[i] = _input[i];
  }

  int8_t expected_pixel[4 * 2] = {0, 0, 2, -1, 4, -3, 0, 0};
  Matrix *filter = malloc(sizeof(Matrix));
  filter->pixel = malloc(3 * sizeof(int8_t));
  int8_t filter_raw[3] = {-1, 0, 1};

  for (int i = 0; i < 3; i++) {
    filter->pixel[i] = filter_raw[i];
  }

  Matrix *output = convolute_horizontal(input, input->R, filter->pixel, 3);

  ck_assert_ptr_nonnull(output);
  ck_assert_ptr_nonnull(output->pixel);

  for (int i = 0; i < (2 * 4); i++) {
    ck_assert_uint_eq(output->pixel[i], expected_pixel[i]);
  }
}
END_TEST

START_TEST(test_convo_vertical) {
  Image *input = malloc(sizeof(Image));
  input->width = 4;
  input->height = 4;
  input->R = malloc(4 * 4 * sizeof(int8_t));

  int8_t _input[4 * 4] = {0, 0, 0, 0, 0, 1, 2, 0, 0, 3, 4, 0, 0, 0, 0, 0};
  for (int i = 0; i < (4 * 4); i++) {
    input->R[i] = _input[i];
  }

  int8_t expected_pixel[4 * 2] = {0, 5, 8, 0, 0, 7, 10, 0};
  Matrix *filter = malloc(sizeof(Matrix));
  filter->pixel = malloc(3 * sizeof(int8_t));
  int8_t filter_raw[3] = {1, 2, 1};

  for (int i = 0; i < 3; i++) {
    filter->pixel[i] = filter_raw[i];
  }

  Matrix *output = convolute_vertical(input, input->R, filter->pixel, 3);

  ck_assert_ptr_nonnull(output);
  ck_assert_ptr_nonnull(output->pixel);

  for (int i = 0; i < (2 * 4); i++) {
    ck_assert_uint_eq(output->pixel[i], expected_pixel[i]);
  }
}
END_TEST

START_TEST(test_full_convo) {
  Image *input = malloc(sizeof(Image));
  input->width = 4;
  input->height = 4;
  input->R = malloc(4 * 4 * sizeof(int8_t));

  int8_t _input[4 * 4] = {0, 0, 0, 0, 0, 1, 2, 0, 0, 3, 4, 0, 0, 0, 0, 0};
  for (int i = 0; i < (4 * 4); i++) {
    input->R[i] = _input[i];
  }

  int8_t expected_pixel[4 * 2] = {0, 5, 8, 0, 0, 7, 10, 0};
  Matrix *filter = malloc(sizeof(Matrix));
  filter->pixel = malloc(3 * sizeof(int8_t));
  int8_t filter_raw[3] = {1, 2, 1};

  for (int i = 0; i < 3; i++) {
    filter->pixel[i] = filter_raw[i];
  }

  Matrix *output = convolute_vertical(input, input->R, filter->pixel, 3);

  ck_assert_ptr_nonnull(output);
  ck_assert_ptr_nonnull(output->pixel);

  for (int i = 0; i < (2 * 4); i++) {
    ck_assert_uint_eq(output->pixel[i], expected_pixel[i]);
  }
}
END_TEST

Suite *padding_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Convolution Horizontal");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_convo_horizontal);
  tcase_add_test(tc_core, test_convo_vertical);
  suite_add_tcase(s, tc_core);

  return s;
}

int main() {
  int total_failed;
  Suite *s;
  SRunner *sr;

  s = padding_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_VERBOSE);
  total_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (total_failed == 0) ? 0 : 1;
}
