#include "Activation.h"

Matrix activation::relu (Matrix& mat)
{
  mat.vectorize();
  for (int i = 0; i < mat.get_rows (); ++i)
  {
    mat[i] = (mat[i] >= 0 ? mat[i] : 0);
  }
  return mat;
}

Matrix activation::softmax (Matrix& mat)
{
  mat.vectorize();
  float sum = 0;
  for (int i = 0; i < mat.get_rows (); ++i)
  {
    mat[i] = std::exp (mat[i]);
    sum += mat[i];
  }
  return mat * (1 / sum);
}