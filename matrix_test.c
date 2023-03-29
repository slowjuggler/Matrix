#include "matrix.h"

#include <check.h>
#include <stdio.h>

TCase *tcase_matrix_create_matrix();
TCase *tcase_matrix_remove_matrix();
TCase *tcase_matrix_eq_matrix();
TCase *tcase_matrix_sum_matrix();
TCase *tcase_matrix_sub_matrix();
TCase *tcase_matrix_mult_number();
TCase *tcase_matrix_mult_matrix();
TCase *tcase_matrix_transpose();
TCase *tcase_matrix_calc_complements();
TCase *tcase_matrix_determinant();
TCase *tcase_matrix_inverse_matrix();

#define suite_add_tcase_list(suite)                                \
  do {                                                             \
    suite_add_tcase((suite), tcase_matrix_create_matrix());    \
    suite_add_tcase((suite), tcase_matrix_remove_matrix());    \
    suite_add_tcase((suite), tcase_matrix_eq_matrix());        \
    suite_add_tcase((suite), tcase_matrix_sum_matrix());       \
    suite_add_tcase((suite), tcase_matrix_sub_matrix());       \
    suite_add_tcase((suite), tcase_matrix_mult_number());      \
    suite_add_tcase((suite), tcase_matrix_mult_matrix());      \
    suite_add_tcase((suite), tcase_matrix_transpose());        \
    suite_add_tcase((suite), tcase_matrix_calc_complements()); \
    suite_add_tcase((suite), tcase_matrix_determinant());      \
    suite_add_tcase((suite), tcase_matrix_inverse_matrix());   \
  } while (0)

void gen_matrix(matrix_t *A) {
  double num = 1;
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++, num++) {
      A->matrix[i][j] = num;
    }
  }
}

void anti_gen_matrix(matrix_t *A) {
  double num = 32;
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++, num--) {
      A->matrix[i][j] = num;
    }
  }
}

void fill(matrix_t *matrixStruct, double value) {
  for (int i = 0; i < matrixStruct->rows; i++) {
    for (int j = 0; j < matrixStruct->columns; j++) {
      matrixStruct->matrix[i][j] = value;
    }
  }
}

START_TEST(test_create_matrix_1) {
  matrix_t *matrix = NULL;
  int rows = 10, columns = 3;
  ck_assert_int_eq(create_matrix(rows, columns, matrix), ERROR);
}
END_TEST

START_TEST(test_create_matrix_2) {
  matrix_t matrix = {0};
  int rows = 5, columns = 5;
  ck_assert_int_eq(create_matrix(rows, columns, &matrix), OK);
  for (int i = 0; i < rows; i++) {
    free(matrix.matrix[i]);
  }
  free(matrix.matrix);
}
END_TEST

START_TEST(test_create_matrix_3) {
  matrix_t matrix = {0};
  ck_assert_int_eq(create_matrix(-1, -20, &matrix), ERROR);
}
END_TEST

TCase *tcase_matrix_create_matrix() {
  TCase *tcase = tcase_create("matrix_create_matrix");
  tcase_add_test(tcase, test_create_matrix_1);
  tcase_add_test(tcase, test_create_matrix_2);
  tcase_add_test(tcase, test_create_matrix_3);
  return tcase;
}

START_TEST(test_determinant_1) {
  matrix_t result1;
  double result2;
  create_matrix(3, 3, &result1);
  result1.matrix[0][0] = 1;
  result1.matrix[0][1] = 2;
  result1.matrix[0][2] = 3;

  result1.matrix[1][0] = 4;
  result1.matrix[1][1] = 5;
  result1.matrix[1][2] = 6;

  result1.matrix[2][0] = 7;
  result1.matrix[2][1] = 8;
  result1.matrix[2][2] = 9;
  double expected_result = 0;
  determinant(&result1, &result2);
  ck_assert_ldouble_eq(expected_result, result2);
  remove_matrix(&result1);
}
END_TEST

START_TEST(test_determinant_2) {
  matrix_t result1;
  double result3;
  create_matrix(3, 4, &result1);
  int actual_result = ERROR;
  int expected_result = determinant(&result1, &result3);
  ck_assert_uint_eq(expected_result, actual_result);
  remove_matrix(&result1);
}
END_TEST

START_TEST(test_determinant_3) {
  matrix_t temp;
  double result;
  int result_defolt;
  create_matrix(3, 2, &temp);
  gen_matrix(&temp);
  result_defolt = determinant(&temp, &result);
  ck_assert_int_eq(result_defolt, 2);
  remove_matrix(&temp);
}
END_TEST

START_TEST(test_determinant_4) {
  matrix_t temp;
  double result;
  int result_defolt;
  create_matrix(3, 3, &temp);
  temp.matrix[0][0] = 0;
  temp.matrix[0][1] = 9;
  temp.matrix[0][2] = 5;

  temp.matrix[1][0] = 4;
  temp.matrix[1][1] = 3;
  temp.matrix[1][2] = -5;

  temp.matrix[2][0] = -1;
  temp.matrix[2][1] = 6;
  temp.matrix[2][2] = -4;
  result_defolt = determinant(&temp, &result);
  ck_assert_int_eq(result_defolt, 0);
  ck_assert_uint_eq(result, 324);
  remove_matrix(&temp);
}
END_TEST

START_TEST(test_determinant_5) {
  matrix_t temp;
  double result;
  int result_defolt;
  create_matrix(3, 3, &temp);
  temp.matrix[0][0] = 1;
  temp.matrix[1][1] = 1;
  temp.matrix[2][2] = 1;

  result_defolt = determinant(&temp, &result);
  ck_assert_int_eq(result_defolt, 0);
  ck_assert_uint_eq(result, 1);
  remove_matrix(&temp);
}
END_TEST

TCase *tcase_matrix_determinant() {
  TCase *tcase = tcase_create("matrix_determinant");
  tcase_add_test(tcase, test_determinant_1);
  tcase_add_test(tcase, test_determinant_2);
  tcase_add_test(tcase, test_determinant_3);
  tcase_add_test(tcase, test_determinant_4);
  tcase_add_test(tcase, test_determinant_5);
  return tcase;
}

START_TEST(test_eq_matrix_1) {
  matrix_t matrix_1 = {0};
  matrix_t matrix_2 = {0};
  matrix_1.rows = 2, matrix_1.columns = 3;
  matrix_2.rows = 3, matrix_2.columns = 3;
  create_matrix(matrix_1.rows, matrix_1.columns, &matrix_1);
  create_matrix(matrix_2.rows, matrix_2.columns, &matrix_2);
  ck_assert_int_eq(eq_matrix(&matrix_1, &matrix_2), ERROR);
  remove_matrix(&matrix_1);
  remove_matrix(&matrix_2);
}
END_TEST

START_TEST(test_eq_matrix_2) {
  matrix_t matrix_1 = {0};
  matrix_t matrix_2 = {0};
  matrix_1.rows = 3, matrix_1.columns = 7;
  matrix_2.rows = 3, matrix_2.columns = 7;
  create_matrix(matrix_1.rows, matrix_1.columns, &matrix_1);
  create_matrix(matrix_2.rows, matrix_2.columns, &matrix_2);
  matrix_1.matrix[1][2] = 4.0000007;
  matrix_2.matrix[1][2] = 4.0000009;
  ck_assert_int_eq(eq_matrix(&matrix_1, &matrix_2), FAILURE);
  remove_matrix(&matrix_1);
  remove_matrix(&matrix_2);
}
END_TEST

TCase *tcase_matrix_eq_matrix() {
  TCase *tcase = tcase_create("matrix_eq_matrix");
  tcase_add_test(tcase, test_eq_matrix_1);
  tcase_add_test(tcase, test_eq_matrix_2);
  return tcase;
}

START_TEST(test_inverse_matrix_1) {
  matrix_t temp, result_defolt, result_defolt_2;
  create_matrix(3, 3, &temp);
  create_matrix(3, 3, &result_defolt);
  temp.matrix[0][0] = 2;
  temp.matrix[0][1] = 5;
  temp.matrix[0][2] = 7;
  temp.matrix[1][0] = 6;
  temp.matrix[1][1] = 3;
  temp.matrix[1][2] = 4;
  temp.matrix[2][0] = 5;
  temp.matrix[2][1] = -2;
  temp.matrix[2][2] = -3;
  result_defolt.matrix[0][0] = 1;
  result_defolt.matrix[0][1] = -1;
  result_defolt.matrix[0][2] = 1;
  result_defolt.matrix[1][0] = -38;
  result_defolt.matrix[1][1] = 41;
  result_defolt.matrix[1][2] = -34;
  result_defolt.matrix[2][0] = 27;
  result_defolt.matrix[2][1] = -29;
  result_defolt.matrix[2][2] = 24;

  inverse_matrix(&temp, &result_defolt_2);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_uint_eq(result_defolt.matrix[i][j],
                        result_defolt_2.matrix[i][j]);
    }
  }
  remove_matrix(&temp);
  remove_matrix(&result_defolt);
  remove_matrix(&result_defolt_2);
}
END_TEST

START_TEST(test_inverse_matrix_2) {
  matrix_t result1;
  matrix_t result2;
  matrix_t result3;
  create_matrix(3, 3, &result1);
  result1.matrix[0][0] = 2;
  result1.matrix[0][1] = 5;
  result1.matrix[0][2] = 7;

  result1.matrix[1][0] = 6;
  result1.matrix[1][1] = 3;
  result1.matrix[1][2] = 4;

  result1.matrix[2][0] = 5;
  result1.matrix[2][1] = -2;
  result1.matrix[2][2] = -3;

  int expected_result = create_matrix(3, 3, &result2);
  result2.matrix[0][0] = 1;
  result2.matrix[0][1] = -1;
  result2.matrix[0][2] = 1;
  result2.matrix[1][0] = -38;
  result2.matrix[1][1] = 41;
  result2.matrix[1][2] = -34;
  result2.matrix[2][0] = 27;
  result2.matrix[2][1] = -29;
  result2.matrix[2][2] = 24;
  int actual_result = inverse_matrix(&result1, &result3);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_double_eq_tol(result2.matrix[i][j], result3.matrix[i][j], 1e-6);
    }
  }
  ck_assert_uint_eq(expected_result, actual_result);
  remove_matrix(&result3);
  remove_matrix(&result2);
  remove_matrix(&result1);
}
END_TEST

START_TEST(test_inverse_matrix_3) {
  matrix_t result1;
  matrix_t result3;
  create_matrix(3, 3, &result1);
  result1.matrix[0][0] = 1;
  result1.matrix[0][1] = 2;
  result1.matrix[0][2] = 3;

  result1.matrix[1][0] = 4;
  result1.matrix[1][1] = 5;
  result1.matrix[1][2] = 6;

  result1.matrix[2][0] = 7;
  result1.matrix[2][1] = 8;
  result1.matrix[2][2] = 9;
  int expected_result = ERROR;
  int actual_result = inverse_matrix(&result1, &result3);
  ck_assert_uint_eq(expected_result, actual_result);

  remove_matrix(&result1);
}
END_TEST

TCase *tcase_matrix_inverse_matrix() {
  TCase *tcase = tcase_create("matrix_21_inverse_matrix");
  tcase_add_test(tcase, test_inverse_matrix_1);
  tcase_add_test(tcase, test_inverse_matrix_2);
  tcase_add_test(tcase, test_inverse_matrix_3);
  return tcase;
}

START_TEST(test_mult_matrix_1) {
  matrix_t result1;
  matrix_t result2;
  matrix_t result3;
  matrix_t result4;
  create_matrix(4, 4, &result1);
  create_matrix(4, 4, &result2);
  fill(&result1, 3);
  fill(&result2, 3);
  int expected_result = create_matrix(4, 4, &result3);
  fill(&result3, 36);
  int actual_result = mult_matrix(&result1, &result2, &result4);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_double_eq(result3.matrix[i][j], result4.matrix[i][j]);
    }
  }
  ck_assert_uint_eq(expected_result, actual_result);
  remove_matrix(&result4);
  remove_matrix(&result3);
  remove_matrix(&result2);
  remove_matrix(&result1);
}
END_TEST

START_TEST(test_mult_matrix_2) {
  matrix_t temp;
  matrix_t temp2;
  matrix_t result;
  matrix_t result_defolt;
  create_matrix(3, 2, &temp);
  create_matrix(2, 3, &temp2);
  create_matrix(3, 3, &result_defolt);
  gen_matrix(&temp);
  gen_matrix(&temp2);
  result_defolt.matrix[0][0] = 9;
  result_defolt.matrix[0][1] = 12;
  result_defolt.matrix[0][2] = 15;
  result_defolt.matrix[1][0] = 19;
  result_defolt.matrix[1][1] = 26;
  result_defolt.matrix[1][2] = 33;
  result_defolt.matrix[2][0] = 29;
  result_defolt.matrix[2][1] = 40;
  result_defolt.matrix[2][2] = 51;
  ck_assert_int_eq(mult_matrix(&temp, &temp2, &result), 0);
  ck_assert_int_eq(eq_matrix(&result_defolt, &result), 1);
  remove_matrix(&temp);
  remove_matrix(&temp2);
  remove_matrix(&result);
  remove_matrix(&result_defolt);
}
END_TEST

START_TEST(test_mult_matrix_3) {
  matrix_t temp;
  matrix_t temp2;
  matrix_t result;
  create_matrix(3, 3, &temp);
  create_matrix(3, 3, &temp2);
  gen_matrix(&temp);
  gen_matrix(&temp2);
  ck_assert_int_eq(mult_matrix(&temp, &temp2, &result), 0);
  remove_matrix(&temp);
  remove_matrix(&temp2);
  remove_matrix(&result);
}
END_TEST

START_TEST(test_mult_matrix_4) {
  matrix_t temp;
  matrix_t temp2;
  matrix_t result;
  matrix_t result_defolt;
  create_matrix(2, 3, &temp);
  create_matrix(3, 2, &temp2);
  create_matrix(2, 2, &result_defolt);
  gen_matrix(&temp);
  gen_matrix(&temp2);
  result_defolt.matrix[0][0] = 22;
  result_defolt.matrix[0][1] = 28;
  result_defolt.matrix[1][0] = 49;
  result_defolt.matrix[1][1] = 64;
  ck_assert_int_eq(mult_matrix(&temp, &temp2, &result), 0);
  ck_assert_int_eq(eq_matrix(&result_defolt, &result), 1);
  remove_matrix(&temp);
  remove_matrix(&temp2);
  remove_matrix(&result);
  remove_matrix(&result_defolt);
}
END_TEST

TCase *tcase_matrix_mult_matrix() {
  TCase *tcase = tcase_create("matrix_mult_matrix");
  tcase_add_test(tcase, test_mult_matrix_1);
  tcase_add_test(tcase, test_mult_matrix_2);
  tcase_add_test(tcase, test_mult_matrix_3);
  tcase_add_test(tcase, test_mult_matrix_4);
  return tcase;
}

START_TEST(test_mult_number_1) {
  matrix_t temp;
  matrix_t temp2;
  matrix_t result;
  matrix_t result_defolt;
  double num = 777.777;
  create_matrix(3, 3, &temp);
  create_matrix(3, 3, &temp2);
  create_matrix(3, 3, &result_defolt);
  gen_matrix(&temp);
  gen_matrix(&temp2);
  for (int i = 0; (i < temp.rows); i++) {
    for (int j = 0; j < temp.columns; j++) {
      result_defolt.matrix[i][j] = temp.matrix[i][j] * 777.777;
    }
  }
  ck_assert_int_eq(mult_number(&temp, num, &result), 0);
  ck_assert_int_eq(eq_matrix(&result_defolt, &result), 1);
  remove_matrix(&temp);
  remove_matrix(&temp2);
  remove_matrix(&result);
  remove_matrix(&result_defolt);
}
END_TEST

START_TEST(test_mult_number_2) {
  matrix_t temp;
  matrix_t temp2;
  matrix_t result;
  matrix_t result_defolt;
  double num = 777.777;
  create_matrix(3, 3, &temp);
  create_matrix(3, 3, &temp2);
  create_matrix(3, 3, &result_defolt);
  gen_matrix(&temp);
  gen_matrix(&temp2);
  for (int i = 0; i < temp.rows; i++) {
    for (int j = 0; j < temp.columns; j++) {
      result_defolt.matrix[i][j] = temp.matrix[i][j] * 777.777;
    }
  }
  ck_assert_int_eq(mult_number(&temp, num, &result), 0);
  ck_assert_int_eq(eq_matrix(&result_defolt, &result), 1);
  remove_matrix(&temp);
  remove_matrix(&temp2);
  remove_matrix(&result);
  remove_matrix(&result_defolt);
}
END_TEST

TCase *tcase_matrix_mult_number() {
  TCase *tcase = tcase_create("matrix_mult_number");
  tcase_add_test(tcase, test_mult_number_1);
  tcase_add_test(tcase, test_mult_number_2);
  return tcase;
}

START_TEST(test_remove_matrix_1) {
  matrix_t matrix = {0};
  double **pmatrix = matrix.matrix;
  int rows = 3, columns = 7;
  create_matrix(rows, columns, &matrix);
  remove_matrix(&matrix);
  ck_assert_ptr_null(pmatrix);
}
END_TEST

TCase *tcase_matrix_remove_matrix() {
  TCase *tcase = tcase_create("matrix_remove_matrix");
  tcase_add_test(tcase, test_remove_matrix_1);
  return tcase;
}

START_TEST(test_sum_matrix_1) {
  matrix_t temp;
  matrix_t temp2;
  matrix_t result;
  matrix_t result_defolt;
  create_matrix(3, 3, &temp);
  create_matrix(3, 3, &temp2);
  create_matrix(3, 3, &result_defolt);
  gen_matrix(&temp);
  anti_gen_matrix(&temp2);
  for (int i = 0; (i < temp.rows); i++) {
    for (int j = 0; j < temp.columns; j++) {
      result_defolt.matrix[i][j] = temp.matrix[i][j] + temp2.matrix[i][j];
    }
  }
  ck_assert_int_eq(sum_matrix(&temp, &temp2, &result), 0);
  ck_assert_int_eq(eq_matrix(&result_defolt, &result), 1);
  remove_matrix(&temp);
  remove_matrix(&temp2);
  remove_matrix(&result);
  remove_matrix(&result_defolt);
}
END_TEST

START_TEST(test_sum_matrix_2) {
  matrix_t temp;
  matrix_t temp2;
  matrix_t result;
  create_matrix(3, 3, &temp);
  create_matrix(3, 3, &temp2);
  gen_matrix(&temp);
  gen_matrix(&temp2);
  ck_assert_int_eq(sum_matrix(&temp, &temp2, &result), 0);
  remove_matrix(&temp);
  remove_matrix(&temp2);
  remove_matrix(&result);
}
END_TEST

START_TEST(test_sum_matrix_3) {
  matrix_t temp;
  matrix_t temp2;
  matrix_t result;
  create_matrix(3, 1, &temp);
  create_matrix(3, 3, &temp2);
  gen_matrix(&temp);
  gen_matrix(&temp2);
  ck_assert_int_eq(sum_matrix(&temp, &temp2, &result), ERROR);
  remove_matrix(&temp);
  remove_matrix(&temp2);
}
END_TEST

TCase *tcase_matrix_sum_matrix() {
  TCase *tcase = tcase_create("matrix_sum_matrix");
  tcase_add_test(tcase, test_sum_matrix_1);
  tcase_add_test(tcase, test_sum_matrix_2);
  tcase_add_test(tcase, test_sum_matrix_3);
  return tcase;
}

START_TEST(test_transpose_1) {
  matrix_t temp;
  matrix_t result;
  matrix_t result_defolt;
  create_matrix(3, 2, &temp);
  create_matrix(2, 3, &result_defolt);
  gen_matrix(&temp);
  result_defolt.matrix[0][0] = 1;
  result_defolt.matrix[0][1] = 3;
  result_defolt.matrix[0][2] = 5;
  result_defolt.matrix[1][0] = 2;
  result_defolt.matrix[1][1] = 4;
  result_defolt.matrix[1][2] = 6;
  ck_assert_int_eq(transpose(&temp, &result), 0);
  ck_assert_int_eq(eq_matrix(&result_defolt, &result), 1);
  remove_matrix(&temp);
  remove_matrix(&result);
  remove_matrix(&result_defolt);
}
END_TEST

TCase *tcase_matrix_transpose() {
  TCase *tcase = tcase_create("matrix_transpose");
  tcase_add_test(tcase, test_transpose_1);
  return tcase;
}

START_TEST(test_sub_matrix_1) {
  matrix_t temp;
  matrix_t temp2;
  matrix_t result;
  matrix_t result_defolt;
  create_matrix(3, 3, &temp);
  create_matrix(3, 3, &temp2);
  create_matrix(3, 3, &result_defolt);
  gen_matrix(&temp);
  anti_gen_matrix(&temp2);
  for (int i = 0; (i < temp.rows); i++) {
    for (int j = 0; j < temp.columns; j++) {
      result_defolt.matrix[i][j] = temp.matrix[i][j] - temp2.matrix[i][j];
    }
  }
  ck_assert_int_eq(sub_matrix(&temp, &temp2, &result), OK);
  ck_assert_int_eq(eq_matrix(&result_defolt, &result), FAILURE);
  remove_matrix(&temp);
  remove_matrix(&temp2);
  remove_matrix(&result);
  remove_matrix(&result_defolt);
}
END_TEST

START_TEST(test_sub_matrix_2) {
  matrix_t temp;
  matrix_t temp2;
  matrix_t result;
  create_matrix(3, 3, &temp);
  create_matrix(3, 3, &temp2);
  gen_matrix(&temp);
  gen_matrix(&temp2);
  ck_assert_int_eq(sub_matrix(&temp, &temp2, &result), 0);
  remove_matrix(&temp);
  remove_matrix(&temp2);
  remove_matrix(&result);
}
END_TEST

START_TEST(test_sub_matrix_3) {
  matrix_t temp;
  matrix_t temp2;
  matrix_t result;
  create_matrix(3, 1, &temp);
  create_matrix(3, 3, &temp2);
  gen_matrix(&temp);
  gen_matrix(&temp2);
  ck_assert_int_eq(sub_matrix(&temp, &temp2, &result), ERROR);
  remove_matrix(&temp);
  remove_matrix(&temp2);
}
END_TEST

TCase *tcase_matrix_sub_matrix() {
  TCase *tcase = tcase_create("matrix_sub_matrix");
  tcase_add_test(tcase, test_sub_matrix_1);
  tcase_add_test(tcase, test_sub_matrix_2);
  tcase_add_test(tcase, test_sub_matrix_3);
  return tcase;
}

START_TEST(test_calc_complements_1) {
  matrix_t test, result;
  create_matrix(3, 2, &test);
  int result_status;
  gen_matrix(&test);
  result_status = calc_complements(&test, &result);
  ck_assert_int_eq(result_status, 2);
  remove_matrix(&test);
}
END_TEST

START_TEST(test_calc_complements_2) {
  matrix_t test, result;
  create_matrix(3, 3, &test);
  gen_matrix(&test);
  int result_status = calc_complements(&test, &result);
  ck_assert_int_eq(result_status, 0);
  remove_matrix(&test);
  remove_matrix(&result);
}
END_TEST

START_TEST(test_calc_complements_3) {
  matrix_t test, test_2, result;
  create_matrix(3, 3, &test);
  test.matrix[0][0] = 1;
  test.matrix[0][1] = 2;
  test.matrix[0][2] = 3;
  test.matrix[1][0] = 0;
  test.matrix[1][1] = 4;
  test.matrix[1][2] = 2;
  test.matrix[2][0] = 5;
  test.matrix[2][1] = 2;
  test.matrix[2][2] = 1;
  calc_complements(&test, &result);
  create_matrix(3, 3, &test_2);
  test_2.matrix[0][0] = 0;
  test_2.matrix[0][1] = 10;
  test_2.matrix[0][2] = -20;
  test_2.matrix[1][0] = 4;
  test_2.matrix[1][1] = -14;
  test_2.matrix[1][2] = 8;
  test_2.matrix[2][0] = -8;
  test_2.matrix[2][1] = -2;
  test_2.matrix[2][2] = 4;
  for (int i = 0; i < result.rows; i++) {
    for (int j = 0; j < result.columns; j++) {
      ck_assert_uint_eq(result.matrix[i][j], test_2.matrix[i][j]);
    }
  }
  remove_matrix(&test);
  remove_matrix(&test_2);
  remove_matrix(&result);
}
END_TEST

TCase *tcase_matrix_calc_complements() {
  TCase *tcase = tcase_create("matrix_calc_complements");
  tcase_add_test(tcase, test_calc_complements_1);
  tcase_add_test(tcase, test_calc_complements_2);
  tcase_add_test(tcase, test_calc_complements_3);
  return tcase;
}

int main() {
  Suite *suite = suite_create("matrix");
#ifdef suite_add_tcase_list
  suite_add_tcase_list(suite);
#endif

  SRunner *suite_runner = srunner_create(suite);
#ifdef srunner_add_suite_list
  srunner_add_suite_list(suite_runner);
#endif

  srunner_run_all(suite_runner, CK_NORMAL);
  int failed_count = srunner_ntests_failed(suite_runner);
  srunner_free(suite_runner);
  return (failed_count != 0);
}
