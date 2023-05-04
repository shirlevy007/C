#include "test.h"

int main () {
  int ok = 0;
  std::cout << "Starting tests for Activation module" << std::endl;
  ok += test_activation();
  std::cout << "Starting tests for Dense module" << std::endl;
  ok += test_dense();
  std::cout << "Starting tests for Matrix module" << std::endl;
  ok += test_matrix();
  return ok;
}