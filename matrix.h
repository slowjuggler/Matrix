#ifndef MATRIX_H_
#define MATRIX_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 1
#define FAILURE 0
#define UNCOR 1
#define ERROR 2
#define OK 0
#define EPS 0.000001

typedef struct {
  double** matrix;
  int rows;
  int columns;
} matrix_t;

int create_matrix(int rows, int columns, matrix_t* result);
void remove_matrix(matrix_t* A);
int sum_matrix(matrix_t* A, matrix_t* B, matrix_t* result);
int sub_matrix(matrix_t* A, matrix_t* B, matrix_t* result);
int mult_number(matrix_t* A, double number, matrix_t* result);
int mult_matrix(matrix_t* A, matrix_t* B, matrix_t* result);
int eq_matrix(matrix_t* A, matrix_t* B);
int transpose(matrix_t* A, matrix_t* result);
int calc_complements(matrix_t* A, matrix_t* result);
int determinant(matrix_t* A, double* result);
int inverse_matrix(matrix_t* A, matrix_t* result);

void add(double**, double**, double**, int, int);
void sub(double**, double**, double**, int, int);
void nmul(double**, double**, double, int, int);
void mul(double**, double**, double**, int, int, int);
void trn(double**, double**, int, int);

inline __attribute__((always_inline)) inline int is_eq(double**, double**, int,
                                                       int);
inline __attribute__((always_inline)) inline void copy(double**, double*,
                                                       size_t);
inline __attribute__((always_inline)) inline int matrix_to_triangle(double*,
                                                                    size_t);
inline __attribute__((always_inline)) inline void fm(double**, double**, int,
                                                     int, int);
/*
inline void add(double** a, double** b, double** c, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      *(*(c + i) + j) = *(*(a + i) + j) + *(*(b + i) + j);
    }
  }
}

inline void sub(double** a, double** b, double** c, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      *(*(c + i) + j) = *(*(a + i) + j) - *(*(b + i) + j);
    }
  }
}

inline void nmul(double** a, double** b, double n, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      *(*(b + i) + j) = n * *(*(a + i) + j);
    }
  }
}

inline void mul(double** a, double** b, double** c, int rows, int cols,
                int num) {
  for (int i = 0; i < rows; i++) {
    for (int k = 0; k < num; k++) {
      double tmp = *(*(a + i) + k);
      for (int j = 0; j < cols; j++) {
        *(*(c + i) + j) += tmp * *(*(b + k) + j);
      }
    }
  }
}
*/
inline void fm(double** p, double** q, int ci, int cj, int n) {
  int ns = n - 1;
  int di = 0;
  for (int i = 0; i < ns; i++) {
    if (i == ci) di = 1;
    int dj = 0;
    for (int j = 0; j < ns; j++) {
      if (j == cj) dj = 1;
      *(*(p + i) + j) = *(*(q + (i + di)) + j + dj);
    }
  }
}
/*
inline void trn(double** a, double** b, int rows, int cols) {
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++) {
      *(*(b + i) + j) = *(*(a + j) + i);
    }
  }
}
*/
inline int is_eq(double** a, double** b, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (*(*(a + i) + j) != *(*(b + i) + j)) return FAILURE;
    }
  }
  return SUCCESS;
}

inline void copy(double** a, double* nm, size_t n) {
  for (size_t i = 0; i < n; i++) {
    int t = i * n;
    for (size_t j = 0; j < n; j++) {
      *(nm + t + j) = *(*(a + i) + j);
    }
  }
}

inline int matrix_to_triangle(double* nm, size_t n) {
  int res = 1;
  int flag_permut = 0;
  for (size_t k = 0; k < n - 1; k++) {
    size_t t = k * n;
    for (size_t i = k + 1; i < n; i++) {
      size_t count = 0;
      double mag = *(nm + t + k);
      while (fabs(mag) < EPS && count < n) {
        count++;
        mag = *(nm + count * n);
      }
      if (count == n && fabs(mag) < EPS) {
        return 0;
      } else if (count) {
        flag_permut++;
        for (size_t j = k; j < n; j++) {
          double c = *(nm + t + j);
          *(nm + t + j) = *(nm + t + count * n + j);
          *(nm + t + n * count + j) = c;
        }
        mag = *(nm + t + k);
      }
      double tmp = *(nm + i * n + k) / mag;
      for (size_t j = k; j < n; j++) {
        *(nm + i * n + j) = *(nm + i * n + j) + (-1) * tmp * *(nm + t + j);
      }
    }
  }
  if (flag_permut & 0x1) res = -1;
  return res;
}

#endif  //  MATRIX_H_
