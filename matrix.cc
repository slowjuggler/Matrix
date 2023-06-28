#include "matrix.h"

Matrix::Matrix() : rows_(0), cols_(0), matrix_(nullptr) {}

Matrix::Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
  Create();
}

Matrix::Matrix(const Matrix &other)
    : rows_(other.rows_), cols_(other.cols_) {
  if (this != &other) {
    Create();
    Copy(other);
  }
}

Matrix::Matrix(Matrix &&other) : rows_(0), cols_(0), matrix_(nullptr) {
  std::swap(rows_, other.rows_);
  std::swap(cols_, other.cols_);
  std::swap(matrix_, other.matrix_);
}

Matrix::~Matrix() { Destroy(); }

bool Matrix::EqMatrix(const Matrix &other) const {
  if (rows_ != other.rows_ || cols_ != other.cols_) return false;
  for (int i = 0; i < rows_; i++)
    for (int j = 0; j < cols_; j++)
      if (fabs((*this)(i, j) - other(i, j)) > EPS) return false;
  return true;
}

void Matrix::SumMatrix(const Matrix &other) {
  CheckRange(other.rows_, other.cols_);
  for (int i = 0; i < rows_; i++)
    for (int j = 0; j < cols_; j++) (*this)(i, j) += other(i, j);
}

void Matrix::SubMatrix(const Matrix &other) {
  CheckRange(other.rows_, other.cols_);
  for (int i = 0; i < rows_; i++)
    for (int j = 0; j < cols_; j++) (*this)(i, j) -= other(i, j);
}

void Matrix::MulNumber(const double val) {
  for (int i = 0; i < rows_; i++)
    for (int j = 0; j < cols_; j++) (*this)(i, j) *= val;
}

void Matrix::MulMatrix(const Matrix &other) {
  if (cols_ != other.rows_) throw std::out_of_range("WRONG SIZE");
  Matrix res(rows_, other.cols_);
  mul(matrix_, other.matrix_, res.matrix_, rows_, other.cols_, cols_);
  *this = res;
}

Matrix Matrix::Transpose() const {
  Matrix res(cols_, rows_);
  for (int i = 0; i < rows_; i++)
    for (int j = 0; j < cols_; j++) res(j, i) = (*this)(i, j);
  return res;
}

Matrix Matrix::CalcComplements() const {
  CheckQuadro();
  Matrix res(rows_, cols_);
  if (rows_ == 1) {
    res(0, 0) = (*this)(0, 0);
  } else {
    GetMinor(res);
    for (int i = 0; i < res.rows_; i++)
      for (int j = 0; j < res.cols_; j++) res(i, j) *= pow(-1, i + j);
  }
  return res;
}

double Matrix::Determinant() const {
  CheckQuadro();
  return GetTriangle();
}

Matrix Matrix::InverseMatrix() const {
  CheckQuadro();
  double det = Determinant();
  if (!det) throw std::invalid_argument("DETERMINANT IS NULL");
  Matrix res(rows_, cols_);
  if (rows_ == 1) {
    res(0, 0) = 1 / det;
    return res;
  }
  res = CalcComplements().Transpose();
  res.MulNumber(1 / det);
  return res;
}

Matrix Matrix::operator+(const Matrix &other) const {
  Matrix res(*this);
  res.SumMatrix(other);
  return res;
}

Matrix Matrix::operator-(const Matrix &other) const {
  Matrix res(*this);
  res.SubMatrix(other);
  return res;
}

Matrix Matrix::operator*(const Matrix &other) const {
  Matrix res(*this);
  res.MulMatrix(other);
  return res;
}

Matrix Matrix::operator*(const double &val) const {
  Matrix res(*this);
  res.MulNumber(val);
  return res;
}

bool Matrix::operator==(const Matrix &other) const {
  return EqMatrix(other);
}

Matrix &Matrix::operator=(const Matrix &other) {
  if (this != &other) {
    Destroy();
    rows_ = other.rows_;
    cols_ = other.cols_;
    Create();
    Copy(other);
  }
  return *this;
}

Matrix &Matrix::operator=(Matrix &&other) {
  if (this != &other) {
    Destroy();
    std::swap(rows_, other.rows_);
    std::swap(cols_, other.cols_);
    std::swap(matrix_, other.matrix_);
  }
  return *this;
}

Matrix &Matrix::operator+=(const Matrix &other) {
  SumMatrix(other);
  return *this;
}

Matrix &Matrix::operator-=(const Matrix &other) {
  SubMatrix(other);
  return *this;
}

Matrix &Matrix::operator*=(const Matrix &other) {
  MulMatrix(other);
  return *this;
}

Matrix &Matrix::operator*=(const double &val) {
  MulNumber(val);
  return *this;
}

double &Matrix::operator()(const int row, const int col) const {
  if (rows_ <= row || cols_ <= col || row < 0 || col < 0)
    throw std::invalid_argument("WRONG INDEX");
  return matrix_[row][col];
}

int Matrix::GetRows() const { return rows_; }

int Matrix::GetCols() const { return cols_; }

void Matrix::SetRows(int rows) {
  if (rows_ != rows && rows > 0) {
    Matrix res(rows, cols_);
    for (int i = 0; i < rows && i < rows_; i++)
      for (int j = 0; j < cols_; j++) res(i, j) = (*this)(i, j);
    *this = res;
  }
}

void Matrix::SetCols(int cols) {
  if (cols_ != cols && cols > 0) {
    Matrix res(rows_, cols);
    for (int i = 0; i < rows_; i++)
      for (int j = 0; j < cols && j < cols_; j++) res(i, j) = (*this)(i, j);
    *this = res;
  }
}

void Matrix::GetMinor(Matrix &other) const {
  Matrix minor(rows_ - 1, cols_ - 1);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      DeleteLines(minor, i, j);
      other(i, j) = minor.Determinant();
    }
  }
}

void Matrix::DeleteLines(Matrix &other, int c_i, int c_j) const {
  int di = 0;
  for (int i = 0; i < other.rows_; i++) {
    if (i == c_i) di = 1;
    int dj = 0;
    for (int j = 0; j < other.cols_; j++) {
      if (j == c_j) dj = 1;
      other(i, j) = (*this)(i + di, j + dj);
    }
  }
}

double *Matrix::GetArray() const {
  double *arr = (double *)new double[rows_ * cols_]();
  for (int i = 0; i < rows_; i++) {
    int t = i * rows_;
    for (int j = 0; j < cols_; j++) {
      arr[t + j] = (*this)(i, j);
    }
  }
  return arr;
}

double Matrix::GetTriangle() const {
  double result = 0;
  double *arr = GetArray();
  int res = 0;
  for (int k = 0; k < rows_ - 1; k++) {
    int t = k * rows_;
    for (int i = k + 1; i < rows_; i++) {
      int count = 0;
      double mag = arr[t + k];
      while (fabs(mag) < EPS && count < rows_) {
        count++;
        if (count < rows_) mag = arr[count * rows_ + k];
      }
      if (count == rows_ && fabs(mag) < EPS) {
        delete[] arr;
        return result;
      } else if (count && count < rows_) {
        res ^= 0x1;
        for (int j = k; j < rows_; j++) {
          double c = arr[t + j];
          arr[t + j] = arr[t + count * rows_ + j];
          arr[t + rows_ * count + j] = c;
        }
        mag = arr[t + k];
      }
      if (fabs(mag) >= MAX_DOUBLE || fabs(mag) <= MIN_DOUBLE) {
        delete[] arr;
        return result;
      }
      double tmp = arr[i * rows_ + k] / mag;
      for (int j = k; j < rows_; j++) {
        arr[i * rows_ + j] += (-1) * tmp * arr[t + j];
      }
    }
  }
  result = (res & 0x1) ? -1 : 1;
  for (int i = 0; i < rows_; i++) {
    int t = i * rows_;
    result *= arr[t + i];
  }
  delete[] arr;
  return result;
}

void Matrix::Create() {
  if (rows_ < 1 || cols_ < 1) throw std::invalid_argument("WRONG SIZE");
  int count = 0;
  matrix_ = new double *[rows_];
  for (int i = 0; i < rows_; i++) {
    try {
      matrix_[i] = new double[cols_]();
      count++;
    } catch (...) {
      for (int i = 0; i < count; i++) {
        delete[] matrix_[i];
      }
      delete[] matrix_;
      matrix_ = nullptr;
    }
  }
}

void Matrix::Destroy() {
  if (matrix_ != nullptr) {
    for (int i = 0; i < rows_; i++) delete[] matrix_[i];
    delete[] matrix_;
    matrix_ = nullptr;
    rows_ = 0;
    cols_ = 0;
  }
}

void Matrix::CheckRange(int rows, int cols) const {
  if (rows_ != rows || cols_ != cols) throw std::out_of_range("WRONG RANGE");
}

void Matrix::CheckQuadro() const {
  if (rows_ != cols_ || !rows_ || !cols_) throw std::out_of_range("WRONG SIZE");
}

void Matrix::Copy(const Matrix &other) {
  if (other.matrix_ != nullptr && rows_ && cols_) {
    for (int i = 0; i < rows_; i++)
      for (int j = 0; j < cols_; j++) (*this)(i, j) = other(i, j);
  }
}
