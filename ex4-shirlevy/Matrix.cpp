#include "Matrix.h"

#include <cmath>
#include <iostream>
#include <iostream>
//#include <stdlib.h>
#include <fstream>

//#pragma clang diagnostic push
//#pragma clang diagnostic ignored "-Wreturn-stack-address"
#define DOUBLE_ASTRISK "**"
#define DOUBLE_SPACE "  "
#define BASE2 2
#define MIN_PROB 0.1

#define NEG_VALUES "Error: illegal values! must be grater than 0"
#define UNMATCHING_SIZE "Error: unmatching marices size"
#define INVALID_MULT "Error: tried to multiply matrices of different size"
#define OUT_OF_BOUNDS "Error: Index out of bounds"
#define INVALID_BINARY "Error: binary file doesn't open"
#define BINARY_READ_ERR "Error: binary writing errored"

//using std::cout;
//using std::cin;
//using std::endl;
//using std::ostream;
//using std::istream;

/**
 * constructor of Matrix by given dimensions
 * @param rows
 * @param cols columns
 */
Matrix::Matrix (int rows, int cols)
{
  if (rows <= 0 || cols <= 0)
  {
    throw std::length_error (NEG_VALUES);
  }
  _dims = new matrix_dims;
  _dims->rows = rows;
  _dims->cols = cols;
  _data = new float[rows * cols]{0};
}
/**
 * default constructor
 */
Matrix::Matrix () : Matrix (1, 1)
{}
/**
 * copy constructor
 * @param mat matrix reference
 */
Matrix::Matrix (const Matrix &mat) : Matrix (mat._dims->rows, mat._dims->cols)
{
  for (int i = 0; i < (mat._dims->rows * mat._dims->cols); i++)
  {
    _data[i] = mat._data[i];
  }
}
/**
 * destructor
 */
Matrix::~Matrix ()
{
  delete _dims;
  delete[] _data;
}
/**
 *
 * @return num of rows
 */
int Matrix::get_rows () const
{ return _dims->rows; }
/**
 *
 * @return num of columns
 */
int Matrix::get_cols () const
{ return _dims->cols; }
/**
 * transposes self matrix
 * @return the transposed matrix
 */
Matrix &Matrix::transpose ()
{
  float *trans = new float[_dims->rows * _dims->cols]{0};
  for (int i = 0; i < _dims->rows; i++)
  {
    for (int j = 0; j < _dims->cols; j++)
    {
      trans[i + (j * _dims->rows)] = _data[j + (i * _dims->cols)];
    }
  }
  int tmp = _dims->rows;
  _dims->rows = _dims->cols;
  _dims->cols = tmp;
//  std::swap (_dims->rows, _dims->cols);
  for (int i = 0; i < _dims->rows*_dims->cols; ++i)
  {
    _data[i] = trans[i];
  }
  return *this;
}
/**
 * transfers matrix into a column vector
 * @return the column vector
 */
Matrix &Matrix::vectorize ()
{
  _dims->rows *= _dims->cols;
  _dims->cols = 1;
  return *this;
}
/**
 * prints the matrix
 */
void Matrix::plain_print () const
{
  for (int i = 0; i < _dims->rows; i++)
  {
    for (int j = 0; j < _dims->cols; j++)
    {
      std::cout << _data[j + (i * _dims->cols)] << " ";
    }
    std::cout << std::endl;
  }
}
/**
 * multiplies self cell with matching cell of matrix mat
 * @param mat matrix to multiply by
 * @return multiplied matrix
 */
Matrix Matrix::dot (const Matrix &mat)
{
  if (mat.get_rows () != _dims->rows || mat.get_cols () != _dims->cols)
  {
    throw std::length_error (INVALID_MULT);
  }
  Matrix mult (*this);
  for (int i = 0; i < _dims->rows * _dims->cols; i++)
  {
    mult._data[i] *= mat._data[i];
  }
  return mult;
}
/**
 *
 * @return the norm of self matrix
 */
float Matrix::norm () const
{
  float sum = 0;
  for (int i = 0; i < _dims->rows * _dims->cols; ++i)
  {
    sum += powf (_data[i], BASE2);
  }
  return sqrtf (sum);
}
/**
 *
 * @param mat matrix to add self
 * @return new matrix
 */
Matrix Matrix::operator+ (const Matrix &mat)
{
  if (mat.get_rows () != _dims->rows || mat.get_cols () != _dims->cols)
  {
    throw std::length_error (UNMATCHING_SIZE);
  }
  Matrix sum_matrix (*this);
  for (int i = 0; i < _dims->rows * _dims->cols; ++i)
  {
    sum_matrix._data[i] += mat._data[i];
  }
  return sum_matrix;
}
/**
 *
 * @param first
 * @param other
 */
void swap_mat (Matrix &first, Matrix &other)
{
  std::swap (first._dims->rows, other._dims->rows);
  std::swap (first._dims->cols, other._dims->cols);
  std::swap (first._data, other._data);
}
/**
 * assignment of mat in self
 * @param mat matrix to ocpy
 * @return
 */
//Matrix& Matrix::operator=(const Matrix& mat)
//{
//  if (this==&mat)
//  {
//    return *this;
//  }
//  delete[] this->_data;
//  _dims->rows = mat.get_rows();
//  _dims->cols = mat.get_cols();
//  _data = new float [mat._dims->rows * mat._dims->cols]{0};
//  for (int i = 0; i < mat._dims->rows * mat._dims->cols; ++i)
//  {
//    _data[i] = mat._data[i];
//  }
//  return *this;
//}
Matrix &Matrix::operator= (Matrix mat)
{
  if (this == &mat)
  {
    return *this;
  }
  swap_mat (*this, mat);
  return *this;
}
/**
 * matrices multiplication
 * @param mat matrix
 * @return self after multiplication matrix
 */
Matrix Matrix::operator* (const Matrix &mat) const
{
  if (mat.get_rows () != _dims->cols)
  {
    throw std::length_error (UNMATCHING_SIZE);
  }
  Matrix mult (_dims->rows, mat.get_cols ());
  float res;
  for (int i = 0; i < _dims->rows; ++i)
  {
    for (int j = 0; j < mat.get_cols (); ++j)
    {
      res = 0;
      for (int k = 0; k < _dims->cols; ++k)
      {
        res += _data[k + (i * _dims->cols)]
               * mat._data[j + (k * mat.get_cols())];
      }
      mult._data[j + (i * mat.get_cols())] = res;
    }
  }
  return mult;
}
/**
 * Scalar multiplication on the right
 * @param c scalar
 * @return self after multiplication matrix
 */
Matrix Matrix::operator* (float c) const
{
  Matrix mult (*this);
  for (int i = 0; i < _dims->cols * _dims->rows; ++i)
  {
    mult._data[i] *= c;
  }
  return mult;
}
/**
 * Scalar multiplication on the left
 * @param c scalar
 * @return self after multiplication matrix
 */
Matrix operator* (float c, Matrix const &mat)
{
  return mat * c;
}
/**
 *
 * @param mat
 * @return
 */
Matrix &Matrix::operator+= (const Matrix &mat)
{
  if (mat.get_rows () != _dims->rows || mat.get_cols () != _dims->cols)
  {
    throw std::length_error (UNMATCHING_SIZE);
  }
  *this = *this + mat;
  return *this;
}
/**
 * Parenthesis indexing
 * @param row of cell
 * @param col of cell
 * @return cell
 */
float &Matrix::operator() (int row, int col)
{
//  if (0 <= row && row < _dims->rows && 0 <= col && col < _dims->cols)
//  {
//    return _data[col + (row * _dims->cols)];
//  }
//  throw std::out_of_range (OUT_OF_BOUNDS);
  if (row >= _dims->rows || row < 0 || col < 0 || col >= _dims->cols)
  {
    throw std::out_of_range (OUT_OF_BOUNDS);
  }
  return _data[col + (row * _dims->cols)];
}
/**
 * Parenthesis indexing
 * @param row of cell
 * @param col of cell
 * @return cell's value
 */
float Matrix::operator() (int row, int col) const
{
//  if (0 <= row && row < _dims->rows && 0 <= col && col < _dims->cols)
//  {
//    return _data[col + (row * _dims->cols)];
//  }
//  throw std::out_of_range (OUT_OF_BOUNDS);
  if (row >= _dims->rows || row < 0 || col < 0 || col >= _dims->cols)
  {
    throw std::out_of_range (OUT_OF_BOUNDS);
  }
  return _data[col + (row * _dims->cols)];
}
/**
 * Brackets indexing
 * @param index of cell
 * @return the cell  can be changed
 */
float &Matrix::operator[] (int index)
{
  if (0 <= index && index < _dims->rows * _dims->cols)
  {
    return _data[index];
  }
  throw std::out_of_range (OUT_OF_BOUNDS);
}
/**
 * Brackets indexing
 * @param index of cell
 * @return the cell's value
 */
float Matrix::operator[] (int index) const
{
  if (0 <= index && index < _dims->rows * _dims->cols)
  {
    return _data[index];
  }
  throw std::out_of_range (OUT_OF_BOUNDS);
}

std::ostream &operator<< (std::ostream &os, const Matrix &mat)
{
  for (int i = 0; i < mat.get_rows (); i++)
  {
    for (int j = 0; j < mat.get_cols (); j++)
    {
      if (mat[j + (i * mat.get_cols())] > MIN_PROB)
      {
        os << DOUBLE_ASTRISK << " ";
      }
      else
      {
        os << DOUBLE_SPACE << " ";
      }
    }
    os << std::endl;
  }
  return os;
}
/**
 *
 * @param is
 * @param mat
 * @return
 */
std::istream &operator>> (std::istream &is, Matrix &mat)
{
  if (!is.good ())
  {
    throw std::runtime_error (INVALID_BINARY);
  }
  auto size_to_read = (long) (mat.get_rows () * mat.get_cols ()
                              * sizeof (float));
  float *temp = new float[mat.get_rows () * mat.get_cols ()];
  is.read ((char *) temp, size_to_read);
  if (is.fail ())
  {
    throw std::runtime_error (INVALID_BINARY);
  }
  delete[] mat._data;
  mat._data = temp;
  return is;
}


//#pragma clang diagnostic pop