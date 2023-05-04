#include "Dense.h"

/**
 *
 * @param weights
 * @param bias
 * @param af
 */
Dense::Dense (const Matrix &weights, const Matrix &bias, ActivationFunc af):
_weights(weights), _bias(bias), _activation_func(af){}
/**
   *
   * @return Returns the weights of this layer.
   */
Matrix Dense::get_weights () const
{
  return _weights;
}
/**
 *
 * @return Returns the bias of this layer.
 */
Matrix Dense::get_bias () const
{
  return _bias;
}
/**
 *
 * @return the activation function of this layer.
 */
ActivationFunc Dense::get_activation () const
{
  return _activation_func;
}
/**
 * Applies the layer on input matrix
 * @param input input matrix
 * @return output matrix Layers operate as per section
 */
Matrix Dense::operator() (const Matrix &input) const
{
  Matrix mat = (_weights * input) + _bias;
  Matrix output = _activation_func (mat);
  return output;
}


