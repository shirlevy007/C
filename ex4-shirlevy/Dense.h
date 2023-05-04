#ifndef DENSE_H
#define DENSE_H

#include "Activation.h"

class Dense
{
  Matrix _weights;
  Matrix _bias;
  ActivationFunc _activation_func;

 public:
  /**
   *
   * @param weights
   * @param bias
   * @param af
   */
  Dense (const Matrix &weights, const Matrix &bias, ActivationFunc af);
  /**
   *
   * @return
   */
  Matrix get_weights () const;
  /**
   *
   * @return
   */
  Matrix get_bias () const;
  /**
   *
   * @return
   */
  ActivationFunc get_activation () const;
  /**
   *
   * @param input input matrix
   * @return
   */
  Matrix operator() (const Matrix &input) const;

};

#endif //DENSE_H
