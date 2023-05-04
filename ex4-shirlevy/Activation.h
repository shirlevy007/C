#include "Matrix.h"

#ifndef ACTIVATION_H
#define ACTIVATION_H

typedef Matrix (*ActivationFunc) (Matrix& mat);

namespace activation
{
    Matrix relu (Matrix& mat);
    Matrix softmax (Matrix& mat);
}

#endif //ACTIVATION_H