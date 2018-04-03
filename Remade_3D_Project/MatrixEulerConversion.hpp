#ifndef MATRIX_EULER_CONVERSION_HPP
#define MATRIX_EULER_CONVERSION_HPP
#include "Vector3.hpp"
#include <DirectXMath.h>

Vector3f MatrixToEuler(const DirectX::XMMATRIX& m);
DirectX::XMFLOAT4X4 EulerToMatrix(const Vector3f& v);

#endif