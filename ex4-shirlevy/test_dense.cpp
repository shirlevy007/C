#include "test.h"
#include <iostream>
#include <cmath>
#define DENSE_TESTS_NUM 2

bool test_layer_with_relu () {
  Matrix W = Matrix(2,2);
  W[0] = 4; W[1] = -2; W[2] = 1.5; W[3] = -10;
  Matrix B = Matrix(2,1);
  B[0] = 100; B[1] = -100;

  Dense D = Dense(W, B, activation::relu);

  Matrix input = Matrix(2,1);
  input[0] = input[1] = 1;
  Matrix output = D(input);

  IS_TRUE(output[0] == 102 && output[1] == 0)
  return true;
}

bool test_layer_with_softmax () {
  Matrix A = Matrix(3,2);
  A[0] = 1; A[1] = 2; A[2] = 3; A[3] = 4; A[4] = 5; A[5] = 6;
  Matrix B = Matrix(3,1);
  B[0] = -10; B[1] = -20; B[2] = -30;

  Dense D = Dense(A, B, activation::softmax);

  Matrix input = Matrix(2,1);
  input[0] = input[1] = 1;
  Matrix output = D(input);
  COMP_FLOAT(output[0], 0.9975212, EPSILON)
  COMP_FLOAT(output[1], 0.0024726, EPSILON)
  COMP_FLOAT(output[2], 6.12898e-06, EPSILON)

  return true;
}

typedef bool (*testFunc) ();

const testFunc tests[DENSE_TESTS_NUM] = {
    &test_layer_with_relu,
    &test_layer_with_softmax
};

int test_dense () {
  bool ok = true;
  bool res;
  for (int i = 0; i < DENSE_TESTS_NUM; i++) {
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