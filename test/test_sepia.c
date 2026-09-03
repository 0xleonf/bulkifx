#include "../include/bulkifx/sepia/sepia.h"
#include <check.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define RGBA_LEN 4

#define EXPECTED_R(r, g, b) clamp_u8((r) * 101 + (g) * 197 + (b) * 48, 8)
#define EXPECTED_G(r, g, b) clamp_u8((r) * 89 + (g) * 176 + (b) * 43, 8)
#define EXPECTED_B(r, g, b) clamp_u8((r) * 70 + (g) * 137 + (b) * 34, 8)

static int clamp_full(int v, int shift) {
  int x = v >> shift;
  if (x > 255) return 255;
  if (x < 0) return 0;
  return x;
}
#define clamp_u8(v, s) clamp_full((v), (s))

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

  c_sepia(in, out, 1, 1);

  ck_assert_uint_eq(out[0], 0);
  ck_assert_uint_eq(out[1], 0);
  ck_assert_uint_eq(out[2], 0);
  ck_assert_uint_eq(out[3], 255);
  free(in);
}
END_TEST

START_TEST(test_white_pixel_clamped) {
  /* coefficients sum > 256 for R and G: must clamp, not wrap */
  uint8_t *in = make_pixel(255, 255, 255, 255);
  uint8_t out[RGBA_LEN];

  c_sepia(in, out, 1, 1);

  ck_assert_uint_eq(out[0], 255); /* 346.4 -> clamp */
  ck_assert_uint_eq(out[1], 255); /* 308.5 -> clamp */
  ck_assert_uint_eq(out[2], EXPECTED_B(255, 255, 255)); /* 238 */
  ck_assert_uint_eq(out[3], 255);
  free(in);
}
END_TEST

START_TEST(test_red_pixel) {
  uint8_t *in = make_pixel(255, 0, 0, 255);
  uint8_t out[RGBA_LEN];

  c_sepia(in, out, 1, 1);

  ck_assert_uint_eq(out[0], EXPECTED_R(255, 0, 0)); /* 100 */
  ck_assert_uint_eq(out[1], EXPECTED_G(255, 0, 0)); /* 88 */
  ck_assert_uint_eq(out[2], EXPECTED_B(255, 0, 0)); /* 69 */
  free(in);
}
END_TEST

START_TEST(test_green_pixel) {
  uint8_t *in = make_pixel(0, 255, 0, 255);
  uint8_t out[RGBA_LEN];

  c_sepia(in, out, 1, 1);

  ck_assert_uint_eq(out[0], EXPECTED_R(0, 255, 0)); /* 196 */
  ck_assert_uint_eq(out[1], EXPECTED_G(0, 255, 0)); /* 175 */
  ck_assert_uint_eq(out[2], EXPECTED_B(0, 255, 0)); /* 134 */
  free(in);
}
END_TEST

START_TEST(test_blue_pixel) {
  uint8_t *in = make_pixel(0, 0, 255, 255);
  uint8_t out[RGBA_LEN];

  c_sepia(in, out, 1, 1);

  ck_assert_uint_eq(out[0], EXPECTED_R(0, 0, 255)); /* 48 */
  ck_assert_uint_eq(out[1], EXPECTED_G(0, 0, 255)); /* 43 */
  ck_assert_uint_eq(out[2], EXPECTED_B(0, 0, 255)); /* 34 */
  free(in);
}
END_TEST

START_TEST(test_mixed_pixel) {
  uint8_t *in = make_pixel(128, 64, 32, 255);
  uint8_t out[RGBA_LEN];

  c_sepia(in, out, 1, 1);

  ck_assert_uint_eq(out[0], EXPECTED_R(128, 64, 32)); /* 95 */
  ck_assert_uint_eq(out[1], EXPECTED_G(128, 64, 32)); /* 85 */
  ck_assert_uint_eq(out[2], EXPECTED_B(128, 64, 32)); /* 66 */
  free(in);
}
END_TEST

START_TEST(test_alpha_preserved) {
  uint8_t *in = make_pixel(200, 100, 50, 128);
  uint8_t out[RGBA_LEN];

  c_sepia(in, out, 1, 1);

  ck_assert_uint_eq(out[3], 128);
  free(in);
}
END_TEST

START_TEST(test_all_outputs_in_range) {
  /* brute force corner + mid values: result must never wrap */
  const uint8_t vals[] = {0, 1, 32, 64, 128, 200, 254, 255};
  const int n = sizeof(vals) / sizeof(vals[0]);
  uint8_t in[RGBA_LEN], out[RGBA_LEN];

  for (int vi = 0; vi < n; vi++) {
    for (int vj = 0; vj < n; vj++) {
      for (int vk = 0; vk < n; vk++) {
        in[0] = vals[vi];
        in[1] = vals[vj];
        in[2] = vals[vk];
        in[3] = 255;
        c_sepia(in, out, 1, 1);
        ck_assert_uint_eq(out[0], EXPECTED_R(in[0], in[1], in[2]));
        ck_assert_uint_eq(out[1], EXPECTED_G(in[0], in[1], in[2]));
        ck_assert_uint_eq(out[2], EXPECTED_B(in[0], in[1], in[2]));
        ck_assert_uint_eq(out[3], 255);
      }
    }
  }
}
END_TEST

START_TEST(test_input_not_modified) {
  uint8_t *in = make_pixel(11, 22, 33, 44);
  uint8_t in_copy[RGBA_LEN];
  memcpy(in_copy, in, RGBA_LEN);
  uint8_t out[RGBA_LEN];

  c_sepia(in, out, 1, 1);

  ck_assert_mem_eq(in, in_copy, RGBA_LEN);
  free(in);
}
END_TEST

START_TEST(test_multi_row_indexing) {
  const int w = 2, h = 3;
  const int total = w * h * RGBA_LEN;
  uint8_t in[24] = {
      0,   0,   0,   255,
      255, 255, 255, 255,
      255, 0,   0,   255,
      0,   255, 0,   255,
      0,   0,   255, 255,
      128, 64,  32,  200,
  };
  uint8_t out[24];

  c_sepia(in, out, w, h);

  for (int i = 0; i < total; i += RGBA_LEN) {
    ck_assert_uint_eq(out[i], EXPECTED_R(in[i], in[i + 1], in[i + 2]));
    ck_assert_uint_eq(out[i + 1], EXPECTED_G(in[i], in[i + 1], in[i + 2]));
    ck_assert_uint_eq(out[i + 2], EXPECTED_B(in[i], in[i + 1], in[i + 2]));
    ck_assert_uint_eq(out[i + 3], in[i + 3]);
  }
}
END_TEST

Suite *sepia_suite(void) {
  Suite *s = suite_create("sepia");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_black_pixel);
  tcase_add_test(tc, test_white_pixel_clamped);
  tcase_add_test(tc, test_red_pixel);
  tcase_add_test(tc, test_green_pixel);
  tcase_add_test(tc, test_blue_pixel);
  tcase_add_test(tc, test_mixed_pixel);
  tcase_add_test(tc, test_alpha_preserved);
  tcase_add_test(tc, test_all_outputs_in_range);
  tcase_add_test(tc, test_input_not_modified);
  tcase_add_test(tc, test_multi_row_indexing);
  suite_add_tcase(s, tc);

  return s;
}

int main(void) {
  int n_failed;
  Suite *s = sepia_suite();
  SRunner *sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  n_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (n_failed == 0) ? 0 : 1;
}
