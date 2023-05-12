#include "matrix.h"

int create(int rows, int columns, matrix_t* result) {
  if (rows <= 0 || columns <= 0 || result == NULL) return ERROR_INIT;
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

void remv(matrix_t* A) {
  if (A != NULL) {
    for (int i = 0; i < A->rows; i++) free(*(A->matrix + i));
    if (A->matrix != NULL) free(A->matrix);
    A->rows = 0;
    A->columns = 0;
    A->matrix = NULL;
  }
}

int transpose(matrix_t* A, matrix_t* result) {
  if (!is_correct(A) || result == NULL) return ERROR_INIT;
  int res = create(A->columns, A->rows, result);
  if (res == OK) trn(A->matrix, result->matrix, result->columns, result->rows);
  return res;
}

int sum_matrix(matrix_t* A, matrix_t* B, matrix_t* result) {
  if (!is_correct(A) || !is_correct(B) || result == NULL) return ERROR_INIT;
  if (A->rows != B->rows || A->columns != B->columns) return ERROR_CALC;
  int res = create(A->rows, A->columns, result);
  if (res == OK)
    add(A->matrix, B->matrix, result->matrix, result->rows, result->columns);
  return res;
}

int sub_matrix(matrix_t* A, matrix_t* B, matrix_t* result) {
  if (!is_correct(A) || !is_correct(B) || result == NULL) return ERROR_INIT;
  if (A->rows != B->rows || A->columns != B->columns) return ERROR_CALC;
  int res = create(A->rows, A->columns, result);
  if (res == OK)
    sub(A->matrix, B->matrix, result->matrix, result->rows, result->columns);
  return res;
}

int mult_number(matrix_t* A, double number, matrix_t* result) {
  if (!is_correct(A) || result == NULL) return ERROR_INIT;
  if (number == NAN || number == INFINITY) return ERROR_CALC;
  int res = create(A->rows, A->columns, result);
  if (res == OK) nmul(A->matrix, result->matrix, number, A->rows, A->columns);
  return res;
}

int mult_matrix(matrix_t* A, matrix_t* B, matrix_t* result) {
  if (!is_correct(A) || !is_correct(B) || result == NULL) return ERROR_INIT;
  if (A->columns != B->rows) return ERROR_CALC;
  int res = create(A->rows, B->columns, result);
  if (res == OK)
    mul(A->matrix, B->matrix, result->matrix, result->rows, result->columns,
        A->columns);
  return res;
}

int eq_matrix(matrix_t* A, matrix_t* B) {
  int res = FAILURE;
  if (!is_correct(A) || !is_correct(B) || (A->rows != B->rows) ||
      (A->columns != B->columns))
    return res;
  res = is_eq(A->matrix, B->matrix, A->rows, A->columns);
  return res;
}

int determinant(matrix_t* A, double* result) {
  if (!is_correct(A) || result == NULL) {
    return ERROR_INIT;
  } else if (A->rows != A->columns) {
    return ERROR_CALC;
  } else if (A->rows == 1) {
    *result = **A->matrix;
  } else if (A->rows == 2) {
    *result = det_2(A);
  } else if (A->rows == 3) {
    *result = det_3(A);
  } else if (A->rows > 3) {
    *result = matrix_to_triangle(A);
  }
  return OK;
}

int calc_complements(matrix_t* A, matrix_t* result) {
  if (!is_correct(A) || result == NULL) return ERROR_INIT;
  if (A->rows != A->columns) return ERROR_CALC;
  int n = A->rows;
  int res = create(n, n, result);
  if (res != OK) {
    return res;
  }
  matrix_t tmp = {0};
  if (n > 1) {
    res = create(n - 1, n - 1, &tmp);
    if (res != OK) {
      remv(result);
      return res;
    }
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        fm(tmp.matrix, A->matrix, i, j, n - 1);
        double det = 0.0;
        res = determinant(&tmp, &det);
        *(*(result->matrix + i) + j) = det * pow(-1, i + j);
      }
    }
    remv(&tmp);
  } else {
    res = ERROR_CALC;
  }
  return res;
}

int inverse_matrix(matrix_t* A, matrix_t* result) {
  if (!is_correct(A) || result == NULL) return ERROR_INIT;
  if (A->rows != A->columns) return ERROR_CALC;
  double det = 0.0;
  int res = determinant(A, &det);
  if (det == 0) {
    return ERROR_CALC;
  } else if (A->rows > 1) {
    matrix_t tmp_1 = {0};
    matrix_t tmp_2 = {0};
    res = calc_complements(A, &tmp_1);
    if (res != OK) return res;
    res = transpose(&tmp_1, &tmp_2);
    if (res != OK) {
      remv(&tmp_1);
      return res;
    }
    res = mult_number(&tmp_2, 1 / det, result);
    remv(&tmp_1);
    remv(&tmp_2);
  } else if (A->rows == 1) {
    create(1, 1, result);
    **result->matrix = 1 / det;
  }
  return res;
}

double matrix_to_triangle(matrix_t* A) {
  double result = 0.0;
  int rows = A->rows;
  double* nm = (double*)calloc(rows * rows, sizeof(double));
  if (nm == NULL) return ERROR;
  for (int i = 0; i < rows; i++) {
    int t = i * rows;
    for (int j = 0; j < rows; j++) {
      *(nm + t + j) = *(*(A->matrix + i) + j);
    }
  }
  int res = 0;
  for (int k = 0; k < rows - 1; k++) {
    int t = k * rows;
    for (int i = k + 1; i < rows; i++) {
      int count = 0;
      double mag = *(nm + t + k);
      while (fabs(mag) < EPS && count < rows) {
        count++;
        if (count < rows) mag = *(nm + count * rows + k);
      }
      if (count == rows && fabs(mag) < EPS) {
        free(nm);
        return result;
      } else if (count && count < rows) {
        res ^= 0x1;
        for (int j = k; j < rows; j++) {
          double c = *(nm + t + j);
          *(nm + t + j) = *(nm + t + count * rows + j);
          *(nm + t + rows * count + j) = c;
        }
        mag = *(nm + t + k);
      }
      if (fabs(mag) >= MAX_DOUBLE || fabs(mag) <= MIN_DOUBLE) {
        free(nm);
        return result;
      }
      double tmp = *(nm + i * rows + k) / mag;
      for (int j = k; j < rows; j++) {
        *(nm + i * rows + j) =
            *(nm + i * rows + j) + (-1) * tmp * *(nm + t + j);
      }
    }
  }
  result = (res & 0x1) ? -1.0 : 1.0;
  for (int i = 0; i < rows; i++) {
    int t = i * rows;
    result *= *(nm + t + i);
  }
  free(nm);
  return result;
}
