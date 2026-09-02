#include "../include/utils.h"
#include <check.h>
#include <stdint.h>
#include <stdlib.h>

START_TEST(test_gauss) {
  int radius = 3 / 2;
  float output[3] = {0};
}
END_TEST

Suite *gauss_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Gauss");
  tc_core = tcase_create("Utils");

  tcase_add_test(tc_core, test_gauss);
  suite_add_tcase(s, tc_core);

  return s;
}

int main() {
  int total_failed;
  Suite *s;
  SRunner *sr;

  s = gauss_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  total_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (total_failed == 0) ? 0 : 1;
}
