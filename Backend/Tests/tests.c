#include "../Headers/affinity_transformations.h"
#include "check.h"

Suite *test_for_calc();

START_TEST(viewer_test_1) {
	char* file_name = "./Models/cube.obj";
  int res = kOK;
  Model* model = (Model*)malloc(1*sizeof(Model));
  res = parser(model, file_name);
  ck_assert_int_eq(res, kOK);
  free_model(model);
}

START_TEST(viewer_test_2) {
	char* file_name = "./Models/aboba.obj";
  int res = kFILE_READ_ERROR;
  Model* model = (Model*)malloc(1*sizeof(Model));
  res = parser(model, file_name);
  ck_assert_int_eq(res, kFILE_READ_ERROR);
  free(model);
}

START_TEST(viewer_test_3) {
	char* file_name = "./Models/cube_error.obj";
  int res = kINCORRECT_PEAKS;
  Model* model = (Model*)malloc(1*sizeof(Model));
  res = parser(model, file_name);
  ck_assert_int_eq(res, kINCORRECT_PEAKS);
  free_model(model);
}

START_TEST(affinity_transformations_1) {
	char* file_name = "./Models/lamp.obj";
  int res = kOK;
  Model* model = (Model*)malloc(1*sizeof(Model));
  res = parser(model, file_name);
  ck_assert_int_eq(res, kOK);
  move_to_xyz(model, 0.5, 1.2, 0.1);
  free_model(model);
}

START_TEST(affinity_transformations_2) {
	char* file_name = "./Models/lamp.obj";
  int res = kOK;
  Model* model = (Model*)malloc(1*sizeof(Model));
  res = parser(model, file_name);
  ck_assert_int_eq(res, kOK);
  rotate_model(model, 0.123, 1.81215, 0.33333);
  free_model(model);
}

Suite *test_for_calc() {
  Suite *s;
  TCase *tc_core;
  s = suite_create("Viewer");
  tc_core = tcase_create("Core");
  tcase_add_test(tc_core, viewer_test_1);
  tcase_add_test(tc_core, viewer_test_2);
  tcase_add_test(tc_core, viewer_test_3);
  tcase_add_test(tc_core, affinity_transformations_1);
  tcase_add_test(tc_core, affinity_transformations_2);

  suite_add_tcase(s, tc_core);
  return s;
}
int main() {
  int no_failed = 0;
  SRunner *runner;
  // Forming a "runner" to test different cases
  runner = srunner_create(test_for_calc());
  // Running and counting failed test cases
  srunner_set_fork_status(runner, CK_NOFORK);
  srunner_run_all(runner, CK_ENV);
  no_failed = srunner_ntests_failed(runner);
  srunner_free(runner);
  return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}