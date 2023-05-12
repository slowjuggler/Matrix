#ifndef MATRIX_H_
#define MATRIX_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 1
#define FAILURE 0
#define ERROR 2
#define OK 0
#define EPS 1e-7
#define MINFOREQ 1e-6
#define ERROR_INIT 1
#define ERROR_CALC 2
#define MAX_DOUBLE 1.79769e+308
#define MIN_DOUBLE 2.22507e-308

typedef struct {
  double** matrix;
  int rows;
  int columns;
} matrix_t;

int create(int rows, int columns, matrix_t* result);
void remv(matrix_t* A);
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
void trn(double**, double**, int, int);
void nmul(double**, double**, double, int, int);
void mul(double**, double**, double**, int, int, int);
double matrix_to_triangle(matrix_t*);

inline __attribute__((always_inline)) int is_eq(double**, double**, int, int);
inline __attribute__((always_inline)) int is_correct(matrix_t*);
inline __attribute__((always_inline)) double det_3(matrix_t*);
inline __attribute__((always_inline)) double det_2(matrix_t*);
inline __attribute__((always_inline)) void fm(double**, double**, int, int,
                                              int);

inline void fm(double** p, double** q, int ci, int cj, int n) {
  int di = 0;
  for (int i = 0; i < n; i++) {
    if (i == ci) di = 1;
    int dj = 0;
    for (int j = 0; j < n; j++) {
      if (j == cj) dj = 1;
      *(*(p + i) + j) = *(*(q + (i + di)) + j + dj);
    }
  }
}

inline int is_eq(double** a, double** b, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (fabs(*(*(b + i) + j) - *(*(a + i) + j)) >= EPS) return FAILURE;
    }
  }
  return SUCCESS;
}

inline int is_correct(matrix_t* A) {
  return A != NULL && A->matrix != NULL && A->rows > 0 && A->columns > 0;
}

inline double det_3(matrix_t* A) {
  return **A->matrix * *(*(A->matrix + 1) + 1) * *(*(A->matrix + 2) + 2) +
         *(*(A->matrix) + 1) * *(*(A->matrix + 1) + 2) * *(*(A->matrix + 2)) +
         *(*(A->matrix + 1)) * *(*(A->matrix + 2) + 1) * *(*(A->matrix) + 2) -
         *(*(A->matrix) + 2) * *(*(A->matrix + 1) + 1) * *(*(A->matrix + 2)) -
         *(*(A->matrix + 1) + 2) * *(*(A->matrix + 2) + 1) * **A->matrix -
         *(*(A->matrix + 1)) * *(*(A->matrix) + 1) * *(*(A->matrix + 2) + 2);
}

inline double det_2(matrix_t* A) {
  return **A->matrix * *(*(A->matrix + 1) + 1) -
         *(*(A->matrix) + 1) * *(*(A->matrix + 1));
}

#endif  //  MATRIX_H_
