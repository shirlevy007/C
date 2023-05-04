#ifndef _TEST_H_
#define _TEST_H_

#include "Matrix.h"
#include "Activation.h"
#include "Dense.h"


#define IS_TRUE(x) IS_TRUE_MSG(x, "")
#define IS_TRUE_MSG(x, msg) if (!(x)) { std::cout << __FUNCTION__ << " failed on line " << __LINE__ << ". Message: " << msg << std::endl; return false; }
#define EPSILON 1e-7
#define COMP_FLOAT(x, y, epsilon) IS_TRUE_MSG(std::fabs (x - y) < epsilon, "Distance is: " << std::fabs (x - y))
#define RAISES_ERROR(error, function, ...) try { function ( __VA_ARGS__ ); return false; } catch (const error &Error) { std::cout << "Got Expected error: " << Error.what() << std::endl; }
#define SUCCESS "\033[1;32mSUCCESS\033[0m"
#define FAILURE "\033[1;31mFAILURE\033[0m"

#include "test_activation.h"
#include "test_dense.h"
#include "test_matrix.h"

#endif //_TEST_H_
