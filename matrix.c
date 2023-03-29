#include "matrix.h"

int create_matrix(int rows, int columns, matrix_t* result) {
  if (rows <= 0 || columns <= 0 || result == NULL) return ERROR;
  result->rows = rows;
  result->columns = columns;
  result->matrix = (double**)calloc(rows, sizeof(double*));
  if (result->matrix == NULL) {
    return ERROR;
  }
  for (int i = 0; i < result->rows; i++) {
    *(result->matrix + i) = (double*)calloc(result->columns, sizeof(double));
    if (*(result->matrix + i) == NULL) {
      free(result->matrix);
      return ERROR;
    }
  }
  return OK;
}

void remove_matrix(matrix_t* A) {
  if (A != NULL) {
    for (int i = 0; i < A->rows; i++) {
      free(*(A->matrix + i));
    }
    free(A->matrix);
  }
}

int transpose(matrix_t* A, matrix_t* result) {
  int res = create_matrix(A->columns, A->rows, result);
  if (res == OK) trn(A->matrix, result->matrix, A->rows, A->columns);
  return OK;
}

int sum_matrix(matrix_t* A, matrix_t* B, matrix_t* result) {
  if ((A->rows != B->rows) || (A->columns != B->columns)) return ERROR;
  int res = create_matrix(A->rows, A->columns, result);
  if (res == OK)
    add(A->matrix, B->matrix, result->matrix, result->rows, result->columns);
  return OK;
}

int sub_matrix(matrix_t* A, matrix_t* B, matrix_t* result) {
  if ((A->rows != B->rows) || (A->columns != B->columns)) return ERROR;
  int res =  create_matrix(A->rows, A->columns, result);
  if (res == OK)
    sub(A->matrix, B->matrix, result->matrix, result->rows, result->columns);
  return OK;
}

int mult_number(matrix_t* A, double number, matrix_t* result) {
  int res =  create_matrix(A->rows, A->columns, result);
  if (res == OK) nmul(A->matrix, result->matrix, number, A->rows, A->columns);
  return OK;
}

int mult_matrix(matrix_t* A, matrix_t* B, matrix_t* result) {
  if (A->columns != B->rows) return ERROR;
  int res =  create_matrix(A->rows, B->columns, result);
  if (res == OK) {
    int num = A->columns;
    mul(A->matrix, B->matrix, result->matrix, result->rows, result->columns,
        num);
  }
  return OK;
}

int eq_matrix(matrix_t* A, matrix_t* B) {
  if ((A->rows != B->rows) || (A->columns != B->columns)) return ERROR;
  int res = is_eq(A->matrix, B->matrix, A->rows, A->columns);
  return res;
}

int determinant(matrix_t* A, double* result) {
  if (A->rows != A->columns) return ERROR;
  size_t n = A->rows;
  double* nm = (double*)malloc(A->columns * A->rows * sizeof(double));
  if (nm == NULL) return ERROR;
  copy(A->matrix, nm, n);
  *result = 1;
  int ret = matrix_to_triangle(nm, n);
  *result *= ret;
  if (*result) {
    for (size_t i = 0; i < n; i++) {
      size_t t = i * n;
      *result *= *(nm + t + i);
    }
  }
  free(nm);
  return OK;
}

int calc_complements(matrix_t* A, matrix_t* result) {
  if (A->rows != A->columns) return ERROR;
  int n = A->rows;
  int res =  create_matrix(n, n, result);
  if (res != OK) return res;
  matrix_t tmp = {0};
  res =  create_matrix(n - 1, n - 1, &tmp);
  if (res != OK) return res;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      fm(tmp.matrix, A->matrix, i, j, n);
      double det = 0.0;
      determinant(&tmp, &det);
      *(*(result->matrix + i) + j) = det * pow(-1, i + j);
    }
  }
  remove_matrix(&tmp);
  return OK;
}

int inverse_matrix(matrix_t* A, matrix_t* result) {
  if (A->rows != A->columns) return ERROR;
  int n = A->rows;
  int res =  create_matrix(n, n, result);
  if (res != OK) return res;
  double det = 0;
  determinant(A, &det);
  if (det == 0) return ERROR;
  double num = 1.0 / det;
  matrix_t tmp_1 = {0};
  matrix_t tmp_2 = {0};
  res =  create_matrix(n, n, &tmp_1);
  if (res != OK) return res;
  res =  create_matrix(n, n, &tmp_2);
  if (res != OK) {
     remove_matrix(&tmp_1);
    return res;
  }
  res = calc_complements(A, &tmp_1);
  if (res != OK) return res;
  transpose(&tmp_1, &tmp_2);
  if (res != OK) return res;
  mult_number(&tmp_2, num, result);
  if (res != OK) return res;
  remove_matrix(&tmp_1);
  remove_matrix(&tmp_2);
  return OK;
}
