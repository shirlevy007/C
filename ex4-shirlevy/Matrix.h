// Matrix.h
#ifndef MATRIX_H
#define MATRIX_H

#include <cmath>
#include <iostream>
#include <stdlib.h>

/**
 * @struct matrix_dims
 * @brief Matrix dimensions container. Used in MlpNetwork.h and main.cpp
 */
typedef struct matrix_dims
{
    int rows, cols;
} matrix_dims;

class Matrix
{
  matrix_dims *_dims;
  float *_data;
//  int get_data_in_index(int row, int col);

 public:
  /**
   * constructor with given dims
   */
  Matrix (int rows, int cols);
  /**
   * default constructor
   */
  Matrix ();
  /**
   * copy constructor
   */
  Matrix (const Matrix &mat);
  /**
   * destructor
   */
  ~Matrix ();
  /**
   * get rows
   */
  int get_rows () const;
  /**
   * get cols
   */
  int get_cols () const;
  /**
   * transpose to self matrix
   */
  Matrix &transpose ();
  /**
   * vectorize
   */
  Matrix &vectorize ();
  /**
   * prints matrix no return value
   */
  void plain_print () const;
  /**
   * returns matrix multiplied by other (by cell)
   */
  Matrix dot (const Matrix &mat);
  /**
   * returns matrix's norm
   */
  float norm () const;
  /**
   * adding a matrix to matrix
   */
  Matrix operator+ (const Matrix &mat);
  /**
   *
   */
  friend void swap_mat (Matrix &first, Matrix &other);
  /**
   * assignment of a matrix to matrix
   */
//       Matrix& operator=(const Matrix& mat);
  Matrix &operator= (Matrix mat);
  /**
   * matrix multiplication
   */
  Matrix operator* (const Matrix &mat) const;
  /**
   * Scalar multiplication on the right
   */
  Matrix operator* (float c) const;
  /**
  * Scalar multiplication on the left
  */
  friend Matrix operator* (float c,  Matrix const &mat);
  /**
   *
   */
  Matrix &operator+= (const Matrix &mat);
  /**
   * Parenthesis indexing
   */
  float &operator() (int row, int col);
  /**
   * Parenthesis indexing
   */
  float operator() (int row, int col) const;
  /**
  * Brackets indexing
  */
  float &operator[] (int index);
  /**
   * Brackets indexing
   */
  float operator[] (int index) const;
  /**
   * Output stream
   */
  friend std::ostream &operator<< (std::ostream &os, const Matrix &mat);
  /**
   * Input stream
   */
  friend std::istream &operator>> (std::istream &is, Matrix &mat);

};

#endif //MATRIX_H