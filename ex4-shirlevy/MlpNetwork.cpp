#include "MlpNetwork.h"

/**
 *
 * @param weights
 * @param biases
 */
MlpNetwork::MlpNetwork (Matrix *weights, Matrix *biases) : _weights (weights),
                                                           _bias (biases)
{}
/**
 *
 * @param img
 * @return
 */
digit MlpNetwork::operator() (const Matrix &img)
{
  Matrix res = Matrix (img);
  for (int i = 0; i < MLP_SIZE - 1; ++i)
  {
    Dense layer = Dense (_weights[i], _bias[i], activation::relu);
    res = layer (res);
  }
  Dense layer = Dense (_weights[MLP_SIZE - 1],
                       _bias[MLP_SIZE- 1], activation::softmax);
  res = layer (res);

  int max = 0;
  for (int i = 1; i < COLS; ++i)
  {
    if (res[i] > res[max])
    {
      max = i;
    }
  }
  digit d;
  d.value = max;
  d.probability = res[max];
  return d;

}

