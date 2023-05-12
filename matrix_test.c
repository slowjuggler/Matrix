#include "matrix.h"

#include <check.h>

void init_matrix(double number, matrix_t *A) {
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; number++, j++) A->matrix[i][j] = number;
  }
}

void fill(matrix_t *matrixStruct, double value) {
  for (int i = 0; i < matrixStruct->rows; i++) {
    for (int j = 0; j < matrixStruct->columns; j++) {
      matrixStruct->matrix[i][j] = value;
    }
  }
}

void gen_matrix(matrix_t *A) {
  double num = 1.0;
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++, num++) {
      A->matrix[i][j] = num;
    }
  }
}

void anti_gen_matrix(matrix_t *A) {
  double num = 32.0;
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++, num--) {
      A->matrix[i][j] = num;
    }
  }
}

START_TEST(create_matrix) {
  int code = 0;
  matrix_t A = {0};
  code = create(1, 1, &A);
  remv(&A);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(mult_num) {
  const int rows = 2;
  const int cols = 2;
  matrix_t m = {0};
  create(rows, cols, &m);
  matrix_t check = {0};
  create(rows, cols, &check);
  m.matrix[0][0] = 1;
  m.matrix[0][1] = 1;
  m.matrix[1][0] = 1;
  m.matrix[1][1] = 1;
  check.matrix[0][0] = 2;
  check.matrix[0][1] = 2;
  check.matrix[1][0] = 2;
  check.matrix[1][1] = 2;
  int num = 2;
  matrix_t res = {0};
  ck_assert_int_eq(mult_number(&m, num, &res), 0);
  ck_assert_int_eq(eq_matrix(&check, &res), 1);
  remv(&m);
  remv(&res);
  remv(&check);
}
END_TEST

START_TEST(transpose_matrix) {
  const int rows = 2;
  const int cols = 2;
  matrix_t m = {0};
  create(rows, cols, &m);
  matrix_t check = {0};
  create(cols, rows, &check);
  m.matrix[0][0] = 1;
  m.matrix[0][1] = 2;
  m.matrix[1][0] = 3;
  m.matrix[1][1] = 4;
  check.matrix[0][0] = 1;
  check.matrix[0][1] = 3;
  check.matrix[1][0] = 2;
  check.matrix[1][1] = 4;
  matrix_t res = {0};
  ck_assert_int_eq(transpose(&m, &res), 0);
  ck_assert_int_eq(eq_matrix(&check, &res), 1);
  remv(&m);
  remv(&res);
  remv(&check);
}
END_TEST

START_TEST(mult_hardcode) {
  const int rows = 3;
  const int cols = 3;
  matrix_t m = {0};
  create(rows, cols, &m);
  matrix_t mtx = {0};
  create(cols, rows, &mtx);

  int md = 1;
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++) m.matrix[i][j] = md++;
  int k1 = 1;
  for (int i = 0; i < cols; i++)
    for (int j = 0; j < rows; j++) mtx.matrix[i][j] = k1++;
  matrix_t check = {0};
  create(m.rows, mtx.columns, &check);

  for (int i = 0; i < m.rows; i++) {
    for (int j = 0; j < mtx.columns; j++) {
      check.matrix[i][j] = 0;
      for (int k = 0; k < m.columns; k++)
        check.matrix[i][j] += m.matrix[i][k] * mtx.matrix[k][j];
    }
  }

  matrix_t res = {0};

  ck_assert_int_eq(mult_matrix(&m, &mtx, &res), 0);

  ck_assert_int_eq(eq_matrix(&check, &res), 1);

  remv(&m);
  remv(&mtx);
  remv(&res);
  remv(&check);
}
END_TEST

START_TEST(complements_hardcoded) {
  const int size = 3;
  matrix_t m = {0};
  create(size, size, &m);
  m.matrix[0][0] = 1;
  m.matrix[0][1] = 2;
  m.matrix[0][2] = 3;
  m.matrix[1][1] = 4;
  m.matrix[1][2] = 2;
  m.matrix[2][0] = 5;
  m.matrix[2][1] = 2;
  m.matrix[2][2] = 1;

  matrix_t expected = {0};
  create(size, size, &expected);
  expected.matrix[0][1] = 10;
  expected.matrix[0][2] = -20;
  expected.matrix[1][0] = 4;
  expected.matrix[1][1] = -14;
  expected.matrix[1][2] = 8;
  expected.matrix[2][0] = -8;
  expected.matrix[2][1] = -2;
  expected.matrix[2][2] = 4;

  matrix_t res = {0};
  calc_complements(&m, &res);

  ck_assert_int_eq(eq_matrix(&expected, &res), 1);
  remv(&m);
  remv(&res);
  remv(&expected);
}
END_TEST

START_TEST(determinant_hardcoded) {
  const int rows = 3;
  const int cols = 3;
  matrix_t m1 = {0};

  create(rows, cols, &m1);
  int m = 1;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      m1.matrix[i][j] = m++;
    }
  }
  m1.matrix[2][2] = 0;
  double result = 0;
  int got = determinant(&m1, &result);
  ck_assert_double_eq_tol(result, 27, 1e-6);
  ck_assert_int_eq(got, 0);
  remv(&m1);
}
END_TEST

START_TEST(inverse_hardcoded) {
  const int size = 3;
  matrix_t m = {0};
  create(size, size, &m);
  m.matrix[0][0] = 2;
  m.matrix[0][1] = 5;
  m.matrix[0][2] = 7;
  m.matrix[1][0] = 6;
  m.matrix[1][1] = 3;
  m.matrix[1][2] = 4;
  m.matrix[2][0] = 5;
  m.matrix[2][1] = -2;
  m.matrix[2][2] = -3;
  matrix_t res = {0};
  inverse_matrix(&m, &res);
  matrix_t expected = {0};
  create(size, size, &expected);
  expected.matrix[0][0] = 1;
  expected.matrix[0][1] = -1;
  expected.matrix[0][2] = 1;
  expected.matrix[1][0] = -38;
  expected.matrix[1][1] = 41;
  expected.matrix[1][2] = -34;
  expected.matrix[2][0] = 27;
  expected.matrix[2][1] = -29;
  expected.matrix[2][2] = 24;
  ck_assert_int_eq(eq_matrix(&expected, &res), 1);
  remv(&expected);
  remv(&res);
  remv(&m);
}
END_TEST

START_TEST(test_eq_matrix) {
  matrix_t temp;
  matrix_t temp2;
  matrix_t temp3;
  create(3, 3, &temp);
  create(3, 3, &temp2);
  create(3, 3, &temp3);
  gen_matrix(&temp);
  gen_matrix(&temp2);
  ck_assert_int_eq(eq_matrix(&temp, &temp2), 1);
  ck_assert_int_eq(eq_matrix(&temp, &temp3), 0);
  remv(&temp);
  remv(&temp2);
  remv(&temp3);
}
END_TEST

START_TEST(test_eq_matrix_2) {
  matrix_t temp;
  matrix_t temp2;
  create(3, 3, &temp);
  create(3, 3, &temp2);
  gen_matrix(&temp);
  gen_matrix(&temp2);
  temp2.matrix[1][1] = 0;
  temp2.matrix[1][2] = 77;
  ck_assert_int_eq(eq_matrix(&temp, &temp2), 0);
  remv(&temp);
  remv(&temp2);
}
END_TEST

START_TEST(test_eq_matrix_3) {
  matrix_t temp;
  matrix_t temp2;
  create(3, 3, &temp);
  create(2, 3, &temp2);
  gen_matrix(&temp);
  gen_matrix(&temp2);
  ck_assert_int_eq(eq_matrix(&temp, &temp2), 0);
  remv(&temp);
  remv(&temp2);
}
END_TEST

START_TEST(test_eq_matrix_4) {
  matrix_t temp;
  matrix_t temp2;
  create(3, 3, &temp);
  create(3, 21, &temp2);
  gen_matrix(&temp);
  gen_matrix(&temp2);
  ck_assert_int_eq(eq_matrix(&temp, &temp2), 0);
  remv(&temp);
  remv(&temp2);
}
END_TEST

START_TEST(test_eq_matrix_5) {
  matrix_t temp;
  matrix_t temp2;
  create(3, 3, &temp);
  create(3, 3, &temp2);
  gen_matrix(&temp);
  gen_matrix(&temp2);
  temp.matrix[0][1] = 4.67773;
  temp2.matrix[0][1] = 4.67774;
  ck_assert_int_eq(eq_matrix(&temp, &temp2), 0);
  remv(&temp);
  remv(&temp2);
}
END_TEST

START_TEST(test_sum_matrix) {
  matrix_t temp;
  matrix_t temp2;
  matrix_t result;
  matrix_t result_defolt;
  create(3, 3, &temp);
  create(3, 3, &temp2);
  create(3, 3, &result_defolt);
  gen_matrix(&temp);
  anti_gen_matrix(&temp2);
  for (int i = 0; (i < temp.rows); i++) {
    for (int j = 0; j < temp.columns; j++) {
      result_defolt.matrix[i][j] = temp.matrix[i][j] + temp2.matrix[i][j];
    }
  }
  ck_assert_int_eq(sum_matrix(&temp, &temp2, &result), 0);
  ck_assert_int_eq(eq_matrix(&result_defolt, &result), 1);
  remv(&temp);
  remv(&temp2);
  remv(&result);
  remv(&result_defolt);
}
END_TEST

START_TEST(test_sum_matrix_2) {
  matrix_t temp;
  matrix_t temp2;
  matrix_t result;
  create(3, 3, &temp);
  create(3, 3, &temp2);
  gen_matrix(&temp);
  gen_matrix(&temp2);
  ck_assert_int_eq(sum_matrix(&temp, &temp2, &result), 0);
  remv(&temp);
  remv(&temp2);
  remv(&result);
}
END_TEST

START_TEST(test_sub_matrix) {
  matrix_t temp;
  matrix_t temp2;
  matrix_t result;
  matrix_t result_defolt;
  create(3, 3, &temp);
  create(3, 3, &temp2);
  create(3, 3, &result_defolt);
  gen_matrix(&temp);
  anti_gen_matrix(&temp2);
  for (int i = 0; (i < temp.rows); i++) {
    for (int j = 0; j < temp.columns; j++) {
      result_defolt.matrix[i][j] = temp.matrix[i][j] - temp2.matrix[i][j];
    }
  }
  ck_assert_int_eq(sub_matrix(&temp, &temp2, &result), 0);
  ck_assert_int_eq(eq_matrix(&result_defolt, &result), 1);
  remv(&temp);
  remv(&temp2);
  remv(&result);
  remv(&result_defolt);
}
END_TEST

START_TEST(test_sub_matrix_2) {
  matrix_t temp;
  matrix_t temp2;
  matrix_t result;
  create(3, 3, &temp);
  create(3, 3, &temp2);
  gen_matrix(&temp);
  gen_matrix(&temp2);
  ck_assert_int_eq(sub_matrix(&temp, &temp2, &result), 0);
  remv(&temp);
  remv(&temp2);
  remv(&result);
}
END_TEST

START_TEST(test_mult_number) {
  matrix_t temp;
  matrix_t temp2;
  matrix_t result;
  matrix_t result_defolt;
  double num = 777.777;
  create(3, 3, &temp);
  create(3, 3, &temp2);
  create(3, 3, &result_defolt);
  gen_matrix(&temp);
  gen_matrix(&temp2);
  for (int i = 0; (i < temp.rows); i++) {
    for (int j = 0; j < temp.columns; j++) {
      result_defolt.matrix[i][j] = temp.matrix[i][j] * 777.777;
    }
  }
  ck_assert_int_eq(mult_number(&temp, num, &result), 0);
  ck_assert_int_eq(eq_matrix(&result_defolt, &result), 1);
  remv(&temp);
  remv(&temp2);
  remv(&result);
  remv(&result_defolt);
}
END_TEST

START_TEST(test_mult_number_2) {
  matrix_t temp;
  matrix_t temp2;
  matrix_t result;
  matrix_t result_defolt;
  double num = 777.777;
  create(3, 3, &temp);
  create(3, 3, &temp2);
  create(3, 3, &result_defolt);
  gen_matrix(&temp);
  gen_matrix(&temp2);
  for (int i = 0; i < temp.rows; i++) {
    for (int j = 0; j < temp.columns; j++) {
      result_defolt.matrix[i][j] = temp.matrix[i][j] * 777.777;
    }
  }
  ck_assert_int_eq(mult_number(&temp, num, &result), 0);
  ck_assert_int_eq(eq_matrix(&result_defolt, &result), 1);
  remv(&temp);
  remv(&temp2);
  remv(&result);
  remv(&result_defolt);
}
END_TEST

START_TEST(test_mult_matrix_2) {
  matrix_t temp;
  matrix_t temp2;
  matrix_t result;
  matrix_t result_defolt;
  create(3, 2, &temp);
  create(2, 3, &temp2);
  create(3, 3, &result_defolt);
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
  remv(&temp);
  remv(&temp2);
  remv(&result);
  remv(&result_defolt);
}
END_TEST

START_TEST(test_mult_matrix_4) {
  matrix_t temp;
  matrix_t temp2;
  matrix_t result;
  matrix_t result_defolt;
  create(2, 3, &temp);
  create(3, 2, &temp2);
  create(2, 2, &result_defolt);
  gen_matrix(&temp);
  gen_matrix(&temp2);
  result_defolt.matrix[0][0] = 22;
  result_defolt.matrix[0][1] = 28;
  result_defolt.matrix[1][0] = 49;
  result_defolt.matrix[1][1] = 64;
  ck_assert_int_eq(mult_matrix(&temp, &temp2, &result), 0);
  ck_assert_int_eq(eq_matrix(&result_defolt, &result), 1);
  remv(&temp);
  remv(&temp2);
  remv(&result);
  remv(&result_defolt);
}
END_TEST

START_TEST(test_mult_matrix_3) {
  matrix_t temp;
  matrix_t temp2;
  matrix_t result;
  create(3, 3, &temp);
  create(3, 3, &temp2);
  gen_matrix(&temp);
  gen_matrix(&temp2);
  ck_assert_int_eq(mult_matrix(&temp, &temp2, &result), 0);
  remv(&temp);
  remv(&temp2);
  remv(&result);
}
END_TEST

START_TEST(test_transpose) {
  matrix_t temp;
  matrix_t result;
  matrix_t result_defolt;
  create(3, 2, &temp);
  create(2, 3, &result_defolt);
  gen_matrix(&temp);
  result_defolt.matrix[0][0] = 1;
  result_defolt.matrix[0][1] = 3;
  result_defolt.matrix[0][2] = 5;
  result_defolt.matrix[1][0] = 2;
  result_defolt.matrix[1][1] = 4;
  result_defolt.matrix[1][2] = 6;
  ck_assert_int_eq(transpose(&temp, &result), 0);
  ck_assert_int_eq(eq_matrix(&result_defolt, &result), 1);
  remv(&temp);
  remv(&result);
  remv(&result_defolt);
}
END_TEST

START_TEST(test_determinant) {
  matrix_t temp;
  double result;
  int result_defolt;
  create(3, 2, &temp);
  gen_matrix(&temp);
  result_defolt = determinant(&temp, &result);
  ck_assert_int_eq(result_defolt, 2);
  remv(&temp);
}
END_TEST

START_TEST(test_determinant_2) {
  matrix_t temp;
  double result;
  int result_defolt;
  create(3, 3, &temp);
  gen_matrix(&temp);
  result_defolt = determinant(&temp, &result);
  ck_assert_int_eq(result_defolt, 0);
  ck_assert_uint_eq(result, 0);
  remv(&temp);
}
END_TEST

START_TEST(test_determinant_3) {
  matrix_t temp;
  double result;
  int result_defolt;
  create(3, 3, &temp);
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
  remv(&temp);
}
END_TEST

START_TEST(test_determinant_5) {
  matrix_t temp;
  double result;
  int result_defolt;
  create(3, 3, &temp);
  temp.matrix[0][0] = 1;
  temp.matrix[1][1] = 1;
  temp.matrix[2][2] = 1;

  result_defolt = determinant(&temp, &result);
  ck_assert_int_eq(result_defolt, 0);
  ck_assert_uint_eq(result, 1);
  remv(&temp);
}
END_TEST

START_TEST(test_calc_complements_2) {
  matrix_t test, result;
  create(3, 3, &test);
  gen_matrix(&test);
  int result_status = calc_complements(&test, &result);
  ck_assert_int_eq(result_status, 0);
  remv(&test);
  remv(&result);
}
END_TEST

START_TEST(test_calc_complements_3) {
  matrix_t test, test_2, result;
  create(3, 3, &test);
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
  create(3, 3, &test_2);
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
  remv(&test);
  remv(&test_2);
  remv(&result);
}
END_TEST

START_TEST(test_inverse_matrix) {
  matrix_t temp, result_defolt, result_defolt_2;
  create(3, 3, &temp);
  create(3, 3, &result_defolt);
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
  remv(&temp);
  remv(&result_defolt);
  remv(&result_defolt_2);
}
END_TEST

START_TEST(test_inverse_matrix_2) {
  matrix_t temp, result;
  create(3, 3, &temp);
  gen_matrix(&temp);
  int result_status = inverse_matrix(&temp, &result);
  ck_assert_int_eq(result_status, 2);
  remv(&temp);
}
END_TEST

START_TEST(test_inverse_matrix_3) {
  matrix_t temp, result;
  create(3, 3, &temp);
  gen_matrix(&temp);
  int result_status = inverse_matrix(&temp, &result);
  ck_assert_int_eq(result_status, 2);
  remv(&temp);
}
END_TEST

START_TEST(test_inverse_matrix_4) {
  matrix_t temp, result;
  create(3, 3, &temp);
  temp.matrix[0][0] = 1;
  temp.matrix[1][1] = 1;
  temp.matrix[2][2] = 1;
  inverse_matrix(&temp, &result);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_uint_eq(temp.matrix[i][j], result.matrix[i][j]);
    }
  }
  remv(&temp);
  remv(&result);
}
END_TEST

START_TEST(eq_matrix_test_1) {
  matrix_t result1;
  matrix_t result2;
  create(41, 41, &result1);
  create(41, 41, &result2);
  fill(&result1, 1);
  fill(&result2, 1);
  int expected_result = 1;
  int actual_result = eq_matrix(&result1, &result2);
  ck_assert_uint_eq(expected_result, actual_result);
  remv(&result1);
  remv(&result2);
}
END_TEST

START_TEST(eq_matrix_test_2) {
  matrix_t result1;
  matrix_t result2;
  create(83, 83, &result1);
  create(83, 83, &result2);
  fill(&result1, 7);
  fill(&result2, 9);
  int expected_result = 0;
  int actual_result = eq_matrix(&result1, &result2);
  ck_assert_uint_eq(expected_result, actual_result);
  remv(&result1);
  remv(&result2);
}
END_TEST

START_TEST(eq_matrix_test_3) {
  matrix_t result1;
  matrix_t result2;
  create(32, 32, &result1);
  create(27, 27, &result2);
  fill(&result1, 42);
  fill(&result2, 42);
  int expected_result = 0;
  int actual_result = eq_matrix(&result1, &result2);
  ck_assert_uint_eq(expected_result, actual_result);
  remv(&result1);
  remv(&result2);
}
END_TEST

START_TEST(eq_matrix_test_4) {
  matrix_t result1;
  matrix_t result2;
  create(22, 22, &result1);
  create(22, 22, &result2);
  fill(&result1, 13);
  fill(&result2, 12);
  int expected_result = 0;
  int actual_result = eq_matrix(&result1, &result2);
  ck_assert_uint_eq(expected_result, actual_result);
  remv(&result1);
  remv(&result2);
}
END_TEST

START_TEST(sum_matrix_test_1) {
  matrix_t result;
  matrix_t result1;
  matrix_t result2;
  matrix_t result3;
  create(4, 4, &result1);
  create(4, 4, &result2);
  fill(&result1, 3);
  fill(&result2, 4);
  create(4, 4, &result);
  fill(&result, 7);
  sum_matrix(&result1, &result2, &result3);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_double_eq(result.matrix[i][j], result3.matrix[i][j]);
    }
  }
  remv(&result1);
  remv(&result2);
  remv(&result3);
  remv(&result);
}
END_TEST

START_TEST(sum_matrix_test_2) {
  matrix_t result;
  matrix_t result1;
  matrix_t result2;
  matrix_t result3;
  create(2, 2, &result1);
  create(4, 4, &result2);
  int expected_result = create(2, 2, &result3);
  int actual_result = sum_matrix(&result1, &result2, &result);
  ck_assert_int_ne(expected_result, actual_result);
  remv(&result1);
  remv(&result2);
  remv(&result3);
}
END_TEST

START_TEST(sum_matrix_test_3) {
  matrix_t result;
  matrix_t result1;
  matrix_t result2;
  matrix_t result3;
  create(4, 4, &result1);
  create(4, 4, &result2);
  fill(&result1, 0);
  result2.matrix[0][0] = 1;
  result2.matrix[1][1] = 1;
  result2.matrix[2][2] = 1;
  result2.matrix[3][3] = 1;
  int expected_result = create(4, 4, &result3);
  result3.matrix[0][0] = 1;
  result3.matrix[1][1] = 1;
  result3.matrix[2][2] = 1;
  result3.matrix[3][3] = 1;
  int actual_result = sum_matrix(&result1, &result2, &result);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_double_eq(result3.matrix[i][j], result.matrix[i][j]);
    }
  }
  ck_assert_uint_eq(expected_result, actual_result);
  remv(&result1);
  remv(&result2);
  remv(&result3);
  remv(&result);
}
END_TEST

START_TEST(sub_matrix_test_1) {
  matrix_t result;
  matrix_t result1;
  matrix_t result2;
  matrix_t result3;
  create(4, 4, &result1);
  create(4, 4, &result2);
  fill(&result1, 4);
  fill(&result2, 3);
  create(4, 4, &result);
  fill(&result, 1);
  int sum_m = sub_matrix(&result1, &result2, &result3);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_double_eq(result.matrix[i][j], result3.matrix[i][j]);
    }
  }
  ck_assert_uint_eq(sum_m, 0);
  remv(&result1);
  remv(&result2);
  remv(&result3);
  remv(&result);
}
END_TEST

START_TEST(sub_matrix_test_2) {
  matrix_t result;
  matrix_t result1;
  matrix_t result2;
  matrix_t result3;
  create(2, 2, &result1);
  create(4, 4, &result2);
  int expected_result = create(2, 2, &result3);
  int actual_result = sum_matrix(&result1, &result2, &result);
  ck_assert_int_ne(expected_result, actual_result);
  remv(&result1);
  remv(&result2);
  remv(&result3);
}
END_TEST

START_TEST(sub_matrix_test_3) {
  matrix_t result;
  matrix_t result1;
  matrix_t result2;
  matrix_t result3;
  create(4, 4, &result1);
  create(4, 4, &result2);
  fill(&result2, 0);
  result1.matrix[0][0] = 1;
  result1.matrix[1][1] = 1;
  result1.matrix[2][2] = 1;
  result1.matrix[3][3] = 1;
  int expected_result = create(4, 4, &result3);
  result3.matrix[0][0] = 1;
  result3.matrix[1][1] = 1;
  result3.matrix[2][2] = 1;
  result3.matrix[3][3] = 1;
  int actual_result = sub_matrix(&result1, &result2, &result);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_double_eq(result3.matrix[i][j], result.matrix[i][j]);
    }
  }
  ck_assert_uint_eq(expected_result, actual_result);
  remv(&result1);
  remv(&result2);
  remv(&result3);
  remv(&result);
}
END_TEST

START_TEST(mult_number_test_1) {
  matrix_t result1;
  matrix_t result2;
  matrix_t result3;
  create(4, 4, &result1);
  fill(&result1, 3);
  int expected_result = create(4, 4, &result2);
  fill(&result2, 30);
  int actual_result = mult_number(&result1, 10, &result3);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_double_eq(result3.matrix[i][j], result2.matrix[i][j]);
    }
  }
  ck_assert_int_eq(expected_result, actual_result);
  remv(&result1);
  remv(&result2);
  remv(&result3);
}
END_TEST

START_TEST(mult_number_test_2) {
  matrix_t result1;
  matrix_t result3;
  create(2, 2, &result1);
  int expected_result = 0;
  int actual_result = mult_number(&result1, 3, &result3);
  ck_assert_uint_eq(expected_result, actual_result);
  remv(&result3);
  remv(&result1);
}
END_TEST

START_TEST(mult_matrix_test_1) {
  matrix_t result1;
  matrix_t result2;
  matrix_t result3;
  matrix_t result4;
  create(4, 4, &result1);
  create(4, 4, &result2);
  fill(&result1, 3);
  fill(&result2, 3);
  int expected_result = create(4, 4, &result3);
  fill(&result3, 36);
  int actual_result = mult_matrix(&result1, &result2, &result4);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_double_eq(result3.matrix[i][j], result4.matrix[i][j]);
    }
  }
  ck_assert_uint_eq(expected_result, actual_result);
  remv(&result4);
  remv(&result3);
  remv(&result2);
  remv(&result1);
}
END_TEST

START_TEST(mult_matrix_test_2) {
  matrix_t result1;
  matrix_t result2;
  matrix_t result3;
  matrix_t result4;
  create(4, 4, &result1);
  create(3, 3, &result2);
  fill(&result1, 3);
  fill(&result2, 3);
  int expected_result = create(4, 4, &result3);
  expected_result = 2;
  int actual_result = mult_matrix(&result1, &result2, &result4);
  ck_assert_uint_eq(expected_result, actual_result);
  remv(&result3);
  remv(&result2);
  remv(&result1);
}
END_TEST

START_TEST(calc_complements_test_1) {
  matrix_t result1;
  matrix_t result2;
  matrix_t result3;

  create(3, 3, &result1);
  result1.matrix[0][0] = 1;
  result1.matrix[0][1] = 2;
  result1.matrix[0][2] = 3;

  result1.matrix[1][0] = 0;
  result1.matrix[1][1] = 4;
  result1.matrix[1][2] = 2;

  result1.matrix[2][0] = 5;
  result1.matrix[2][1] = 2;
  result1.matrix[2][2] = 1;
  int expected_result = create(3, 3, &result2);
  result2.matrix[0][0] = 0;
  result2.matrix[0][1] = 10;
  result2.matrix[0][2] = -20;

  result2.matrix[1][0] = 4;
  result2.matrix[1][1] = -14;
  result2.matrix[1][2] = 8;

  result2.matrix[2][0] = -8;
  result2.matrix[2][1] = -2;
  result2.matrix[2][2] = 4;
  int actual_result = calc_complements(&result1, &result3);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_double_eq_tol(result2.matrix[i][j], result3.matrix[i][j], 1e-6);
    }
  }
  ck_assert_uint_eq(expected_result, actual_result);
  remv(&result3);
  remv(&result2);
  remv(&result1);
}
END_TEST

START_TEST(determinant_test_1) {
  matrix_t result1;
  double result2;
  create(3, 3, &result1);
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
  remv(&result1);
}
END_TEST

START_TEST(determinant_test_2) {
  matrix_t result1;
  double result3;
  create(3, 4, &result1);
  int actual_result = 2;
  int expected_result = determinant(&result1, &result3);
  ck_assert_uint_eq(expected_result, actual_result);
  remv(&result1);
}
END_TEST

START_TEST(determinant_test_3) {
  matrix_t result1;
  double result2;
  create(3, 3, &result1);
  result1.matrix[0][0] = 3.0;
  result1.matrix[0][1] = 4.0;
  result1.matrix[0][2] = 5.0;

  result1.matrix[1][0] = 11.0;
  result1.matrix[1][1] = 22.0;
  result1.matrix[1][2] = 6.0;

  result1.matrix[2][0] = 71.0;
  result1.matrix[2][1] = 82.0;
  result1.matrix[2][2] = 99.0;
  double expected_result = -894.0;
  determinant(&result1, &result2);
  ck_assert_double_eq_tol(expected_result, result2, 1e-7);
  remv(&result1);
}
END_TEST

START_TEST(inverse_matrix_test_1) {
  matrix_t result1;
  matrix_t result2;
  matrix_t result3;
  create(3, 3, &result1);
  result1.matrix[0][0] = 2;
  result1.matrix[0][1] = 5;
  result1.matrix[0][2] = 7;

  result1.matrix[1][0] = 6;
  result1.matrix[1][1] = 3;
  result1.matrix[1][2] = 4;

  result1.matrix[2][0] = 5;
  result1.matrix[2][1] = -2;
  result1.matrix[2][2] = -3;

  int expected_result = create(3, 3, &result2);
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
  remv(&result3);
  remv(&result2);
  remv(&result1);
}
END_TEST

START_TEST(inverse_matrix_test_2) {
  matrix_t result1;
  matrix_t result3;
  create(3, 3, &result1);
  result1.matrix[0][0] = 1;
  result1.matrix[0][1] = 2;
  result1.matrix[0][2] = 3;

  result1.matrix[1][0] = 4;
  result1.matrix[1][1] = 5;
  result1.matrix[1][2] = 6;

  result1.matrix[2][0] = 7;
  result1.matrix[2][1] = 8;
  result1.matrix[2][2] = 9;
  int expected_result = 2;
  int actual_result = inverse_matrix(&result1, &result3);
  ck_assert_uint_eq(expected_result, actual_result);

  remv(&result1);
}
END_TEST

START_TEST(calc_determinant_core) {
  matrix_t A;
  matrix_t B;
  matrix_t C;
  matrix_t D;
  double A_result;
  double B_result;
  double C_result;
  double D_result;

  double test_1[3][3][3] = {{{1350.717, 250.7127, 340.77},
                             {450.707, 520.717, 660.77},
                             {770.707, 840.717, 9750.77}},

                            {{191.709, 2920.797, 3920.707},
                             {491.709, 590.7097, 690.797},
                             {791.71, 890.71, 990.71}},

                            {{1901.757, 291.705, 3901.71},
                             {491.75, 5901.705, 6901.75},
                             {7901.775, 8901.7050, 99001.75}}};

  double test_2[1][5][5] = {{{13, 25, 30, 25, 30},
                             {45, 52, 60, 50, 66},
                             {77, 84, 97, 84, 97},
                             {77, 84, 97, 84, 97},
                             {77, 84, 97, 84, 97}}};

  double answer_1 = 5125994629.3762889999;
  double answer_2 = 52361913.725383336231;
  double answer_3 = 831152331541.16479492;
  create(3, 3, &A);
  create(3, 3, &B);
  create(3, 3, &C);
  create(5, 5, &D);
  for (int x = 0; x < 3; x++) {
    for (int y = 0; y < 3; y++) {
      for (int z = 0; z < 3; z++) {
        if (x == 0) A.matrix[y][z] = test_1[x][y][z];
        if (x == 1) B.matrix[y][z] = test_1[x][y][z];
        if (x == 2) C.matrix[y][z] = test_1[x][y][z];
      }
    }
  }
  for (int x = 0; x < 1; x++) {
    for (int y = 0; y < 5; y++) {
      for (int z = 0; z < 5; z++) {
        if (x == 0) D.matrix[y][z] = test_2[x][y][z];
      }
    }
  }
  determinant(&A, &A_result);
  determinant(&B, &B_result);
  determinant(&C, &C_result);
  determinant(&D, &D_result);
  ck_assert_double_eq_tol(A_result, answer_1, 1e-6);
  ck_assert_double_eq_tol(B_result, answer_2, 1e-6);
  //  printf("%.8f %.8f\n", C_result, answer_3);
  ck_assert_double_eq_tol(C_result, answer_3, 1e-6);
  remv(&A);
  remv(&B);
  remv(&C);
  remv(&D);
}
END_TEST

START_TEST(create_01) {
  int res = 0;
  matrix_t A = {0};

  res = create(1, 1, &A);
  ck_assert_int_eq(res, OK);
  remv(&A);
}
END_TEST

START_TEST(create_02) {
  int res = 0;
  matrix_t A = {0};

  res = create(2, 2, &A);
  ck_assert_int_eq(res, OK);
  remv(&A);
}
END_TEST
START_TEST(create_NULL) {
  int res = 0;
  matrix_t A = {0};
#define calloc(x, y) NULL
  res = create(2, 2, &A);
  ck_assert_int_eq(res, OK);
  remv(&A);
#undef calloc
}
END_TEST

START_TEST(create_03) {
  int res = 0;
  matrix_t A = {0};

  res = create(3, 2, &A);
  ck_assert_int_eq(res, OK);
  remv(&A);
}
END_TEST

START_TEST(create_04) {
  int res = 0;
  matrix_t A = {0};

  res = create(2, 4, &A);
  ck_assert_int_eq(res, OK);
  remv(&A);
}
END_TEST

START_TEST(create_05) {
  int res = 0;
  matrix_t A = {0};

  res = create(10, 15, &A);
  ck_assert_int_eq(res, OK);
  remv(&A);
}
END_TEST

START_TEST(create_06) {
  int res = 0;
  matrix_t A = {0};

  res = create(0, 0, &A);
  ck_assert_int_eq(res, ERROR_INIT);
  remv(&A);
}
END_TEST

START_TEST(create_07) {
  int res = 0;
  matrix_t A = {0};

  res = create(1, 0, &A);
  ck_assert_int_eq(res, ERROR_INIT);
  remv(&A);
}
END_TEST

START_TEST(create_08) {
  int res = 0;
  matrix_t A = {0};

  res = create(0, 1, &A);
  ck_assert_int_eq(res, ERROR_INIT);
  remv(&A);
}
END_TEST

START_TEST(create_09) {
  int res = 0;
  matrix_t A = {0};
  res = create(-1, 1, &A);
  ck_assert_int_eq(res, ERROR_INIT);
  remv(&A);
}
END_TEST

START_TEST(remv_01) {
  int res = 0;
  matrix_t A = {0};

  res = create(2, 2, &A);
  remv(&A);
  if (A.matrix) {
    res = OK;
  }
  ck_assert_int_eq(res, OK);
}
END_TEST

START_TEST(remv_02) {
  int res = 0;
  matrix_t A = {0};

  res = create(1, 1, &A);
  remv(&A);
  if (A.matrix) {
    res = OK;
  }
  ck_assert_int_eq(res, OK);
}
END_TEST

START_TEST(remv_03) {
  int res = 0;
  matrix_t A = {0};

  res = create(3, 2, &A);
  remv(&A);
  if (A.matrix != NULL) {
    res = OK;
  }
  ck_assert_int_eq(res, OK);
}
END_TEST

START_TEST(remv_04) {
  int res = 0;
  matrix_t A = {0};

  res = create(3, 4, &A);
  remv(&A);
  if (A.matrix != NULL) {
    res = OK;
  }
  ck_assert_int_eq(res, OK);
}
END_TEST

START_TEST(remv_05) {
  int res = 0;
  matrix_t A = {0};

  res = create(3, 2, &A);
  remv(&A);
  if (A.matrix != NULL) {
    res = OK;
  }
  ck_assert_int_eq(res, OK);
}
END_TEST

START_TEST(eq_matrix_01) {
  int res = 0;
  matrix_t A = {0};
  matrix_t B = {0};

  create(3, 2, &A);
  init_matrix(1.0, &A);

  create(3, 2, &B);
  init_matrix(1.0, &B);

  res = eq_matrix(&A, &B);

  remv(&A);
  remv(&B);

  ck_assert_int_eq(res, SUCCESS);
}
END_TEST

START_TEST(eq_matrix_02) {
  int res = 0;
  matrix_t A = {0};
  matrix_t B = {0};

  create(3, 2, &A);
  init_matrix(1.0, &A);

  create(3, 2, &B);
  init_matrix(2.0, &B);

  res = eq_matrix(&B, &A);

  remv(&A);
  remv(&B);

  ck_assert_int_eq(res, FAILURE);
}
END_TEST

START_TEST(eq_matrix_03) {
  int res = 0;
  matrix_t A = {0};
  matrix_t B = {0};

  create(3, 4, &A);
  init_matrix(-5.0, &A);

  create(3, 4, &B);
  init_matrix(1.0, &B);

  res = eq_matrix(&A, &B);

  remv(&A);
  remv(&B);

  ck_assert_int_eq(res, FAILURE);
}
END_TEST

START_TEST(eq_matrix_04) {
  int res = 0;
  matrix_t A = {0};
  matrix_t B = {0};

  create(3, 2, &A);
  init_matrix(10.0, &A);

  create(3, 2, &B);
  init_matrix(-10.0, &B);

  res = eq_matrix(&A, &B);

  remv(&A);
  remv(&B);

  ck_assert_int_eq(res, FAILURE);
}
END_TEST

START_TEST(eq_matrix_05) {
  int res = 0;
  matrix_t A = {0};
  matrix_t B = {0};

  create(3, 2, &A);
  init_matrix(1.0, &A);
  create(3, 3, &B);
  init_matrix(1.0, &B);
  res = eq_matrix(&A, &B);
  remv(&A);
  remv(&B);
  ck_assert_int_eq(res, FAILURE);
}
END_TEST

START_TEST(eq_matrix_06) {
  int res = 0;
  matrix_t A = {0};
  matrix_t B = {0};

  create(10, 10, &A);
  init_matrix(1.0, &A);

  create(10, 10, &B);
  init_matrix(1.0, &B);

  res = eq_matrix(&A, &B);

  remv(&A);
  remv(&B);

  ck_assert_int_eq(res, SUCCESS);
}
END_TEST

START_TEST(sum_matrix_01) {
  int res = 0;
  matrix_t A = {0};
  matrix_t B = {0};
  matrix_t Z = {0};
  matrix_t X = {0};

  create(4, 4, &A);
  init_matrix(1.0, &A);

  create(4, 4, &B);
  init_matrix(1.0, &B);

  sum_matrix(&A, &B, &Z);

  create(4, 4, &X);

  X.matrix[0][0] = 2.0;
  X.matrix[0][1] = 4.0;
  X.matrix[0][2] = 6.0;
  X.matrix[0][3] = 8.0;
  X.matrix[1][0] = 10.0;
  X.matrix[1][1] = 12.0;
  X.matrix[1][2] = 14.0;
  X.matrix[1][3] = 16.0;
  X.matrix[2][0] = 18.0;
  X.matrix[2][1] = 20.0;
  X.matrix[2][2] = 22.0;
  X.matrix[2][3] = 24.0;
  X.matrix[3][0] = 26.0;
  X.matrix[3][1] = 28.0;
  X.matrix[3][2] = 30.0;
  X.matrix[3][3] = 32.0;

  res = eq_matrix(&X, &Z);

  remv(&A);
  remv(&B);
  remv(&X);
  remv(&Z);

  ck_assert_int_eq(res, SUCCESS);
}
END_TEST

START_TEST(sum_matrix_02) {
  int res = 0;
  matrix_t A = {0};
  matrix_t B = {0};
  matrix_t Z = {0};
  matrix_t X = {0};

  create(4, 4, &A);
  init_matrix(0.0, &A);

  create(4, 4, &B);
  init_matrix(2.0, &B);

  sum_matrix(&A, &B, &Z);

  create(4, 4, &X);

  X.matrix[0][0] = 2.0;
  X.matrix[0][1] = 4.0;
  X.matrix[0][2] = 6.0;
  X.matrix[0][3] = 8.0;
  X.matrix[1][0] = 10.0;
  X.matrix[1][1] = 12.0;
  X.matrix[1][2] = 14.0;
  X.matrix[1][3] = 16.0;
  X.matrix[2][0] = 18.0;
  X.matrix[2][1] = 20.0;
  X.matrix[2][2] = 22.0;
  X.matrix[2][3] = 24.0;
  X.matrix[3][0] = 26.0;
  X.matrix[3][1] = 28.0;
  X.matrix[3][2] = 30.0;
  X.matrix[3][3] = 32.0;

  res = eq_matrix(&X, &Z);

  remv(&A);
  remv(&B);
  remv(&X);
  remv(&Z);

  ck_assert_int_eq(res, SUCCESS);
}
END_TEST

START_TEST(sum_matrix_03) {
  int res = 0;
  matrix_t A = {0};
  matrix_t B = {0};
  matrix_t Z = {0};
  matrix_t X = {0};

  create(4, 4, &A);
  init_matrix(-3.0, &A);

  create(4, 4, &B);
  init_matrix(5.0, &B);

  sum_matrix(&A, &B, &Z);

  create(4, 4, &X);

  X.matrix[0][0] = 2.0;
  X.matrix[0][1] = 4.0;
  X.matrix[0][2] = 6.0;
  X.matrix[0][3] = 8.0;
  X.matrix[1][0] = 10.0;
  X.matrix[1][1] = 12.0;
  X.matrix[1][2] = 14.0;
  X.matrix[1][3] = 16.0;
  X.matrix[2][0] = 18.0;
  X.matrix[2][1] = 20.0;
  X.matrix[2][2] = 22.0;
  X.matrix[2][3] = 24.0;
  X.matrix[3][0] = 26.0;
  X.matrix[3][1] = 28.0;
  X.matrix[3][2] = 30.0;
  X.matrix[3][3] = 32.0;

  res = eq_matrix(&X, &Z);

  remv(&A);
  remv(&B);
  remv(&X);
  remv(&Z);

  ck_assert_int_eq(res, SUCCESS);
}
END_TEST

START_TEST(sum_matrix_04) {
  int res = 0;
  matrix_t A = {0};
  matrix_t B = {0};
  matrix_t Z = {0};
  matrix_t X = {0};

  create(4, 4, &A);
  init_matrix(5.0, &A);

  create(4, 4, &B);
  init_matrix(-3.0, &B);

  sum_matrix(&A, &B, &Z);

  create(4, 4, &X);

  X.matrix[0][0] = 2.0;
  X.matrix[0][1] = 4.0;
  X.matrix[0][2] = 6.0;
  X.matrix[0][3] = 8.0;
  X.matrix[1][0] = 10.0;
  X.matrix[1][1] = 12.0;
  X.matrix[1][2] = 14.0;
  X.matrix[1][3] = 16.0;
  X.matrix[2][0] = 18.0;
  X.matrix[2][1] = 20.0;
  X.matrix[2][2] = 22.0;
  X.matrix[2][3] = 24.0;
  X.matrix[3][0] = 26.0;
  X.matrix[3][1] = 28.0;
  X.matrix[3][2] = 30.0;
  X.matrix[3][3] = 32.0;

  res = eq_matrix(&X, &Z);

  remv(&A);
  remv(&B);
  remv(&X);
  remv(&Z);

  ck_assert_int_eq(res, SUCCESS);
}
END_TEST

START_TEST(sum_matrix_05) {
  int res = 0;
  matrix_t A = {0};
  matrix_t B = {0};
  matrix_t Z = {0};
  matrix_t X = {0};

  create(4, 2, &A);
  init_matrix(1.0, &A);

  create(4, 2, &B);
  init_matrix(1.0, &B);

  sum_matrix(&A, &B, &Z);

  create(4, 2, &X);

  X.matrix[0][0] = 2.0;
  X.matrix[0][1] = 4.0;
  X.matrix[1][0] = 6.0;
  X.matrix[1][1] = 8.0;
  X.matrix[2][0] = 10.0;
  X.matrix[2][1] = 12.0;
  X.matrix[3][0] = 14.0;
  X.matrix[3][1] = 16.0;

  res = eq_matrix(&X, &Z);

  remv(&A);
  remv(&B);
  remv(&X);
  remv(&Z);

  ck_assert_int_eq(res, SUCCESS);
}
END_TEST

START_TEST(sum_matrix_06) {
  int res = 0;
  matrix_t A = {0};
  matrix_t B = {0};
  matrix_t Z = {0};
  matrix_t X = {0};

  create(4, 2, &A);
  init_matrix(-1.0, &A);

  create(4, 2, &B);
  init_matrix(3.0, &B);

  sum_matrix(&A, &B, &Z);

  create(4, 2, &X);

  X.matrix[0][0] = 2.0;
  X.matrix[0][1] = 4.0;
  X.matrix[1][0] = 6.0;
  X.matrix[1][1] = 8.0;
  X.matrix[2][0] = 10.0;
  X.matrix[2][1] = 12.0;
  X.matrix[3][0] = 14.0;
  X.matrix[3][1] = 16.0;

  res = eq_matrix(&X, &Z);

  remv(&A);
  remv(&B);
  remv(&X);
  remv(&Z);

  ck_assert_int_eq(res, SUCCESS);
}
END_TEST

START_TEST(sum_matrix_07) {
  int res = 0;
  matrix_t A = {0};
  matrix_t B = {0};
  matrix_t Z = {0};
  matrix_t X = {0};

  create(4, 2, &A);
  init_matrix(4.0, &A);

  create(4, 2, &B);
  init_matrix(-2.0, &B);

  sum_matrix(&A, &B, &Z);

  create(4, 2, &X);

  X.matrix[0][0] = 2.0;
  X.matrix[0][1] = 4.0;
  X.matrix[1][0] = 6.0;
  X.matrix[1][1] = 8.0;
  X.matrix[2][0] = 10.0;
  X.matrix[2][1] = 12.0;
  X.matrix[3][0] = 14.0;
  X.matrix[3][1] = 16.0;

  res = eq_matrix(&X, &Z);

  remv(&A);
  remv(&B);
  remv(&X);
  remv(&Z);

  ck_assert_int_eq(res, SUCCESS);
}
END_TEST

START_TEST(sum_matrix_08) {
  int res = 0;
  matrix_t A = {0};
  matrix_t B = {0};
  matrix_t Z = {0};

  create(4, 2, &A);

  create(4, 3, &B);

  res = sum_matrix(&A, &B, &Z);

  remv(&A);
  remv(&B);
  remv(&Z);

  ck_assert_int_eq(res, ERROR_CALC);
}
END_TEST

START_TEST(sub_matrix_01) {
  int res = 0;
  matrix_t A = {0};
  matrix_t B = {0};
  matrix_t Z = {0};
  matrix_t X = {0};

  create(4, 4, &A);
  init_matrix(1.0, &A);

  create(4, 4, &B);
  init_matrix(1.0, &B);

  sub_matrix(&A, &B, &Z);

  create(4, 4, &X);

  X.matrix[0][0] = 0.0;
  X.matrix[0][1] = 0.0;
  X.matrix[0][2] = 0.0;
  X.matrix[0][3] = 0.0;
  X.matrix[1][0] = 0.0;
  X.matrix[1][1] = 0.0;
  X.matrix[1][2] = 0.0;
  X.matrix[1][3] = 0.0;
  X.matrix[2][0] = 0.0;
  X.matrix[2][1] = 0.0;
  X.matrix[2][2] = 0.0;
  X.matrix[2][3] = 0.0;
  X.matrix[3][0] = 0.0;
  X.matrix[3][1] = 0.0;
  X.matrix[3][2] = 0.0;
  X.matrix[3][3] = 0.0;

  res = eq_matrix(&X, &Z);

  remv(&A);
  remv(&B);
  remv(&X);
  remv(&Z);

  ck_assert_int_eq(res, SUCCESS);
}
END_TEST

START_TEST(sub_matrix_02) {
  matrix_t A = {0};
  matrix_t B = {0};
  matrix_t Z = {0};
  matrix_t X = {0};

  create(4, 4, &A);
  init_matrix(0.0, &A);

  create(4, 4, &B);
  init_matrix(2.0, &B);

  sub_matrix(&A, &B, &Z);

  create(4, 4, &X);

  X.matrix[0][0] = -2.0;
  X.matrix[0][1] = -2.0;
  X.matrix[0][2] = -2.0;
  X.matrix[0][3] = -2.0;
  X.matrix[1][0] = -2.0;
  X.matrix[1][1] = -2.0;
  X.matrix[1][2] = -2.0;
  X.matrix[1][3] = -2.0;
  X.matrix[2][0] = -2.0;
  X.matrix[2][1] = -2.0;
  X.matrix[2][2] = -2.0;
  X.matrix[2][3] = -2.0;
  X.matrix[3][0] = -2.0;
  X.matrix[3][1] = -2.0;
  X.matrix[3][2] = -2.0;
  X.matrix[3][3] = -2.0;
  int res = eq_matrix(&X, &Z);
  remv(&A);
  remv(&B);
  remv(&X);
  remv(&Z);
  ck_assert_int_eq(res, SUCCESS);
}
END_TEST

START_TEST(sub_matrix_03) {
  int res = 0;
  matrix_t A = {0};
  matrix_t B = {0};
  matrix_t Z = {0};
  matrix_t X = {0};

  create(4, 4, &A);
  init_matrix(-3.0, &A);

  create(4, 4, &B);
  init_matrix(5.0, &B);

  sub_matrix(&A, &B, &Z);

  create(4, 4, &X);

  X.matrix[0][0] = -8.0;
  X.matrix[0][1] = -8.0;
  X.matrix[0][2] = -8.0;
  X.matrix[0][3] = -8.0;
  X.matrix[1][0] = -8.0;
  X.matrix[1][1] = -8.0;
  X.matrix[1][2] = -8.0;
  X.matrix[1][3] = -8.0;
  X.matrix[2][0] = -8.0;
  X.matrix[2][1] = -8.0;
  X.matrix[2][2] = -8.0;
  X.matrix[2][3] = -8.0;
  X.matrix[3][0] = -8.0;
  X.matrix[3][1] = -8.0;
  X.matrix[3][2] = -8.0;
  X.matrix[3][3] = -8.0;

  res = eq_matrix(&X, &Z);

  remv(&A);
  remv(&B);
  remv(&X);
  remv(&Z);

  ck_assert_int_eq(res, SUCCESS);
}
END_TEST

START_TEST(sub_matrix_04) {
  int res = 0;
  matrix_t A = {0};
  matrix_t B = {0};
  matrix_t Z = {0};
  matrix_t X = {0};

  create(4, 4, &A);
  init_matrix(5.0, &A);

  create(4, 4, &B);
  init_matrix(-3.0, &B);

  sub_matrix(&A, &B, &Z);

  create(4, 4, &X);

  X.matrix[0][0] = 8.0;
  X.matrix[0][1] = 8.0;
  X.matrix[0][2] = 8.0;
  X.matrix[0][3] = 8.0;
  X.matrix[1][0] = 8.0;
  X.matrix[1][1] = 8.0;
  X.matrix[1][2] = 8.0;
  X.matrix[1][3] = 8.0;
  X.matrix[2][0] = 8.0;
  X.matrix[2][1] = 8.0;
  X.matrix[2][2] = 8.0;
  X.matrix[2][3] = 8.0;
  X.matrix[3][0] = 8.0;
  X.matrix[3][1] = 8.0;
  X.matrix[3][2] = 8.0;
  X.matrix[3][3] = 8.0;

  res = eq_matrix(&X, &Z);

  remv(&A);
  remv(&B);
  remv(&X);
  remv(&Z);

  ck_assert_int_eq(res, SUCCESS);
}
END_TEST

START_TEST(sub_matrix_05) {
  int res = 0;
  matrix_t A = {0};
  matrix_t B = {0};
  matrix_t Z = {0};
  matrix_t X = {0};

  create(4, 2, &A);
  init_matrix(3.0, &A);

  create(4, 2, &B);
  init_matrix(3.0, &B);

  sub_matrix(&A, &B, &Z);

  create(4, 2, &X);

  X.matrix[0][0] = 0.0;
  X.matrix[0][1] = 0.0;
  X.matrix[1][0] = 0.0;
  X.matrix[1][1] = 0.0;
  X.matrix[2][0] = 0.0;
  X.matrix[2][1] = 0.0;
  X.matrix[3][0] = 0.0;
  X.matrix[3][1] = 0.0;

  res = eq_matrix(&X, &Z);

  remv(&A);
  remv(&B);
  remv(&X);
  remv(&Z);

  ck_assert_int_eq(res, SUCCESS);
}
END_TEST

START_TEST(sub_matrix_06) {
  int res = 0;
  matrix_t A = {0};
  matrix_t B = {0};
  matrix_t Z = {0};
  matrix_t X = {0};

  create(4, 2, &A);
  init_matrix(-1.0, &A);

  create(4, 2, &B);
  init_matrix(3.0, &B);

  sub_matrix(&A, &B, &Z);

  create(4, 2, &X);

  X.matrix[0][0] = -4.0;
  X.matrix[0][1] = -4.0;
  X.matrix[1][0] = -4.0;
  X.matrix[1][1] = -4.0;
  X.matrix[2][0] = -4.0;
  X.matrix[2][1] = -4.0;
  X.matrix[3][0] = -4.0;
  X.matrix[3][1] = -4.0;

  res = eq_matrix(&X, &Z);

  remv(&A);
  remv(&B);
  remv(&X);
  remv(&Z);

  ck_assert_int_eq(res, SUCCESS);
}
END_TEST

START_TEST(sub_matrix_07) {
  int res = 0;
  matrix_t A = {0};
  matrix_t B = {0};
  matrix_t Z = {0};
  matrix_t X = {0};

  create(4, 2, &A);
  init_matrix(4.0, &A);

  create(4, 2, &B);
  init_matrix(-2.0, &B);

  sub_matrix(&A, &B, &Z);

  create(4, 2, &X);

  X.matrix[0][0] = 6.0;
  X.matrix[0][1] = 6.0;
  X.matrix[1][0] = 6.0;
  X.matrix[1][1] = 6.0;
  X.matrix[2][0] = 6.0;
  X.matrix[2][1] = 6.0;
  X.matrix[3][0] = 6.0;
  X.matrix[3][1] = 6.0;

  res = eq_matrix(&X, &Z);

  remv(&A);
  remv(&B);
  remv(&X);
  remv(&Z);

  ck_assert_int_eq(res, SUCCESS);
}
END_TEST

START_TEST(sub_matrix_08) {
  int res = 0;
  matrix_t A = {0};
  matrix_t B = {0};
  matrix_t Z = {0};

  create(4, 2, &A);

  create(4, 3, &B);

  res = sub_matrix(&A, &B, &Z);

  remv(&A);
  remv(&B);
  remv(&Z);

  ck_assert_int_eq(res, ERROR_CALC);
}
END_TEST

START_TEST(mult_number_01) {
  int res = 0;
  double number = 3.0;
  matrix_t A = {0};
  matrix_t Z = {0};
  matrix_t X = {0};

  create(4, 4, &A);

  init_matrix(1.0, &A);

  mult_number(&A, number, &Z);

  create(4, 4, &X);

  X.matrix[0][0] = 3.0;
  X.matrix[0][1] = 6.0;
  X.matrix[0][2] = 9.0;
  X.matrix[0][3] = 12.0;
  X.matrix[1][0] = 15.0;
  X.matrix[1][1] = 18.0;
  X.matrix[1][2] = 21.0;
  X.matrix[1][3] = 24.0;
  X.matrix[2][0] = 27.0;
  X.matrix[2][1] = 30.0;
  X.matrix[2][2] = 33.0;
  X.matrix[2][3] = 36.0;
  X.matrix[3][0] = 39.0;
  X.matrix[3][1] = 42.0;
  X.matrix[3][2] = 45.0;
  X.matrix[3][3] = 48.0;

  res = eq_matrix(&X, &Z);

  remv(&A);
  remv(&X);
  remv(&Z);

  ck_assert_int_eq(res, SUCCESS);
}
END_TEST

START_TEST(mult_number_02) {
  int res = 0;
  double number = 5.0;
  matrix_t A = {0};
  matrix_t Z = {0};
  matrix_t X = {0};

  create(4, 4, &A);
  init_matrix(1.0, &A);

  mult_number(&A, number, &Z);

  create(4, 4, &X);

  X.matrix[0][0] = 5.0;
  X.matrix[0][1] = 10.0;
  X.matrix[0][2] = 15.0;
  X.matrix[0][3] = 20.0;
  X.matrix[1][0] = 25.0;
  X.matrix[1][1] = 30.0;
  X.matrix[1][2] = 35.0;
  X.matrix[1][3] = 40.0;
  X.matrix[2][0] = 45.0;
  X.matrix[2][1] = 50.0;
  X.matrix[2][2] = 55.0;
  X.matrix[2][3] = 60.0;
  X.matrix[3][0] = 65.0;
  X.matrix[3][1] = 70.0;
  X.matrix[3][2] = 75.0;
  X.matrix[3][3] = 80.0;

  res = eq_matrix(&X, &Z);

  remv(&A);
  remv(&X);
  remv(&Z);

  ck_assert_int_eq(res, SUCCESS);
}
END_TEST

START_TEST(mult_number_03) {
  int res = 0;
  double number = 2.0;
  matrix_t A = {0};
  matrix_t Z = {0};
  matrix_t X = {0};

  create(4, 2, &A);
  init_matrix(2.0, &A);

  mult_number(&A, number, &Z);
  create(4, 2, &X);

  X.matrix[0][0] = 4.0;
  X.matrix[0][1] = 6.0;
  X.matrix[1][0] = 8.0;
  X.matrix[1][1] = 10.0;
  X.matrix[2][0] = 12.0;
  X.matrix[2][1] = 14.0;
  X.matrix[3][0] = 16.0;
  X.matrix[3][1] = 18.0;

  res = eq_matrix(&X, &Z);
  remv(&A);
  remv(&X);
  remv(&Z);

  ck_assert_int_eq(res, SUCCESS);
}
END_TEST

START_TEST(mult_number_04) {
  int res = 0;
  double number = -2.0;
  matrix_t A = {0};
  matrix_t Z = {0};
  matrix_t X = {0};

  create(4, 2, &A);
  init_matrix(2.0, &A);

  mult_number(&A, number, &Z);

  create(4, 2, &X);

  X.matrix[0][0] = -4.0;
  X.matrix[0][1] = -6.0;
  X.matrix[1][0] = -8.0;
  X.matrix[1][1] = -10.0;
  X.matrix[2][0] = -12.0;
  X.matrix[2][1] = -14.0;
  X.matrix[3][0] = -16.0;
  X.matrix[3][1] = -18.0;

  res = eq_matrix(&X, &Z);

  remv(&A);
  remv(&X);
  remv(&Z);

  ck_assert_int_eq(res, SUCCESS);
}
END_TEST

START_TEST(mult_number_05) {
  int res = 0;
  double number = 2.0;
  matrix_t A = {0};
  matrix_t Z = {0};
  matrix_t X = {0};

  create(1, 1, &A);
  init_matrix(2.0, &A);

  mult_number(&A, number, &Z);

  create(1, 1, &X);

  X.matrix[0][0] = 4.0;

  res = eq_matrix(&X, &Z);

  remv(&A);
  remv(&X);
  remv(&Z);

  ck_assert_int_eq(res, SUCCESS);
}
END_TEST

START_TEST(mult_number_06) {
  int res = 0;
  double number = 2.0;
  matrix_t A = {0};
  matrix_t Z = {0};

  create(0, 0, &A);

  res = mult_number(&A, number, &Z);

  remv(&A);
  remv(&Z);

  ck_assert_int_eq(res, ERROR_INIT);
}
END_TEST

START_TEST(mult_matrix_01) {
  int res = 0;
  matrix_t A = {0};
  matrix_t B = {0};
  matrix_t Z = {0};
  matrix_t X = {0};

  create(4, 4, &A);
  init_matrix(1.0, &A);

  create(4, 4, &B);
  init_matrix(1.0, &B);

  mult_matrix(&A, &B, &Z);

  create(4, 4, &X);

  X.matrix[0][0] = 90.0;
  X.matrix[0][1] = 100.0;
  X.matrix[0][2] = 110.0;
  X.matrix[0][3] = 120.0;
  X.matrix[1][0] = 202.0;
  X.matrix[1][1] = 228.0;
  X.matrix[1][2] = 254.0;
  X.matrix[1][3] = 280.0;
  X.matrix[2][0] = 314.0;
  X.matrix[2][1] = 356.0;
  X.matrix[2][2] = 398.0;
  X.matrix[2][3] = 440.0;
  X.matrix[3][0] = 426.0;
  X.matrix[3][1] = 484.0;
  X.matrix[3][2] = 542.0;
  X.matrix[3][3] = 600.0;

  res = eq_matrix(&X, &Z);

  remv(&A);
  remv(&B);
  remv(&X);
  remv(&Z);

  ck_assert_int_eq(res, SUCCESS);
}
END_TEST

START_TEST(mult_matrix_02) {
  int res = 0;
  matrix_t A = {0};
  matrix_t B = {0};
  matrix_t Z = {0};
  matrix_t X = {0};

  create(4, 4, &A);
  init_matrix(1.0, &A);

  create(4, 4, &B);
  init_matrix(-5.0, &B);

  mult_matrix(&A, &B, &Z);

  create(4, 4, &X);

  X.matrix[0][0] = 30.0;
  X.matrix[0][1] = 40.0;
  X.matrix[0][2] = 50.0;
  X.matrix[0][3] = 60.0;
  X.matrix[1][0] = 46.0;
  X.matrix[1][1] = 72.0;
  X.matrix[1][2] = 98.0;
  X.matrix[1][3] = 124.0;
  X.matrix[2][0] = 62.0;
  X.matrix[2][1] = 104.0;
  X.matrix[2][2] = 146.0;
  X.matrix[2][3] = 188.0;
  X.matrix[3][0] = 78.0;
  X.matrix[3][1] = 136.0;
  X.matrix[3][2] = 194.0;
  X.matrix[3][3] = 252.0;

  res = eq_matrix(&X, &Z);

  remv(&A);
  remv(&B);
  remv(&X);
  remv(&Z);

  ck_assert_int_eq(res, SUCCESS);
}
END_TEST

START_TEST(mult_matrix_03) {
  int res = 0;
  matrix_t A = {0};
  matrix_t B = {0};
  matrix_t Z = {0};
  matrix_t X = {0};

  create(4, 4, &A);
  init_matrix(-5.0, &A);

  create(4, 4, &B);
  init_matrix(1.0, &B);

  mult_matrix(&A, &B, &Z);

  create(4, 4, &X);

  X.matrix[0][0] = -78.0;
  X.matrix[0][1] = -92.0;
  X.matrix[0][2] = -106.0;
  X.matrix[0][3] = -120.0;
  X.matrix[1][0] = 34.0;
  X.matrix[1][1] = 36.0;
  X.matrix[1][2] = 38.0;
  X.matrix[1][3] = 40.0;
  X.matrix[2][0] = 146.0;
  X.matrix[2][1] = 164.0;
  X.matrix[2][2] = 182.0;
  X.matrix[2][3] = 200.0;
  X.matrix[3][0] = 258.0;
  X.matrix[3][1] = 292.0;
  X.matrix[3][2] = 326.0;
  X.matrix[3][3] = 360.0;

  res = eq_matrix(&X, &Z);

  remv(&A);
  remv(&B);
  remv(&X);
  remv(&Z);

  ck_assert_int_eq(res, SUCCESS);
}
END_TEST

START_TEST(mult_matrix_04) {
  int res = 0;
  matrix_t A = {0};
  matrix_t B = {0};
  matrix_t Z = {0};
  matrix_t X = {0};

  create(4, 2, &A);
  init_matrix(-5.0, &A);

  create(2, 4, &B);
  init_matrix(1.0, &B);

  mult_matrix(&A, &B, &Z);

  create(4, 4, &X);

  X.matrix[0][0] = -25.0;
  X.matrix[0][1] = -34.0;
  X.matrix[0][2] = -43.0;
  X.matrix[0][3] = -52.0;
  X.matrix[1][0] = -13.0;
  X.matrix[1][1] = -18.0;
  X.matrix[1][2] = -23.0;
  X.matrix[1][3] = -28.0;
  X.matrix[2][0] = -1.0;
  X.matrix[2][1] = -2.0;
  X.matrix[2][2] = -3.0;
  X.matrix[2][3] = -4.0;
  X.matrix[3][0] = 11.0;
  X.matrix[3][1] = 14.0;
  X.matrix[3][2] = 17.0;
  X.matrix[3][3] = 20.0;

  res = eq_matrix(&X, &Z);

  remv(&A);
  remv(&B);
  remv(&X);
  remv(&Z);

  ck_assert_int_eq(res, SUCCESS);
}
END_TEST

START_TEST(mult_matrix_05) {
  int res = 0;
  matrix_t A = {0};
  matrix_t B = {0};
  matrix_t Z = {0};
  matrix_t X = {0};

  create(1, 1, &A);
  init_matrix(-5.0, &A);

  create(1, 1, &B);
  init_matrix(1.0, &B);

  mult_matrix(&A, &B, &Z);

  create(1, 1, &X);

  X.matrix[0][0] = -5.0;

  res = eq_matrix(&X, &Z);

  remv(&A);
  remv(&B);
  remv(&X);
  remv(&Z);

  ck_assert_int_eq(res, SUCCESS);
}
END_TEST

START_TEST(mult_matrix_06) {
  int res = 0;
  matrix_t A = {0};
  matrix_t B = {0};
  matrix_t Z = {0};

  res = mult_matrix(&A, &B, &Z);

  remv(&A);
  remv(&B);
  remv(&Z);

  ck_assert_int_eq(res, ERROR_INIT);
}
END_TEST

START_TEST(transpose_01) {
  int res = 0;
  matrix_t A = {0};

  res = transpose(&A, &A);
  ck_assert_int_eq(res, ERROR_INIT);
  remv(&A);
}
END_TEST

START_TEST(transpose_02) {
  int res = 0;
  matrix_t A = {0};
  matrix_t B = {0};
  matrix_t D = {0};

  create(4, 5, &A);
  init_matrix(1.0, &A);
  create(4, 5, &D);
  init_matrix(1.0, &D);
  res = transpose(&A, &B);
  init_matrix(1.0, &B);
  res = eq_matrix(&B, &D);
  ck_assert_int_eq(res, FAILURE);
  remv(&A);
  remv(&B);
  remv(&D);
}
END_TEST

START_TEST(transpose_03) {
  int res = 0;
  matrix_t A = {0};
  matrix_t T = {0};
  matrix_t B = {0};

  create(2, 3, &A);
  init_matrix(1.0, &A);
  create(3, 2, &B);
  B.matrix[0][0] = 1.0;
  B.matrix[0][1] = 4.0;
  B.matrix[1][0] = 2.0;
  B.matrix[1][1] = 5.0;
  B.matrix[2][0] = 3.0;
  B.matrix[2][1] = 6.0;

  res = transpose(&A, &T);
  res = eq_matrix(&T, &B);
  ck_assert_int_eq(res, SUCCESS);
  remv(&A);
  remv(&T);
  remv(&B);
}
END_TEST

START_TEST(determinant_01) {
  int res = 0;
  double determ = 0.0;
  matrix_t A = {NULL, 0, 0};

  res = determinant(&A, &determ);
  ck_assert_int_eq(res, ERROR_INIT);
  remv(&A);
}
END_TEST

START_TEST(determinant_02) {
  int res = 0;
  double determ = 0.0;
  matrix_t A = {0};
  create(1, 2, &A);

  res = determinant(&A, &determ);
  ck_assert_int_eq(res, 2);
  remv(&A);
}
END_TEST

START_TEST(determinant_03) {
  double determ = 0.0;
  matrix_t A = {0};

  create(1, 1, &A);
  init_matrix(1.0, &A);

  determinant(&A, &determ);
  ck_assert_double_eq(determ, 1.0);
  remv(&A);
}
END_TEST

START_TEST(determinant_04) {
  double determ = 0.0;
  matrix_t A = {0};

  create(3, 3, &A);
  A.matrix[0][0] = 4000.87;
  A.matrix[0][1] = 454.0;
  A.matrix[0][2] = 4.0;
  A.matrix[1][0] = 2.0;
  A.matrix[1][1] = 1.897;
  A.matrix[1][2] = 3.0;
  A.matrix[2][0] = 0.000087;
  A.matrix[2][1] = 2.5668;
  A.matrix[2][2] = 1.0;

  determinant(&A, &determ);
  ck_assert_double_eq_tol(determ, -24105.996724156, 1e-7);
  remv(&A);
}
END_TEST

START_TEST(determinant_05) {
  double determ = 0.0;
  matrix_t A = {0};

  create(2, 2, &A);
  A.matrix[0][0] = -677700.0;
  A.matrix[0][1] = 654.0;
  A.matrix[1][0] = 2.0;
  A.matrix[1][1] = -0.00001;

  determinant(&A, &determ);
  ck_assert_double_eq(determ, -1301.223);
  remv(&A);
}
END_TEST

START_TEST(determinant_06) {
  double determ = 0.0;
  matrix_t A = {0};

  create(8, 8, &A);
  init_matrix(1.0, &A);
  determinant(&A, &determ);
  ck_assert_double_eq(determ, 0.0);
  remv(&A);
}
END_TEST

START_TEST(calc_complements_01) {
  int res = 0;
  matrix_t A = {0};
  matrix_t Z = {0};
  matrix_t X = {0};

  create(4, 4, &A);
  A.matrix[0][0] = 112.0;
  A.matrix[0][1] = 243.0;
  A.matrix[0][2] = 3.0;
  A.matrix[0][3] = -8.0;
  A.matrix[1][0] = 47.0;
  A.matrix[1][1] = 51.0;
  A.matrix[1][2] = -66.0;
  A.matrix[1][3] = 99.0;
  A.matrix[2][0] = -74.0;
  A.matrix[2][1] = 85.0;
  A.matrix[2][2] = 97.0;
  A.matrix[2][3] = 63.0;
  A.matrix[3][0] = -13.0;
  A.matrix[3][1] = 79.0;
  A.matrix[3][2] = -99.0;
  A.matrix[3][3] = -121.0;

  calc_complements(&A, &Z);
  create(4, 4, &X);

  X.matrix[0][0] = -2879514.0;
  X.matrix[0][1] = -1236631.0;
  X.matrix[0][2] = -1685096.0;
  X.matrix[0][3] = 880697.0;
  X.matrix[1][0] = 1162090.0;
  X.matrix[1][1] = -714015.0;
  X.matrix[1][2] = 4046255.0;
  X.matrix[1][3] = -3901600.0;
  X.matrix[2][0] = 4362897.0;
  X.matrix[2][1] = -2049432.0;
  X.matrix[2][2] = -532912.0;
  X.matrix[2][3] = -1370781.0;
  X.matrix[3][0] = 3412773.0;
  X.matrix[3][1] = -1569493.0;
  X.matrix[3][2] = 3144517.0;
  X.matrix[3][3] = 1284666.0;

  res = eq_matrix(&X, &Z);

  remv(&A);
  remv(&X);
  remv(&Z);

  ck_assert_int_eq(res, SUCCESS);
}
END_TEST

START_TEST(calc_complements_03) {
  int res = 0;
  matrix_t A = {0};
  matrix_t Z = {0};

  create(4, 3, &A);
  A.matrix[0][0] = -1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 7.0;
  A.matrix[1][0] = 1.0;
  A.matrix[1][1] = 0.0;
  A.matrix[1][2] = 0.0;
  A.matrix[2][0] = 47.0;
  A.matrix[2][1] = 13.0;
  A.matrix[2][2] = 17.0;
  A.matrix[3][0] = 22.0;
  A.matrix[3][1] = 7.0;
  A.matrix[3][2] = 1.0;

  res = calc_complements(&A, &Z);

  remv(&A);

  ck_assert_int_eq(res, 2);
}
END_TEST

START_TEST(calc_complements_02) {
  matrix_t A = {0};
  create(1, 1, &A);
  matrix_t res = {0};
  create(1, 1, &res);
  matrix_t result = {0};
  A.matrix[0][0] = 777.777;
  res.matrix[0][0] = 0.0;
  int r = calc_complements(&A, &result);
  ck_assert_double_eq(result.matrix[0][0], res.matrix[0][0]);
  ck_assert_int_eq(r, ERROR_CALC);
  remv(&A);
  remv(&res);
  remv(&result);
}
END_TEST

START_TEST(calc_complements_04) {
  matrix_t A = {0};
  create(2, 2, &A);
  matrix_t result = {0};
  gen_matrix(&A);
  inverse_matrix(&A, &result);
  A.matrix[0][0] = -2.0;
  A.matrix[0][1] = 1.0;
  A.matrix[1][0] = 1.5;
  A.matrix[1][1] = -0.5;
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      ck_assert_double_eq(A.matrix[i][j], result.matrix[i][j]);
    }
  }
  remv(&A);
  remv(&result);
}
END_TEST

START_TEST(calc_complements_05) {
  matrix_t A = {0};
  //  matrix_t B = {0};
  create(2, 2, &A);
  //  create(2, 2, &B);
  matrix_t result = {0};

  double det = -0.38;
  double det_res = 0.0;

  A.matrix[0][0] = 12.0;
  A.matrix[0][1] = -1.0;
  A.matrix[1][0] = 11.5;
  A.matrix[1][1] = -0.99;

  determinant(&A, &det_res);
  ck_assert_double_eq_tol(det, det_res, 1e-6);

  inverse_matrix(&A, &result);
  A.matrix[0][0] = 1 / -0.38 * -0.99;
  A.matrix[0][1] = 1 / -0.38 * 1.0;
  A.matrix[1][0] = 1 / -0.38 * -11.5;
  A.matrix[1][1] = 1 / -0.38 * 12.0;
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      ck_assert_double_eq_tol(A.matrix[i][j], result.matrix[i][j], 1e-6);
    }
  }
  remv(&A);
  remv(&result);
}
END_TEST

START_TEST(calc_complements_06) {
  int res = 0;
  matrix_t A = {0};
  matrix_t Z = {0};
  create(2, 2, &A);
  remv(&A);
  res = calc_complements(&A, &Z);
  ck_assert_int_eq(res, ERROR_INIT);
}
END_TEST

START_TEST(calc_complements_07) {
  matrix_t A = {0};
  create(2, 2, &A);
  matrix_t result = {0};

  A.matrix[0][0] = 4.0;
  A.matrix[0][1] = 6.0;
  A.matrix[1][0] = 9.0;
  A.matrix[1][1] = 11.0;

  calc_complements(&A, &result);

  A.matrix[0][0] = 11.0;
  A.matrix[0][1] = -9.0;
  A.matrix[1][0] = -6.0;
  A.matrix[1][1] = 4.0;
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      ck_assert_double_eq_tol(A.matrix[i][j], result.matrix[i][j], 1e-6);
    }
  }
  remv(&A);
  remv(&result);
}
END_TEST

START_TEST(inverse_matrix_03) {
  int res = 0;
  matrix_t A = {0};
  matrix_t Z = {0};

  create(3, 3, &A);
  gen_matrix(&A);

  res = inverse_matrix(&A, &Z);
  ck_assert_int_eq(res, ERROR_CALC);
  remv(&A);
}
END_TEST

START_TEST(inverse_matrix_04) {
  int res = 0;
  matrix_t A = {0};
  matrix_t Z = {0};

  create(3, 2, &A);
  init_matrix(1.0, &A);

  res = inverse_matrix(&A, &Z);
  ck_assert_int_eq(res, ERROR_CALC);
  remv(&A);
}
END_TEST

START_TEST(inverse_matrix_05) {
  int res = 0;
  matrix_t A = {0};
  matrix_t Z = {0};

  create(3, 3, &A);
  init_matrix(1.0, &A);
  remv(&A);

  res = inverse_matrix(&A, &Z);

  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(inverse_matrix_06) {
  int res = 0;
  matrix_t A = {0};
  matrix_t Z = {0};
  matrix_t X = {0};

  create(3, 3, &A);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 3.0;
  A.matrix[1][0] = 4.0;
  A.matrix[1][1] = 5.0;
  A.matrix[1][2] = 6.0;
  A.matrix[2][0] = 8.0;
  A.matrix[2][1] = 6.0;
  A.matrix[2][2] = 9.0;

  inverse_matrix(&A, &Z);

  create(3, 3, &X);

  X.matrix[0][0] = -3.0 / 5.0;
  X.matrix[0][1] = 0.0;
  X.matrix[0][2] = 1.0 / 5.0;
  ;
  X.matrix[1][0] = -4.0 / 5.0;
  X.matrix[1][1] = 1.0;
  X.matrix[1][2] = -2.0 / 5.0;
  X.matrix[2][0] = 16.0 / 15.0;
  X.matrix[2][1] = -2.0 / 3.0;
  X.matrix[2][2] = 1.0 / 5.0;

  res = eq_matrix(&X, &Z);

  remv(&A);
  remv(&X);
  remv(&Z);

  ck_assert_int_eq(res, SUCCESS);
}
END_TEST

START_TEST(inverse_matrix_08) {
  int res = 0;
  matrix_t A = {0};
  matrix_t Z = {0};
  matrix_t X = {0};

  create(3, 3, &A);
  A.matrix[0][0] = 44.0;
  A.matrix[0][1] = 7.0;
  A.matrix[0][2] = 17.0;
  A.matrix[1][0] = 0.0;
  A.matrix[1][1] = 99.0;
  A.matrix[1][2] = 1.0;
  A.matrix[2][0] = 0.0;
  A.matrix[2][1] = 11.0;
  A.matrix[2][2] = 15.0;

  double det_res = 0.0;
  double det = 64856.0;

  res = determinant(&A, &det_res);

  ck_assert_double_eq(det, det_res);

  inverse_matrix(&A, &Z);

  create(3, 3, &X);

  X.matrix[0][0] = 1.0 / 44.00;
  X.matrix[0][1] = 41.0 / 32428.0;
  X.matrix[0][2] = -419.0 / 16214.0;
  X.matrix[1][0] = 0.0;
  X.matrix[1][1] = 15.0 / 1474.0;
  X.matrix[1][2] = -1.0 / 1474.0;
  X.matrix[2][0] = 0.0;
  X.matrix[2][1] = -1.0 / 134.0;
  X.matrix[2][2] = 9.0 / 134.0;

  res = eq_matrix(&X, &Z);

  remv(&A);
  remv(&X);
  remv(&Z);

  ck_assert_int_eq(res, SUCCESS);
}
END_TEST

START_TEST(inverse_matrix_07) {
  matrix_t A = {0};
  matrix_t B = {0};
  matrix_t Z = {0};
  create(1, 1, &A);
  create(1, 1, &B);
  init_matrix(777.777, &A);
  init_matrix(1.0 / 777.777, &B);
  inverse_matrix(&A, &Z);
  ck_assert_int_eq(eq_matrix(&B, &Z), 1);
  remv(&A);
  remv(&B);
  remv(&Z);
}

END_TEST
START_TEST(create_matrix_1) {
  const int rows = rand() % 100 + 1;
  const int cols = rand() % 100 + 1;
  matrix_t m = {0};
  create(rows, cols, &m);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      m.matrix[i][j] = 0;
      ck_assert_ldouble_eq_tol(0, m.matrix[i][j], 1e-06);
    }
  }
  ck_assert_int_eq(m.rows, rows);
  ck_assert_int_eq(m.columns, cols);
  remv(&m);
}
END_TEST

START_TEST(complements) {
  const int size = 3;
  matrix_t m = {0};
  create(size, size, &m);
  m.matrix[0][0] = 1;
  m.matrix[0][1] = 2;
  m.matrix[0][2] = 3;
  m.matrix[1][1] = 4;
  m.matrix[1][2] = 2;
  m.matrix[2][0] = 5;
  m.matrix[2][1] = 2;
  m.matrix[2][2] = 1;

  matrix_t expected = {0};
  create(size, size, &expected);
  expected.matrix[0][1] = 10;
  expected.matrix[0][2] = -20;
  expected.matrix[1][0] = 4;
  expected.matrix[1][1] = -14;
  expected.matrix[1][2] = 8;
  expected.matrix[2][0] = -8;
  expected.matrix[2][1] = -2;
  expected.matrix[2][2] = 4;

  matrix_t res = {0};
  calc_complements(&m, &res);

  ck_assert_int_eq(eq_matrix(&expected, &res), SUCCESS);
  remv(&m);
  remv(&res);
  remv(&expected);
}
END_TEST

START_TEST(determinant1) {
  const int size = 5;
  matrix_t m = {0};
  create(size, size, &m);

  for (int i = 0; i < size; i++)
    for (int j = 0; j < size; j++) m.matrix[i][j] = j;
  double res = 0;
  int code = determinant(&m, &res);
  ck_assert_double_eq_tol(res, 0, 1e-6);
  ck_assert_int_eq(code, OK);

  remv(&m);
}
END_TEST

START_TEST(determinant2) {
  const int size = 4;
  matrix_t m = {0};
  create(size, size, &m);

  for (int i = 0; i < size; i++)
    for (int j = 0; j < size; j++) m.matrix[i][j] = j + i;

  double res = 0;
  int code = determinant(&m, &res);
  ck_assert_double_eq_tol(res, 0, 1e-6);
  ck_assert_int_eq(code, OK);

  remv(&m);
}
END_TEST

START_TEST(determinant3) {
  const int size = 5;
  matrix_t m = {0};
  create(size, size, &m);
  m.matrix[0][1] = 6;
  m.matrix[0][2] = -2;
  m.matrix[0][3] = -1;
  m.matrix[0][4] = 5;
  m.matrix[1][3] = -9;
  m.matrix[1][4] = -7;
  m.matrix[2][1] = 15;
  m.matrix[2][2] = 35;
  m.matrix[3][1] = -1;
  m.matrix[3][2] = -11;
  m.matrix[3][3] = -2;
  m.matrix[3][4] = 1;
  m.matrix[4][0] = -2;
  m.matrix[4][1] = -2;
  m.matrix[4][2] = 3;
  m.matrix[4][4] = -2;
  double res = 0.0;
  int code = determinant(&m, &res);
  ck_assert_double_eq_tol(res, 2480, 1e-6);
  ck_assert_int_eq(code, OK);

  remv(&m);
}
END_TEST

START_TEST(determinant4) {
  const int size = 3;
  matrix_t m = {0};
  create(size, size, &m);
  m.matrix[0][0] = 2;
  m.matrix[0][1] = 3;
  m.matrix[0][2] = 1;
  m.matrix[1][0] = 7;
  m.matrix[1][1] = 4;
  m.matrix[1][2] = 1;
  m.matrix[2][0] = 9;
  m.matrix[2][1] = -2;
  m.matrix[2][2] = 1;

  double res = 0;
  int code = determinant(&m, &res);
  ck_assert_double_eq_tol(res, -32, 1e-6);
  ck_assert_int_eq(code, OK);

  remv(&m);
}
END_TEST

START_TEST(determinant5) {
  const int size = 2;
  matrix_t m = {0};
  create(size, size, &m);
  m.matrix[0][0] = -5;
  m.matrix[0][1] = -4;
  m.matrix[1][0] = -2;
  m.matrix[1][1] = -3;

  double res = 0;
  int code = determinant(&m, &res);
  ck_assert_double_eq_tol(res, 7, 1e-6);
  ck_assert_int_eq(code, OK);

  remv(&m);
}
END_TEST

START_TEST(calc_compl_2) {
  matrix_t a = {0};
  create(3, 3, &a);
  matrix_t res = {0};
  create(3, 3, &res);
  matrix_t m;
  res.matrix[0][0] = 0;
  res.matrix[0][1] = 10;
  res.matrix[0][2] = -20;
  res.matrix[1][0] = 4;
  res.matrix[1][1] = -14;
  res.matrix[1][2] = 8;
  res.matrix[2][0] = -8;
  res.matrix[2][1] = -2;
  res.matrix[2][2] = 4;

  a.matrix[0][0] = 1;
  a.matrix[0][1] = 2;
  a.matrix[0][2] = 3;
  a.matrix[1][0] = 0;
  a.matrix[1][1] = 4;
  a.matrix[1][2] = 2;
  a.matrix[2][0] = 5;
  a.matrix[2][1] = 2;
  a.matrix[2][2] = 1;
  calc_complements(&a, &m);
  ck_assert_int_eq(eq_matrix(&res, &m), SUCCESS);
  remv(&m);
  remv(&a);
  remv(&res);
}
END_TEST

START_TEST(calc_compl_1) {
  matrix_t A, B, C;
  create(3, 3, &A);
  create(3, 3, &C);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 3.0;
  A.matrix[1][0] = 0.0;
  A.matrix[1][1] = 4.0;
  A.matrix[1][2] = 2.0;
  A.matrix[2][0] = 5.0;
  A.matrix[2][1] = 2.0;
  A.matrix[2][2] = 1.0;
  calc_complements(&A, &B);
  C.matrix[0][0] = 0.0;
  C.matrix[0][1] = 10.0;
  C.matrix[0][2] = -20.0;
  C.matrix[1][0] = 4.0;
  C.matrix[1][1] = -14.0;
  C.matrix[1][2] = 8.0;
  C.matrix[2][0] = -8.0;
  C.matrix[2][1] = -2.0;
  C.matrix[2][2] = 4.0;
  int res = eq_matrix(&B, &C);
  ck_assert_int_eq(res, SUCCESS);
  remv(&A);
  remv(&B);
  remv(&C);
}
END_TEST

START_TEST(inverse_2) {
  matrix_t A, B, C;
  create(3, 3, &A);
  create(3, 3, &C);
  C.matrix[0][0] = 44300.0 / 367429.0;
  C.matrix[0][1] = -236300.0 / 367429.0;
  C.matrix[0][2] = 200360.0 / 367429.0;
  C.matrix[1][0] = 20600.0 / 367429.0;
  C.matrix[1][1] = 56000.0 / 367429.0;
  C.matrix[1][2] = -156483.0 / 367429.0;
  C.matrix[2][0] = 30900.0 / 367429.0;
  C.matrix[2][1] = 84000.0 / 367429.0;
  C.matrix[2][2] = -51010.0 / 367429.0;
  A.matrix[0][0] = 2.8;
  A.matrix[0][1] = 1.3;
  A.matrix[0][2] = 7.01;
  A.matrix[1][0] = -1.03;
  A.matrix[1][1] = -2.3;
  A.matrix[1][2] = 3.01;
  A.matrix[2][0] = 0;
  A.matrix[2][1] = -3;
  A.matrix[2][2] = 2;
  inverse_matrix(&A, &B);
  int res = eq_matrix(&B, &C);
  ck_assert_int_eq(res, 1);
  remv(&A);
  remv(&B);
  remv(&C);
}

END_TEST

START_TEST(inverse_1) {
  matrix_t A, C;
  create(3, 3, &A);
  create(3, 3, &C);
  C.matrix[0][0] = 1.0;
  C.matrix[0][1] = -1.0;
  C.matrix[0][2] = 1.0;
  C.matrix[1][0] = -38.0;
  C.matrix[1][1] = 41.0;
  C.matrix[1][2] = -34.0;
  C.matrix[2][0] = 27.0;
  C.matrix[2][1] = -29.0;
  C.matrix[2][2] = 24.0;
  A.matrix[0][0] = 2.0;
  A.matrix[0][1] = 5.0;
  A.matrix[0][2] = 7.0;
  A.matrix[1][0] = 6.0;
  A.matrix[1][1] = 3.0;
  A.matrix[1][2] = 4.0;
  A.matrix[2][0] = 5.0;
  A.matrix[2][1] = -2.0;
  A.matrix[2][2] = -3.0;
  matrix_t B;
  inverse_matrix(&A, &B);
  int res = eq_matrix(&B, &C);
  ck_assert_int_eq(res, 1);
  remv(&A);
  remv(&B);
  remv(&C);
}
END_TEST

START_TEST(test_normal) {
  matrix_t m = {0};
  matrix_t expected = {0};
  int codec1, codec2;
  codec1 = create(3, 3, &m);
  if (!codec1) codec2 = create(3, 3, &expected);

  if (!codec1 && !codec2) {
    m.matrix[0][0] = 2;
    m.matrix[0][1] = 5;
    m.matrix[0][2] = 7;

    m.matrix[1][0] = 6;
    m.matrix[1][1] = 3;
    m.matrix[1][2] = 4;

    m.matrix[2][0] = 5;
    m.matrix[2][1] = -2;
    m.matrix[2][2] = -3;

    expected.matrix[0][0] = 1;
    expected.matrix[0][1] = -1;
    expected.matrix[0][2] = 1;

    expected.matrix[1][0] = -38;
    expected.matrix[1][1] = 41;
    expected.matrix[1][2] = -34;

    expected.matrix[2][0] = 27;
    expected.matrix[2][1] = -29;
    expected.matrix[2][2] = 24;
    matrix_t result = {0};
    int code = inverse_matrix(&m, &result);

    ck_assert_int_eq(eq_matrix(&result, &expected), SUCCESS);
    ck_assert_int_eq(code, OK);

    remv(&m);
    remv(&result);
    remv(&expected);
  }
}
END_TEST

START_TEST(inverse) {
  const int size = 3;
  matrix_t m = {0};
  create(size, size, &m);

  m.matrix[0][0] = 2;
  m.matrix[0][1] = 5;
  m.matrix[0][2] = 7;
  m.matrix[1][0] = 6;
  m.matrix[1][1] = 3;
  m.matrix[1][2] = 4;
  m.matrix[2][0] = 5;
  m.matrix[2][1] = -2;
  m.matrix[2][2] = -3;

  matrix_t res = {0};
  inverse_matrix(&m, &res);

  matrix_t expected = {0};
  create(size, size, &expected);
  expected.matrix[0][0] = 1;
  expected.matrix[0][1] = -1;
  expected.matrix[0][2] = 1;
  expected.matrix[1][0] = -38;
  expected.matrix[1][1] = 41;
  expected.matrix[1][2] = -34;
  expected.matrix[2][0] = 27;
  expected.matrix[2][1] = -29;
  expected.matrix[2][2] = 24;

  ck_assert_int_eq(eq_matrix(&expected, &res), SUCCESS);

  remv(&expected);
  remv(&res);
  remv(&m);
}
END_TEST

int main(void) {
  Suite *s1 = suite_create("Core");
  TCase *tc1_1 = tcase_create("Core");
  SRunner *sr = srunner_create(s1);
  int nf;
  suite_add_tcase(s1, tc1_1);

  tcase_add_test(tc1_1, calc_determinant_core);

  tcase_add_test(tc1_1, create_01);
  tcase_add_test(tc1_1, create_02);
  tcase_add_test(tc1_1, create_03);
  tcase_add_test(tc1_1, create_04);
  tcase_add_test(tc1_1, create_05);
  tcase_add_test(tc1_1, create_06);
  tcase_add_test(tc1_1, create_07);
  tcase_add_test(tc1_1, create_08);
  tcase_add_test(tc1_1, create_09);

  tcase_add_test(tc1_1, remv_01);
  tcase_add_test(tc1_1, remv_02);
  tcase_add_test(tc1_1, remv_03);
  tcase_add_test(tc1_1, remv_04);
  tcase_add_test(tc1_1, remv_05);

  tcase_add_test(tc1_1, eq_matrix_01);
  tcase_add_test(tc1_1, eq_matrix_02);
  tcase_add_test(tc1_1, eq_matrix_03);
  tcase_add_test(tc1_1, eq_matrix_04);
  tcase_add_test(tc1_1, eq_matrix_05);
  tcase_add_test(tc1_1, eq_matrix_06);

  tcase_add_test(tc1_1, sum_matrix_01);
  tcase_add_test(tc1_1, sum_matrix_02);
  tcase_add_test(tc1_1, sum_matrix_03);
  tcase_add_test(tc1_1, sum_matrix_04);
  tcase_add_test(tc1_1, sum_matrix_05);
  tcase_add_test(tc1_1, sum_matrix_06);
  tcase_add_test(tc1_1, sum_matrix_07);
  tcase_add_test(tc1_1, sum_matrix_08);

  tcase_add_test(tc1_1, sub_matrix_01);
  tcase_add_test(tc1_1, sub_matrix_02);
  tcase_add_test(tc1_1, sub_matrix_03);
  tcase_add_test(tc1_1, sub_matrix_04);
  tcase_add_test(tc1_1, sub_matrix_05);
  tcase_add_test(tc1_1, sub_matrix_06);
  tcase_add_test(tc1_1, sub_matrix_07);
  tcase_add_test(tc1_1, sub_matrix_08);

  tcase_add_test(tc1_1, mult_number_01);
  tcase_add_test(tc1_1, mult_number_02);
  tcase_add_test(tc1_1, mult_number_03);
  tcase_add_test(tc1_1, mult_number_04);
  tcase_add_test(tc1_1, mult_number_05);
  tcase_add_test(tc1_1, mult_number_06);

  tcase_add_test(tc1_1, mult_matrix_01);
  tcase_add_test(tc1_1, mult_matrix_02);
  tcase_add_test(tc1_1, mult_matrix_03);
  tcase_add_test(tc1_1, mult_matrix_04);
  tcase_add_test(tc1_1, mult_matrix_05);
  tcase_add_test(tc1_1, mult_matrix_06);

  tcase_add_test(tc1_1, transpose_01);
  tcase_add_test(tc1_1, transpose_02);
  tcase_add_test(tc1_1, transpose_03);
  tcase_add_test(tc1_1, determinant_01);
  tcase_add_test(tc1_1, determinant_02);
  tcase_add_test(tc1_1, determinant_03);
  tcase_add_test(tc1_1, determinant_04);
  tcase_add_test(tc1_1, determinant_05);
  tcase_add_test(tc1_1, determinant_06);
  tcase_add_test(tc1_1, calc_complements_01);
  tcase_add_test(tc1_1, calc_complements_02);
  tcase_add_test(tc1_1, calc_complements_03);
  tcase_add_test(tc1_1, calc_complements_04);
  tcase_add_test(tc1_1, calc_complements_05);
  tcase_add_test(tc1_1, calc_complements_06);
  tcase_add_test(tc1_1, calc_complements_07);
  tcase_add_test(tc1_1, inverse_matrix_03);
  tcase_add_test(tc1_1, inverse_matrix_04);
  tcase_add_test(tc1_1, inverse_matrix_05);
  tcase_add_test(tc1_1, inverse_matrix_06);
  tcase_add_test(tc1_1, inverse_matrix_07);
  tcase_add_test(tc1_1, inverse_matrix_08);

  tcase_add_loop_test(tc1_1, create_matrix, 0, 100);

  tcase_add_test(tc1_1, complements);

  tcase_add_test(tc1_1, determinant1);
  tcase_add_test(tc1_1, determinant2);
  tcase_add_test(tc1_1, determinant3);
  tcase_add_test(tc1_1, determinant4);
  tcase_add_test(tc1_1, determinant5);

  tcase_add_test(tc1_1, inverse);
  tcase_add_test(tc1_1, inverse_1);
  tcase_add_test(tc1_1, inverse_2);

  tcase_add_test(tc1_1, test_normal);

  tcase_add_test(tc1_1, calc_compl_2);
  tcase_add_test(tc1_1, calc_compl_1);

  tcase_add_test(tc1_1, create_matrix_1);
  tcase_add_test(tc1_1, transpose_matrix);
  tcase_add_test(tc1_1, mult_hardcode);
  tcase_add_test(tc1_1, complements_hardcoded);
  tcase_add_test(tc1_1, determinant_hardcoded);
  tcase_add_test(tc1_1, inverse_hardcoded);
  tcase_add_test(tc1_1, mult_num);
  tcase_add_test(tc1_1, test_eq_matrix);
  tcase_add_test(tc1_1, test_eq_matrix_2);
  tcase_add_test(tc1_1, test_eq_matrix_3);
  tcase_add_test(tc1_1, test_eq_matrix_3);
  tcase_add_test(tc1_1, test_eq_matrix_4);
  tcase_add_test(tc1_1, test_eq_matrix_5);
  tcase_add_test(tc1_1, test_sum_matrix);
  tcase_add_test(tc1_1, test_sum_matrix_2);
  tcase_add_test(tc1_1, test_sub_matrix);
  tcase_add_test(tc1_1, test_sub_matrix_2);
  tcase_add_test(tc1_1, test_mult_number);
  tcase_add_test(tc1_1, test_mult_number_2);
  tcase_add_test(tc1_1, test_mult_matrix_2);
  tcase_add_test(tc1_1, test_mult_matrix_4);
  tcase_add_test(tc1_1, test_mult_matrix_3);
  tcase_add_test(tc1_1, test_transpose);
  tcase_add_test(tc1_1, test_determinant);
  tcase_add_test(tc1_1, test_determinant_2);
  tcase_add_test(tc1_1, test_determinant_3);
  tcase_add_test(tc1_1, test_determinant_5);
  tcase_add_test(tc1_1, test_calc_complements_2);
  tcase_add_test(tc1_1, test_calc_complements_3);
  tcase_add_test(tc1_1, test_inverse_matrix);
  tcase_add_test(tc1_1, test_inverse_matrix_2);
  tcase_add_test(tc1_1, test_inverse_matrix_3);
  tcase_add_test(tc1_1, test_inverse_matrix_4);
  tcase_add_test(tc1_1, eq_matrix_test_1);
  tcase_add_test(tc1_1, eq_matrix_test_2);
  tcase_add_test(tc1_1, eq_matrix_test_3);
  tcase_add_test(tc1_1, eq_matrix_test_4);
  tcase_add_test(tc1_1, sum_matrix_test_1);
  tcase_add_test(tc1_1, sum_matrix_test_2);
  tcase_add_test(tc1_1, sum_matrix_test_3);
  tcase_add_test(tc1_1, sub_matrix_test_1);
  tcase_add_test(tc1_1, sub_matrix_test_2);
  tcase_add_test(tc1_1, sub_matrix_test_3);
  tcase_add_test(tc1_1, mult_number_test_1);
  tcase_add_test(tc1_1, mult_number_test_2);
  tcase_add_test(tc1_1, mult_matrix_test_1);
  tcase_add_test(tc1_1, mult_matrix_test_2);
  tcase_add_test(tc1_1, calc_complements_test_1);
  tcase_add_test(tc1_1, determinant_test_1);
  tcase_add_test(tc1_1, determinant_test_2);
  tcase_add_test(tc1_1, determinant_test_3);
  tcase_add_test(tc1_1, inverse_matrix_test_1);
  tcase_add_test(tc1_1, inverse_matrix_test_2);
  tcase_add_test(tc1_1, create_NULL);

  srunner_run_all(sr, CK_ENV);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}
