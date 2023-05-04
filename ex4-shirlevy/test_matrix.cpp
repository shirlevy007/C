#include "test.h"
#include <iostream>
#include <cmath>
#define MATRIX_TESTS_NUM 18

float wrap_access(const Matrix &M, int index) {
  return M[index];
}

bool test_matrix_constructor_default () {
  Matrix M = Matrix();
  IS_TRUE(M.get_cols() == 1);
  IS_TRUE(M.get_rows() == 1);
  IS_TRUE(M(0,0) == 0);
  return true;
}

bool test_matrix_constructor_with_params () {
  Matrix M = Matrix(3, 2);
  IS_TRUE(M.get_cols() == 2);
  IS_TRUE(M.get_rows() == 3);
  for (int i = 0; i < M.get_rows() * M.get_cols(); i ++) {
    IS_TRUE((M[i] == 0))
  }
  return true;
}

bool test_matrix_constructor_copy () {
  Matrix M = Matrix(1, 2);
  M[0] = 10;
  M[1] = 20;
  Matrix P = Matrix(M);
  IS_TRUE(P.get_rows() == 1);
  IS_TRUE(P.get_cols() == 2);
  IS_TRUE(P[0] == 10);
  IS_TRUE(P[1] == 20);
  return true;
}

Matrix get_2_3_matrix () {
  Matrix M = Matrix(2, 3);
  M(0,0) = 1;
  M(0,1) = 2;
  M(0,2) = 3;
  M(1,0) = 4;
  M(1,1) = 5;
  M(1,2) = 6;
  return M;
}

bool test_transpose () {
  Matrix A = get_2_3_matrix();
  IS_TRUE(A.transpose().get_cols() == 2);
  IS_TRUE(A.get_rows() == 3);
  IS_TRUE(A(0,0) == 1);
  IS_TRUE(A(0,1) == 4);
  IS_TRUE(A(1,0) == 2);
  IS_TRUE(A(1,1) == 5);
  IS_TRUE(A(2,0) == 3);
  IS_TRUE(A(2,1) == 6);
  return true;
}

bool test_vectorize () {
  Matrix A = get_2_3_matrix();
  IS_TRUE(A.vectorize().get_cols() == 1);
  IS_TRUE(A.get_rows() == 6);
  IS_TRUE(A(0,0) == 1);
  IS_TRUE(A(1,0) == 2);
  IS_TRUE(A(2,0) == 3);
  IS_TRUE(A[3] == 4);
  IS_TRUE(A[4] == 5);
  IS_TRUE(A[5] == 6);
  return true;
}

bool test_print ()
{
  Matrix M = Matrix(2,2);
  M[0] = 0.099999;
  M[1] = 0.11;
  M[2] = 10;
  M[3] = -1.23;
  M.plain_print();
  std::cout << M;


  return true;
}

bool test_dot ()
{
  Matrix A = get_2_3_matrix();
  Matrix B = get_2_3_matrix();
  Matrix C = A.dot (B);
  IS_TRUE(C.get_cols() == 3)
  IS_TRUE(C.get_rows() == 2)
  IS_TRUE(C(0,0) == 1)
  IS_TRUE(C(0,1) == 4)
  IS_TRUE(C(0,2) == 9)
  IS_TRUE(C(1,0) == 16)
  IS_TRUE(C(1,1) == 25)
  IS_TRUE(C(1,2) == 36)
  return true;
}

bool test_norm ()
{
  Matrix A = get_2_3_matrix();
  A(0,2) = 1;
  A(1,0) = 2;
  A(1,1) = 2;
  A(1,2) = std::sqrt(2);
  IS_TRUE_MSG(A.norm() == 4, A.norm())
  return true;
}

bool test_addition ()
{
  Matrix A = get_2_3_matrix();
  Matrix B = get_2_3_matrix();
  Matrix C = A + B;
  IS_TRUE(A.get_rows() == C.get_rows())
  IS_TRUE(A.get_cols() == C.get_cols())
  for (int i = 0; i < A.get_cols() * A.get_rows(); i ++) {
    IS_TRUE(C[i] == 2 * (i+1));
  }
  return true;
}

bool test_incrementation ()
{
  Matrix A = get_2_3_matrix();
  Matrix B = get_2_3_matrix();
  A += B;
  IS_TRUE(B.get_rows() == A.get_rows())
  IS_TRUE(B.get_cols() == A.get_cols())
  for (int i = 0; i < A.get_cols() * A.get_rows(); i ++) {
      IS_TRUE(A[i] == 2 * (i+1));
      IS_TRUE(B[i] == i + 1);
    }
  return true;
}

bool test_assignment ()
{
  Matrix A = get_2_3_matrix();
  Matrix B = Matrix();
  B = A;
  IS_TRUE(B.get_rows() == A.get_rows())
  IS_TRUE(B.get_cols() == A.get_cols())
  for (int i = 0; i < A.get_cols() * A.get_rows(); i ++) {
      IS_TRUE_MSG(B[i] == A[i], "I is " << i << " A is " << A[i] << " B is " << B[i]);
  }
  return true;
}

bool test_multiplication_simple ()
{
  Matrix A = Matrix(2, 2);
  A[0] = 1;
  A[3] = 2;
  Matrix A2 = A * A;
  IS_TRUE(A2.get_rows() == A.get_rows())
  IS_TRUE(A2.get_cols() == A.get_cols())
  IS_TRUE_MSG(A2(0,0) == 1 && A2(0,1) == 0 && A2(1,0) == 0 && A2(1,1) == 4, A2)
  return true;
}

bool test_multiplication_advanced ()
{
  Matrix A = get_2_3_matrix();
  Matrix B = get_2_3_matrix().transpose();
  Matrix C = A * B;
  IS_TRUE(C.get_rows() == A.get_rows())
  IS_TRUE(C.get_cols() == B.get_cols())
  IS_TRUE(C(0,0) == 14 && C(0,1) == 32 && C(1,0) == 32 && C(1,1) == 77)
  return true;
}

bool test_scalar_multiplication ()
{
  Matrix A = get_2_3_matrix();
  Matrix B = A * 2.5;
  Matrix C = 2.5 * A;
  IS_TRUE(C.get_rows() == A.get_rows() && B.get_rows() == A.get_rows())
  IS_TRUE(C.get_cols() == A.get_cols() && B.get_cols() == A.get_cols())
  IS_TRUE(C(0,0) == B(0,0) && C(0,1) == B(0,1) && C(0,2) == B(0,2) &&
  C(1,0) == B(1,0) && C(1,1) == B(1,1) && C(1,2) == B(1,2))
  IS_TRUE(C(0,0) == 2.5 && C(0,1) == 5 && C(0,2) == 7.5 && C(1,0) == 10
  && C(1,1) == 12.5 && C(1,2) == 15)
  return true;
}

bool test_access_errors ()
{
  std::cout << std::endl;
  Matrix A = get_2_3_matrix();
  RAISES_ERROR(std::out_of_range, A, -1, -2)
  RAISES_ERROR(std::out_of_range, A, -1, 2)
  RAISES_ERROR(std::out_of_range, A, 2, 2)
  RAISES_ERROR(std::out_of_range, A, 1, 3)
  RAISES_ERROR(std::out_of_range, A, 10, 3)

  RAISES_ERROR(std::out_of_range, wrap_access, A, -2)
  RAISES_ERROR(std::out_of_range, wrap_access, A, 10)
  RAISES_ERROR(std::out_of_range, wrap_access, A, 6)
  return true;
}

bool test_constructor_errors ()
{
  std::cout << std::endl;
  RAISES_ERROR(std::length_error, new Matrix, -1, -2)
  RAISES_ERROR(std::length_error, new Matrix, 1, -2)
  RAISES_ERROR(std::length_error, new Matrix, -1, 2)
  RAISES_ERROR(std::length_error, new Matrix, 1, 0)
  RAISES_ERROR(std::length_error, new Matrix, 0, 1)
  return true;
}

bool test_same_size_errors ()
{
  std::cout << std::endl;
  Matrix A = Matrix(2,1);
  Matrix B = Matrix(1,2);
  RAISES_ERROR(std::length_error, A.dot, B)
  RAISES_ERROR(std::length_error, B.dot, A)
  RAISES_ERROR(std::length_error, A +=, B)
  RAISES_ERROR(std::length_error, B +=, A)
  return true;
}
bool test_multiply_error ()
{
  std::cout << std::endl;
  Matrix A = Matrix(2,2);
  Matrix B = Matrix(1,3);
  RAISES_ERROR(std::length_error, A *, B)
  RAISES_ERROR(std::length_error, B *, A)
  return true;
}

typedef bool (*testFunc) ();

const testFunc tests[MATRIX_TESTS_NUM] = {
    &test_matrix_constructor_default,
    &test_matrix_constructor_with_params,
    &test_matrix_constructor_copy,
    &test_transpose,
    &test_vectorize,
    &test_print,
    &test_dot,
    &test_norm,
    &test_addition,
    &test_assignment,
    &test_incrementation,
    &test_multiplication_simple,
    &test_multiplication_advanced,
    &test_scalar_multiplication,
    &test_access_errors,
    &test_constructor_errors,
    &test_same_size_errors,
    &test_multiply_error
};

int test_matrix () {
  bool ok = true;
  bool res;
  for (int i = 0; i < MATRIX_TESTS_NUM; i++) {
    std::cout << "Test[" << i + 1 << "]: ";
    res = tests[i] ();
    std::cout << (res ? SUCCESS: FAILURE) << std::endl;
    ok = ok && res;
  }
  if (ok) {
    return 0;
  }
  return 1;
}