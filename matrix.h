#ifndef MATRIX_H_
#define MATRIX_H_

#include <cmath>
#include <iostream>

#define EPS 1e-07 // precision for double value blablabla
#define MAX_DOUBLE 1.79769e+308
#define MIN_DOUBLE 2.22507e-308

class Matrix {
 public:
  Matrix();
  Matrix(int rows, int cols);
  Matrix(const Matrix &other);
  Matrix(Matrix &&other);
  ~Matrix();

  bool EqMatrix(const Matrix &other) const;
  void SumMatrix(const Matrix &other);
  void SubMatrix(const Matrix &other);
  void MulNumber(const double val);
  void MulMatrix(const Matrix &other);
  Matrix Transpose() const;
  Matrix CalcComplements() const;
  Matrix InverseMatrix() const;
  double Determinant() const;
  Matrix operator+(const Matrix &other) const;
  Matrix operator-(const Matrix &other) const;
  Matrix operator*(const Matrix &other) const;
  Matrix operator*(const double &val) const;
  bool operator==(const Matrix &other) const;
  Matrix &operator=(const Matrix &other);
  Matrix &operator=(Matrix &&other);
  Matrix &operator+=(const Matrix &other);
  Matrix &operator-=(const Matrix &other);
  Matrix &operator*=(const Matrix &other);
  Matrix &operator*=(const double &val);
  double &operator()(const int row, const int col) const;
  int GetRows() const;
  int GetCols() const;
  void SetRows(int rows);
  void SetCols(int cols);

 private:
  int rows_;
  int cols_;
  double **matrix_;
  double *GetArray() const;
  double GetTriangle() const;
  void DeleteLines(Matrix &other, int c_i, int c_j) const;
  void GetMinor(Matrix &other) const;
  void CheckRange(int rows, int cols) const;
  void CheckQuadro() const;
  void Create();
  void Destroy();
  void Copy(const Matrix &other);
};

extern "C" void mul(double **, double **, double **, int, int, int);

#endif  //  MATRIX_H_
