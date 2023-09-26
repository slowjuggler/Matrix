#include <gtest/gtest.h>

#include "matrix.h"

#define SIZE 2048	// multiplication matrixes size 
#define SIZE_S 20	// determinant calculation matrix size

TEST(Constructors, DefaultEqual) {
  Matrix matrix;
  EXPECT_EQ(matrix.GetRows(), 0);
  EXPECT_EQ(matrix.GetCols(), 0);
}

TEST(Constructors, DefaultNotEqual) {
  Matrix matrix;
  EXPECT_NE(matrix.GetRows(), 1);
  EXPECT_NE(matrix.GetCols(), 1);
}

TEST(Constructors, ParameterizedSuccess) {
  Matrix matrix(3, 4);
  EXPECT_EQ(matrix.GetRows(), 3);
  EXPECT_EQ(matrix.GetCols(), 4);
}

TEST(Constructors, ParameterizedException) {
  EXPECT_THROW(Matrix matrix(-1, 2), std::invalid_argument);
  EXPECT_THROW(Matrix matrix(2, -1), std::invalid_argument);
  EXPECT_THROW(Matrix matrix(0, 4), std::invalid_argument);
  EXPECT_THROW(Matrix matrix(4, 0), std::invalid_argument);
}

TEST(Constructors, Copy) {
  Matrix matrix1(2, 2);
  matrix1(0, 0) = 1;
  matrix1(0, 1) = 2;
  matrix1(1, 0) = 3;
  matrix1(1, 1) = 4;
  Matrix matrix2(matrix1);
  EXPECT_EQ(matrix2.GetRows(), 2);
  EXPECT_EQ(matrix2.GetCols(), 2);
  EXPECT_EQ(matrix2.EqMatrix(matrix1), true);
  for (int i = 0; i < matrix2.GetRows(); i++) {
    for (int j = 0; j < matrix2.GetCols(); j++) {
      EXPECT_EQ(matrix2(i, j), matrix1(i, j));
    }
  }
}

TEST(Constructors, Move) {
  Matrix matrix1(2, 2);
  matrix1(0, 0) = 1;
  matrix1(0, 1) = 2;
  matrix1(1, 0) = 3;
  matrix1(1, 1) = 4;
  Matrix matrix2(matrix1);
  Matrix matrix3(std::move(matrix1));
  EXPECT_EQ(matrix3.GetRows(), 2);
  EXPECT_EQ(matrix3.GetCols(), 2);
  EXPECT_EQ(matrix1.GetRows(), 0);
  EXPECT_EQ(matrix1.GetCols(), 0);
  EXPECT_EQ(matrix3.EqMatrix(matrix2), true);
  for (int i = 0; i < matrix3.GetRows(); i++) {
    for (int j = 0; j < matrix3.GetCols(); j++) {
      EXPECT_EQ(matrix3(i, j), matrix2(i, j));
    }
  }
}

TEST(Getters, GetRows_cols) {
  Matrix mtr(2, 3);
  EXPECT_EQ(mtr.GetRows(), 2);
  EXPECT_EQ(mtr.GetCols(), 3);
}

TEST(Setters, SetRowsNullValue1) {
  Matrix matrix(2, 2);
  matrix.SetRows(3);
  EXPECT_EQ(matrix.GetRows(), 3);
  EXPECT_EQ(matrix.GetCols(), 2);
  for (int i = 0; i < matrix.GetRows(); i++) {
    for (int j = 0; j < matrix.GetCols(); j++) {
      EXPECT_EQ(matrix(i, j), 0);
    }
  }
}

TEST(Setters, SetRowsNullValue2) {
  Matrix matrix(3, 3);
  matrix.SetRows(1);
  EXPECT_EQ(matrix.GetRows(), 1);
  EXPECT_EQ(matrix.GetCols(), 3);
  for (int i = 0; i < matrix.GetRows(); i++) {
    for (int j = 0; j < matrix.GetCols(); j++) {
      EXPECT_EQ(matrix(i, j), 0);
    }
  }
}

TEST(Setters, SetRowsNotNullValue1) {
  Matrix matrix1(2, 2);
  matrix1(0, 0) = 1;
  matrix1(0, 1) = 2;
  matrix1(1, 0) = 3;
  matrix1(1, 1) = 4;

  Matrix matrix2(3, 2);
  matrix2(0, 0) = 1;
  matrix2(0, 1) = 2;
  matrix2(1, 0) = 3;
  matrix2(1, 1) = 4;
  matrix2(2, 0) = 0;
  matrix2(2, 1) = 0;

  matrix1.SetRows(3);
  EXPECT_EQ(matrix1.GetRows(), 3);
  EXPECT_EQ(matrix1.GetCols(), 2);
  for (int i = 0; i < matrix1.GetRows(); i++) {
    for (int j = 0; j < matrix1.GetCols(); j++) {
      EXPECT_EQ(matrix1(i, j), matrix2(i, j));
    }
  }
}

TEST(Setters, SetRowsNotNullValue2) {
  Matrix matrix1(3, 3);
  matrix1(0, 0) = 1;
  matrix1(0, 1) = 2;
  matrix1(0, 2) = 3;
  matrix1(1, 0) = 4;
  matrix1(1, 1) = 5;
  matrix1(1, 2) = 6;
  matrix1(2, 0) = 7;
  matrix1(2, 1) = 8;
  matrix1(2, 2) = 9;

  Matrix matrix2(2, 3);
  matrix2(0, 0) = 1;
  matrix2(0, 1) = 2;
  matrix2(0, 2) = 3;
  matrix2(1, 0) = 4;
  matrix2(1, 1) = 5;
  matrix2(1, 2) = 6;

  matrix1.SetRows(2);
  EXPECT_EQ(matrix1.GetRows(), 2);
  EXPECT_EQ(matrix1.GetCols(), 3);
  for (int i = 0; i < matrix1.GetRows(); i++) {
    for (int j = 0; j < matrix1.GetCols(); j++) {
      EXPECT_EQ(matrix1(i, j), matrix2(i, j));
    }
  }
}

TEST(Setters, SetColsNullValue1) {
  Matrix matrix(2, 2);
  matrix.SetCols(3);
  EXPECT_EQ(matrix.GetRows(), 2);
  EXPECT_EQ(matrix.GetCols(), 3);
  for (int i = 0; i < matrix.GetRows(); i++) {
    for (int j = 0; j < matrix.GetCols(); j++) {
      EXPECT_EQ(matrix(i, j), 0);
    }
  }
}

TEST(Setters, SetColsNullValue2) {
  Matrix matrix(3, 3);
  matrix.SetCols(1);
  EXPECT_EQ(matrix.GetRows(), 3);
  EXPECT_EQ(matrix.GetCols(), 1);
  for (int i = 0; i < matrix.GetRows(); i++) {
    for (int j = 0; j < matrix.GetCols(); j++) {
      EXPECT_EQ(matrix(i, j), 0);
    }
  }
}

TEST(Setters, SetColsNotNullValue1) {
  Matrix matrix1(2, 2);
  matrix1(0, 0) = 1;
  matrix1(0, 1) = 2;
  matrix1(1, 0) = 3;
  matrix1(1, 1) = 4;

  Matrix matrix2(2, 3);
  matrix2(0, 0) = 1;
  matrix2(0, 1) = 2;
  matrix2(0, 2) = 0;
  matrix2(1, 0) = 3;
  matrix2(1, 1) = 4;
  matrix2(1, 2) = 0;

  matrix1.SetCols(3);
  EXPECT_EQ(matrix1.GetRows(), 2);
  EXPECT_EQ(matrix1.GetCols(), 3);
  for (int i = 0; i < matrix1.GetRows(); i++) {
    for (int j = 0; j < matrix1.GetCols(); j++) {
      EXPECT_EQ(matrix1(i, j), matrix2(i, j));
    }
  }
}

TEST(Setters, SetColsNotNullValue2) {
  Matrix matrix1(3, 3);
  matrix1(0, 0) = 1;
  matrix1(0, 1) = 2;
  matrix1(0, 2) = 3;
  matrix1(1, 0) = 4;
  matrix1(1, 1) = 5;
  matrix1(1, 2) = 6;
  matrix1(2, 0) = 7;
  matrix1(2, 1) = 8;
  matrix1(2, 2) = 9;

  Matrix matrix2(3, 2);
  matrix2(0, 0) = 1;
  matrix2(0, 1) = 2;
  matrix2(1, 0) = 4;
  matrix2(1, 1) = 5;
  matrix2(2, 0) = 7;
  matrix2(2, 1) = 8;

  matrix1.SetCols(2);
  EXPECT_EQ(matrix1.GetRows(), 3);
  EXPECT_EQ(matrix1.GetCols(), 2);
  for (int i = 0; i < matrix1.GetRows(); i++) {
    for (int j = 0; j < matrix1.GetCols(); j++) {
      EXPECT_EQ(matrix1(i, j), matrix2(i, j));
    }
  }
}

TEST(Methods, EqMatrixSuccess) {
  Matrix matrix1(3, 3);
  Matrix matrix2(3, 3);
  matrix1(0, 0) = 1.1;
  matrix2(0, 0) = 1.1;
  matrix1(0, 1) = 1.00000001;
  matrix2(0, 1) = 1.00000002;
  EXPECT_TRUE(matrix2.EqMatrix(matrix1));
}

TEST(Methods, EqMatrixFailure) {
  Matrix matrix1(3, 3);
  Matrix matrix2(3, 3);
  Matrix matrix3(2, 3);
  Matrix matrix4(3, 3);
  matrix1(0, 0) = 1.1;
  matrix2(0, 0) = 1.2;
  EXPECT_FALSE(matrix2.EqMatrix(matrix1));
  EXPECT_FALSE(matrix4.EqMatrix(matrix3));
}

TEST(Methods, SumMatrixSuccess) {
  Matrix matrix1(3, 3);
  Matrix matrix2(3, 3);
  matrix2(0, 0) = 5;
  matrix1.SumMatrix(matrix2);
  EXPECT_TRUE(matrix1.EqMatrix(matrix2));
}

TEST(Methods, SumMatrixFailure) {
  Matrix matrix1(3, 2);
  Matrix matrix2(3, 3);
  EXPECT_THROW(matrix1.SumMatrix(matrix2), std::out_of_range);
}

TEST(Methods, SubMatrixSuccess) {
  Matrix matrix1(3, 3);
  Matrix matrix2(3, 3);
  matrix2(0, 0) = 5;
  matrix1.SubMatrix(matrix2);
  matrix2(0, 0) = -5;
  EXPECT_TRUE(matrix1.EqMatrix(matrix2));
}

TEST(Methods, SubMatrixFailure) {
  Matrix matrix1(3, 2);
  Matrix matrix2(3, 3);
  EXPECT_THROW(matrix1.SubMatrix(matrix2), std::out_of_range);
}

TEST(Methods, MulNumber) {
  Matrix matrix1(2, 2);
  double num = 2.1;
  matrix1(0, 0) = 1;
  matrix1.MulNumber(num);
  EXPECT_EQ(matrix1(0, 0), 2.1);
}

TEST(Methods, MulMatrixSuccess) {
  Matrix matrix1(3, 3);
  Matrix matrix2(3, 3);
  Matrix matrix3(3, 3);
  int count = 1;

  for (int i = 0; i < matrix1.GetRows(); i++) {
    for (int j = 0; j < matrix1.GetCols(); j++) {
      matrix1(i, j) = count;
      matrix2(i, j) = count;
      count++;
    }
  }

  matrix3(0, 0) = 30;
  matrix3(0, 1) = 36;
  matrix3(0, 2) = 42;
  matrix3(1, 0) = 66;
  matrix3(1, 1) = 81;
  matrix3(1, 2) = 96;
  matrix3(2, 0) = 102;
  matrix3(2, 1) = 126;
  matrix3(2, 2) = 150;

  matrix1.MulMatrix(matrix2);
  EXPECT_TRUE(matrix1.EqMatrix(matrix3));
}

TEST(Methods, MulBigMatrixSuccess) {	// Big matrixes multiplication test
  Matrix matrix1(SIZE, SIZE);
  Matrix matrix2(SIZE, SIZE);
  double delta_1 = 0.0077;
  double delta_2 = 0.0033;
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++, delta_1 += 0.001, delta_2 += 0.001) {
      matrix1(i, j) = delta_1;
      matrix2(i, j) = delta_2;
    }
  }
  matrix1.MulMatrix(matrix2);
  EXPECT_TRUE(fabs(matrix1(0, 0) - 5892829.8162417) < EPS);
}

TEST(Methods, MulMatrixFailure) {
  Matrix matrix1(3, 2);
  Matrix matrix2(3, 3);
  EXPECT_THROW(matrix1.MulMatrix(matrix2), std::out_of_range);
}

TEST(Methods, Transpose) {
  Matrix matrix1(3, 2);
  Matrix matrix2(2, 3);

  matrix1(0, 0) = 1;
  matrix1(0, 1) = 2;
  matrix1(1, 0) = 3;
  matrix1(1, 1) = 4;
  matrix1(2, 0) = 5;
  matrix1(2, 1) = 6;

  matrix2(0, 0) = 1;
  matrix2(0, 1) = 3;
  matrix2(0, 2) = 5;
  matrix2(1, 0) = 2;
  matrix2(1, 1) = 4;
  matrix2(1, 2) = 6;
  Matrix matrix3 = matrix1.Transpose();
  EXPECT_TRUE(matrix3.EqMatrix(matrix2));
}

TEST(Methods, Determinant) {
  Matrix matrix1(4, 4);
  matrix1(0, 0) = 9;
  matrix1(0, 1) = 2;
  matrix1(0, 2) = 2;
  matrix1(0, 3) = 4;
  matrix1(1, 0) = 3;
  matrix1(1, 1) = 4;
  matrix1(1, 2) = 4;
  matrix1(1, 3) = 4;
  matrix1(2, 0) = 4;
  matrix1(2, 1) = 4;
  matrix1(2, 2) = 9;
  matrix1(2, 3) = 9;
  matrix1(3, 0) = 1;
  matrix1(3, 1) = 1;
  matrix1(3, 2) = 5;
  matrix1(3, 3) = 1;
  EXPECT_EQ(matrix1.Determinant(), -578);
  Matrix matrix2(1, 1);
  matrix2(0, 0) = 10;
  EXPECT_EQ(matrix2.Determinant(), 10);
  Matrix matrix3(2, 2);
  matrix3(0, 0) = 1.1;
  matrix3(0, 1) = 3.5;
  matrix3(1, 0) = -2;
  matrix3(1, 1) = 4;
  EXPECT_DOUBLE_EQ(matrix3.Determinant(), 11.4);
}

TEST(Methods, DeterminantDop) {
  Matrix matrix1(4, 4);
  matrix1(0, 0) = 1;
  matrix1(0, 1) = 2;
  matrix1(0, 2) = 3;
  matrix1(0, 3) = 4;
  matrix1(1, 0) = 1;
  matrix1(1, 1) = 2;
  matrix1(1, 2) = 5;
  matrix1(1, 3) = 7;
  matrix1(2, 0) = 1;
  matrix1(2, 1) = 0;
  matrix1(2, 2) = 6;
  matrix1(2, 3) = 8;
  matrix1(3, 0) = 1;
  matrix1(3, 1) = 0;
  matrix1(3, 2) = 6;
  matrix1(3, 3) = 6;
  EXPECT_EQ(matrix1.Determinant(), -8);
}

TEST(Methods, DeterminantBig) {
  Matrix matrix1(SIZE_S, SIZE_S);
  double delta_1 = 0.0077;
  for (int i = 0; i < SIZE_S; i++) {
    for (int j = 0; j < SIZE_S; j++, delta_1 += 0.001) {
      matrix1(i, j) = delta_1;
    }
  }
  matrix1(0, 0) = -1.8;
  matrix1(0, 1) = 2.3;
  matrix1(0, 2) = 3.7;
  matrix1(0, 3) = 4.7;
  matrix1(1, 0) = -1.1;
  matrix1(1, 1) = 2.2;
  matrix1(1, 2) = 5.9;
  matrix1(1, 3) = 7.9;
  matrix1(2, 0) = 1.5;
  matrix1(2, 1) = 0.01;
  matrix1(2, 2) = 6.4;
  matrix1(2, 3) = -8.1;
  matrix1(3, 0) = 1.2;
  matrix1(3, 1) = -0.9;
  matrix1(3, 2) = 6.1;
  matrix1(1, 3) = 6.7;
  matrix1(0, 0) = -1.8;
  matrix1(0, 1) = 2.3;
  matrix1(0, 2) = 3.7;
  matrix1(0, 3) = 4.7;
  matrix1(1, 0) = -1.1;
  matrix1(1, 1) = 2.2;
  matrix1(1, 2) = 5.9;
  matrix1(1, 3) = 7.9;
  matrix1(2, 0) = 1.5;
  matrix1(2, 1) = 0.01;
  matrix1(2, 2) = 6.4;
  matrix1(2, 3) = -8.1;
  matrix1(3, 0) = 1.2;
  matrix1(3, 1) = -0.9;
  matrix1(3, 2) = 6.1;
  matrix1(3, 3) = 6.7;
  matrix1(4, 4) = -6.1;
  matrix1(5, 5) = 6.7;
  matrix1(6, 6) = 6.1;
  matrix1(7, 7) = 6.7;
  matrix1(8, 8) = -6.7;
  matrix1(9, 9) = 6.1;
  matrix1(10, 10) = 6.7;
  matrix1(11, 11) = 6.1;
  matrix1(12, 12) = 6.7;
  matrix1(13, 13) = -6.7;
  matrix1(14, 14) = 1.1;
  matrix1(15, 15) = 2.1;
  matrix1(16, 16) = 7.7;
  matrix1(17, 17) = -0.7;
  matrix1(18, 18) = 0.01;
  matrix1(19, 19) = 1e-10;
  long double res = fabs(matrix1.Determinant() / 1.74485e+10);
  EXPECT_TRUE(res >= 1.0 - 1e-05 && res <= 1.0 + 1e-05);
}

TEST(Methods, DeterminantExcept) {
  Matrix matrix1(3, 2);
  EXPECT_THROW(matrix1.Determinant(), std::out_of_range);
}

TEST(Methods, CalcComplements) {
  Matrix matrix1(3, 3);
  Matrix matrix2(3, 3);
  matrix1(0, 0) = 1;
  matrix1(0, 1) = 2;
  matrix1(0, 2) = 3;
  matrix1(1, 0) = 0;
  matrix1(1, 1) = 4;
  matrix1(1, 2) = 2;
  matrix1(2, 0) = 5;
  matrix1(2, 1) = 2;
  matrix1(2, 2) = 1;

  matrix2(0, 0) = 0;
  matrix2(0, 1) = 10;
  matrix2(0, 2) = -20;
  matrix2(1, 0) = 4;
  matrix2(1, 1) = -14;
  matrix2(1, 2) = 8;
  matrix2(2, 0) = -8;
  matrix2(2, 1) = -2;
  matrix2(2, 2) = 4;

  Matrix matrix3(matrix1.CalcComplements());
  EXPECT_TRUE(matrix3.EqMatrix(matrix2));

  matrix1.SetCols(1);
  matrix1.SetRows(1);
  matrix2.SetCols(1);
  matrix2.SetRows(1);
  matrix1(0, 0) = 1;
  matrix2(0, 0) = 1;
  Matrix matrix4(matrix1.CalcComplements());
  EXPECT_TRUE(matrix4.EqMatrix(matrix2));
}

TEST(Methods, CalcComplementsExcept) {
  Matrix matrix1(5, 4);
  EXPECT_THROW(matrix1.CalcComplements(), std::out_of_range);
}

TEST(Methods, InverseMatrix) {
  Matrix matrix1(3, 3);
  Matrix matrix2(3, 3);
  matrix1(0, 0) = 2;
  matrix1(0, 1) = 6;
  matrix1(0, 2) = 5;
  matrix1(1, 0) = 5;
  matrix1(1, 1) = 3;
  matrix1(1, 2) = -2;
  matrix1(2, 0) = 7;
  matrix1(2, 1) = 4;
  matrix1(2, 2) = -3;

  matrix2(0, 0) = 1;
  matrix2(0, 1) = -38;
  matrix2(0, 2) = 27;
  matrix2(1, 0) = -1;
  matrix2(1, 1) = 41;
  matrix2(1, 2) = -29;
  matrix2(2, 0) = 1;
  matrix2(2, 1) = -34;
  matrix2(2, 2) = 24;

  Matrix matrix3(matrix1.InverseMatrix());
  EXPECT_TRUE(matrix2.EqMatrix(matrix2));
}

TEST(Methods, InverseMatrixOne) {
  Matrix matrix1(1, 1);
  Matrix matrix2(1, 1);
  matrix1(0, 0) = 2.;
  matrix2(0, 0) = 1. / 2.;
  Matrix matrix3(matrix1.InverseMatrix());
  EXPECT_TRUE(matrix3.EqMatrix(matrix2));
}

TEST(Methods, InverseMatrixDop) {
  Matrix matrix1(3, 3);
  matrix1(0, 0) = 4;
  matrix1(0, 1) = -2;
  matrix1(0, 2) = 1;
  matrix1(1, 0) = 1;
  matrix1(1, 1) = 6;
  matrix1(1, 2) = -2;
  matrix1(2, 0) = 1;
  matrix1(2, 1) = 0;
  matrix1(2, 2) = 0;
  matrix1 = matrix1.InverseMatrix();
  EXPECT_EQ(matrix1(0, 1), 0);
  EXPECT_EQ(matrix1(0, 2), 1);
  EXPECT_EQ(matrix1(1, 0), 1);
  EXPECT_EQ(matrix1(2, 0), 3);
  EXPECT_EQ(matrix1(2, 1), 1);
  EXPECT_EQ(matrix1(2, 2), -13);
}

TEST(Methods, InverseMatrixExcept) {
  Matrix matrix1(3, 3);

  matrix1(0, 0) = 1;
  matrix1(0, 1) = 4;
  matrix1(0, 2) = 1;
  matrix1(1, 0) = 3;
  matrix1(1, 1) = 7;
  matrix1(1, 2) = 2;
  matrix1(2, 0) = 3;
  matrix1(2, 1) = 2;
  matrix1(2, 2) = 1;
  EXPECT_EQ(matrix1.Determinant(), 0);
  EXPECT_THROW(matrix1.InverseMatrix(), std::invalid_argument);
}

TEST(Methods, InverseMatrixExceptDop) {
  Matrix matrix1(5, 4);
  EXPECT_THROW(matrix1.CalcComplements(), std::out_of_range);
}

TEST(Operators, OperatorSum) {
  Matrix matrix1(3, 3);
  Matrix matrix2(3, 3);
  matrix2(0, 0) = 5;
  Matrix matrix3 = matrix1 + matrix2;
  EXPECT_TRUE(matrix3.EqMatrix(matrix2));
}

TEST(Operators, OperatorSumExcept) {
  Matrix matrix1(5, 5);
  Matrix matrix2(6, 5);
  EXPECT_THROW(matrix1 + matrix2, std::out_of_range);
}

TEST(Operators, OperatorSub) {
  Matrix matrix1(5, 5);
  Matrix matrix2(5, 5);
  matrix1(1, 1) = 6.6;
  matrix2(1, 1) = 1.1;
  Matrix matrix3 = matrix1 - matrix2;
  EXPECT_EQ(matrix3(1, 1), 5.5);
}

TEST(Operators, OperatorSubExcept) {
  Matrix matrix1(5, 5);
  Matrix matrix2(6, 5);
  EXPECT_THROW(matrix1 - matrix2, std::out_of_range);
}

TEST(Operators, OperatorMulNum) {
  Matrix matrix1(3, 3);
  double num = 2.2;
  matrix1(1, 1) = 2;
  Matrix matrix2 = matrix1 * num;
  EXPECT_DOUBLE_EQ(matrix2(1, 1), 4.4);
}

TEST(Operators, OperatorMulMatrix) {
  Matrix matrix1(3, 2);
  Matrix matrix2(2, 3);
  Matrix matrix3(3, 3);

  matrix1(0, 0) = 1;
  matrix1(1, 0) = 2;
  matrix1(2, 0) = 3;
  matrix1(0, 1) = 4;
  matrix1(1, 1) = 5;
  matrix1(2, 1) = 6;

  matrix2(0, 0) = 1;
  matrix2(0, 1) = -1;
  matrix2(0, 2) = 1;
  matrix2(1, 0) = 2;
  matrix2(1, 1) = 3;
  matrix2(1, 2) = 4;

  matrix3(0, 0) = 9;
  matrix3(0, 1) = 11;
  matrix3(0, 2) = 17;
  matrix3(1, 0) = 12;
  matrix3(1, 1) = 13;
  matrix3(1, 2) = 22;
  matrix3(2, 0) = 15;
  matrix3(2, 1) = 15;
  matrix3(2, 2) = 27;

  Matrix matrix4 = matrix1 * matrix2;
  EXPECT_TRUE(matrix4.EqMatrix(matrix3));
}

TEST(Operators, OperatorEq) {
  Matrix matrix1(2, 3);
  Matrix matrix2(2, 2);
  EXPECT_EQ(matrix1 == matrix2, false);
  matrix2.SetCols(3);
  matrix1(1, 1) = 1.1;
  matrix2(1, 1) = 1.1;
  EXPECT_EQ(matrix1 == matrix2, true);
  matrix2(1, 2) = 1.1;
  EXPECT_EQ(matrix1 == matrix2, false);
}

TEST(Operators, OperatorAssign) {
  Matrix matrix1(2, 3);
  Matrix matrix2(2, 5);
  matrix1 = matrix2;
  EXPECT_EQ(matrix1 == matrix2, true);
}

TEST(Operators, OperatorSumAssign) {
  Matrix matrix1(5, 5);
  matrix1(1, 1) = 1.1;
  Matrix matrix2(5, 5);
  matrix2 += matrix1;
  EXPECT_TRUE(matrix1.EqMatrix(matrix2));
}

TEST(Operators, OperatorSubAssign) {
  Matrix matrix1(5, 5);
  matrix1(1, 1) = 1.1;
  Matrix matrix2(5, 5);
  matrix2 -= matrix1;
  matrix1(1, 1) = -1.1;
  EXPECT_TRUE(matrix1.EqMatrix(matrix2));
}

TEST(Operators, OperatorMulEqMatr) {
  Matrix matrix1(3, 2);
  Matrix matrix2(2, 3);
  matrix1(1, 1) = 1.1;
  matrix2(1, 1) = 2;
  matrix1 *= matrix2;
  EXPECT_DOUBLE_EQ(matrix1(1, 1), 2.2);
}

TEST(functionalTest, OperatorMulEqExcept) {
  Matrix matrix1(2, 3);
  Matrix matrix2(2, 2);
  matrix1(1, 1) = 1.1;
  matrix2(1, 1) = 2.2;
  EXPECT_ANY_THROW(matrix1 *= matrix2);
}

TEST(Operators, OperatorMulEqNum) {
  Matrix matrix1(3, 2);
  double num = 2;
  matrix1(1, 1) = 1.1;
  matrix1 *= num;
  EXPECT_DOUBLE_EQ(matrix1(1, 1), 2.2);
}

TEST(assignmentOperator, OperatorBracket) {
  Matrix matrix1(2, 3);
  matrix1(1, 1) = 3;
  EXPECT_EQ(matrix1(1, 1), 3);
}

TEST(functionalFuncTest, OperatorBracketExcept) {
  Matrix matrix1(1, 1);
  EXPECT_THROW(matrix1(5, 0) = 5, std::invalid_argument);
}

TEST(functionalFuncTest, OperatorBracketExceptDop) {
  Matrix matrix1(3, 3);
  matrix1(1, 1) = 1;
  EXPECT_EQ(matrix1(1, 1), 1);
  EXPECT_THROW(matrix1(-1, -1), std::invalid_argument);
  EXPECT_THROW(matrix1(0, -1), std::invalid_argument);
  EXPECT_THROW(matrix1(-1, 0), std::invalid_argument);
  EXPECT_THROW(matrix1(-1, 1), std::invalid_argument);
}

TEST(assignmentOperator, OperatorBracketExceptDop2) {
  Matrix matrix1(2, 3);
  EXPECT_THROW(matrix1(1, 5), std::invalid_argument);
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
