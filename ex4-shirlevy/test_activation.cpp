#include "test.h"
#include <iostream>
#include <cmath>
#define ACTIVATION_TESTS_NUM 2

bool test_relu () {
  Matrix A = Matrix(2,2);
  A[0] = 10; A[1] = -0.0000001; A[2] = 0.0001; A[3] = 1.2;
  Matrix B = activation::relu (A.vectorize());
  IS_TRUE(B[0] == 10)
  IS_TRUE(B[1] == 0)
  COMP_FLOAT(B[2], 1e-4, EPSILON)
  COMP_FLOAT(B[3], 1.2, EPSILON)
  return true;
}

bool test_softmax () {
  Matrix A = Matrix(4,1);
  A[0] = 1; A[1] = 2; A[2] = 3; A[3] = 4;
  Matrix B = activation::softmax(A);
  COMP_FLOAT(B[0], 0.0320586, EPSILON)
  COMP_FLOAT(B[1], 0.08714432, EPSILON)
  COMP_FLOAT(B[2], 0.23688282, EPSILON)
  COMP_FLOAT(B[3], 0.64391426, EPSILON)
  return true;
}

typedef bool (*testFunc) ();

const testFunc tests[ACTIVATION_TESTS_NUM] = {
    &test_relu,
    &test_softmax
};

int test_activation () {
  bool ok = true;
  bool res;
  for (int i = 0; i < ACTIVATION_TESTS_NUM; i++) {
    std::cout << "Test[" << i + 1 << "]: ";
    res = tests[i] ();
    std::cout << (res ? SUCCESS : FAILURE) << std::endl;
    ok = ok && res;
  }
  if (ok) {
    return 0;
  }
  return 1;
}