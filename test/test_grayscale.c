#include "../include/bulkifx/grayscale/grayscale.h"
#include <check.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define RGBA_LEN 4

#define EXPECTED_GRAY(r, g, b) ((uint8_t)((19595 * (r) + 38470 * (g) + 7471 * (b)) >> 16))

static uint8_t *make_pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  uint8_t *px = malloc(RGBA_LEN);
  ck_assert_ptr_nonnull(px);
  px[0] = r;
  px[1] = g;
  px[2] = b;
  px[3] = a;
  return px;
}

START_TEST(test_black_pixel) {
  uint8_t *in = make_pixel(0, 0, 0, 255);
  uint8_t out[RGBA_LEN] = {9, 9, 9, 9};

  c_grayscale(in, out, 1, 1);

  ck_assert_uint_eq(out[0], 0);
  ck_assert_uint_eq(out[1], 0);
  ck_assert_uint_eq(out[2], 0);
  ck_assert_uint_eq(out[3], 255);
  free(in);
}
END_TEST

START_TEST(test_white_pixel) {
  uint8_t *in = make_pixel(255, 255, 255, 255);
  uint8_t out[RGBA_LEN] = {9, 9, 9, 9};

  c_grayscale(in, out, 1, 1);

  ck_assert_uint_eq(out[0], 255);
  ck_assert_uint_eq(out[1], 255);
  ck_assert_uint_eq(out[2], 255);
  ck_assert_uint_eq(out[3], 255);
  free(in);
}
END_TEST

START_TEST(test_primary_red) {
  uint8_t *in = make_pixel(255, 0, 0, 255);
  uint8_t out[RGBA_LEN];

  c_grayscale(in, out, 1, 1);

  ck_assert_uint_eq(out[0], EXPECTED_GRAY(255, 0, 0)); /* 76 */
  ck_assert_uint_eq(out[1], 76);
  ck_assert_uint_eq(out[2], 76);
  free(in);
}
END_TEST

START_TEST(test_primary_green) {
  uint8_t *in = make_pixel(0, 255, 0, 255);
  uint8_t out[RGBA_LEN];

  c_grayscale(in, out, 1, 1);

  ck_assert_uint_eq(out[0], EXPECTED_GRAY(0, 255, 0)); /* 149 */
  ck_assert_uint_eq(out[1], 149);
  ck_assert_uint_eq(out[2], 149);
  free(in);
}
END_TEST

START_TEST(test_primary_blue) {
  uint8_t *in = make_pixel(0, 0, 255, 255);
  uint8_t out[RGBA_LEN];

  c_grayscale(in, out, 1, 1);

  ck_assert_uint_eq(out[0], EXPECTED_GRAY(0, 0, 255)); /* 29 */
  ck_assert_uint_eq(out[1], 29);
  ck_assert_uint_eq(out[2], 29);
  free(in);
}
END_TEST

START_TEST(test_mixed_pixel) {
  uint8_t *in = make_pixel(128, 64, 32, 255);
  uint8_t out[RGBA_LEN];

  c_grayscale(in, out, 1, 1);

  ck_assert_uint_eq(out[0], EXPECTED_GRAY(128, 64, 32)); /* 79 */
  ck_assert_uint_eq(out[1], 79);
  ck_assert_uint_eq(out[2], 79);
  free(in);
}
END_TEST

START_TEST(test_alpha_preserved) {
  uint8_t *in = make_pixel(200, 100, 50, 128);
  uint8_t out[RGBA_LEN];

  c_grayscale(in, out, 1, 1);

  ck_assert_uint_eq(out[3], 128);
  ck_assert_uint_eq(out[0], out[1]);
  ck_assert_uint_eq(out[1], out[2]);
  free(in);
}
END_TEST

START_TEST(test_input_not_modified) {
  uint8_t *in = make_pixel(11, 22, 33, 44);
  uint8_t in_copy[RGBA_LEN];
  memcpy(in_copy, in, RGBA_LEN);
  uint8_t out[RGBA_LEN];

  c_grayscale(in, out, 1, 1);

  ck_assert_mem_eq(in, in_copy, RGBA_LEN);
  free(in);
}
END_TEST

START_TEST(test_multi_row_indexing) {
  /* 2x3 image: verify every pixel, catches indexing bugs between rows */
  const int w = 2, h = 3;
  const int total = w * h * RGBA_LEN;
  uint8_t in[24] = {
      0,   0,   0,   255, /* row 0: black  */
      255, 255, 255, 255, /* row 0: white  */
      255, 0,   0,   255, /* row 1: red    */
      0,   255, 0,   255, /* row 1: green  */
      0,   0,   255, 255, /* row 2: blue   */
      128, 64,  32,  200, /* row 2: mixed  */
  };
  uint8_t out[24];

  c_grayscale(in, out, w, h);

  for (int i = 0; i < total; i += RGBA_LEN) {
    uint8_t expected = EXPECTED_GRAY(in[i], in[i + 1], in[i + 2]);
    ck_assert_uint_eq(out[i], expected);
    ck_assert_uint_eq(out[i + 1], expected);
    ck_assert_uint_eq(out[i + 2], expected);
    ck_assert_uint_eq(out[i + 3], in[i + 3]);
  }
}
END_TEST

START_TEST(test_output_written_completely) {
  /* output buffer prefilled with sentinel, must be fully overwritten */
  const int w = 3, h = 2;
  const int total = w * h * RGBA_LEN;
  uint8_t *in = malloc(total);
  uint8_t *out = malloc(total);
  ck_assert_ptr_nonnull(in);
  ck_assert_ptr_nonnull(out);
  for (int i = 0; i < total; i += RGBA_LEN) {
    in[i] = (uint8_t)(i % 256);
    in[i + 1] = (uint8_t)(i / 2);
    in[i + 2] = (uint8_t)(i / 3);
    in[i + 3] = (uint8_t)(i / 4);
    out[i] = out[i + 1] = out[i + 2] = out[i + 3] = 0xEE;
  }

  c_grayscale(in, out, w, h);

  for (int i = 0; i < total; i += RGBA_LEN) {
    uint8_t expected = EXPECTED_GRAY(in[i], in[i + 1], in[i + 2]);
    ck_assert_uint_eq(out[i], expected);
    ck_assert_uint_eq(out[i + 1], expected);
    ck_assert_uint_eq(out[i + 2], expected);
    ck_assert_uint_eq(out[i + 3], in[i + 3]);
  }
  free(in);
  free(out);
}
END_TEST

Suite *grayscale_suite(void) {
  Suite *s = suite_create("grayscale");
  TCase *tc = tcase_create("core");

  tcase_add_checked_fixture(tc, NULL, NULL);
  tcase_add_test(tc, test_black_pixel);
  tcase_add_test(tc, test_white_pixel);
  tcase_add_test(tc, test_primary_red);
  tcase_add_test(tc, test_primary_green);
  tcase_add_test(tc, test_primary_blue);
  tcase_add_test(tc, test_mixed_pixel);
  tcase_add_test(tc, test_alpha_preserved);
  tcase_add_test(tc, test_input_not_modified);
  tcase_add_test(tc, test_multi_row_indexing);
  tcase_add_test(tc, test_output_written_completely);
  suite_add_tcase(s, tc);

  return s;
}

int main(void) {
  int n_failed;
  Suite *s = grayscale_suite();
  SRunner *sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  n_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (n_failed == 0) ? 0 : 1;
}
