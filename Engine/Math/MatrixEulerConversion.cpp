#include "MatrixEulerConversion.hpp"
#include "../Misc/HelperFunctions.hpp"

Vector3f MatrixToEuler(const DirectX::XMMATRIX & m)
{
	Vector3f angles;

	DirectX::XMFLOAT4X4 mat;
	DirectX::XMStoreFloat4x4(&mat, DirectX::XMMatrixTranspose(m));

	if (mat._31 != 1 && mat._31 != -1)
	{
		//angles.x = -asin(mat._31);
		angles.x = -asinf(mat._31);

		float invCosX = 1.0f / cosf(angles.x);

		angles.y = atan2f(mat._32 * invCosX, mat._33 * invCosX);
		angles.z = atan2f(mat._21 * invCosX, mat._11 * invCosX);
	}
	else
	{
		angles.z = 0;
		if (mat._31 == -1)
		{
			angles.x = 3.1415927f * 0.5f;
			angles.y = atan2f(mat._12, mat._13);
		}
		else
		{
			angles.x = -3.1415927f * 0.5f;
			angles.y = atan2f(-mat._12, -mat._13);
		}
	}

	return angles;
}

DirectX::XMFLOAT4X4 EulerToMatrix(const Vector3f & v)
{
	DirectX::XMFLOAT4X4 m;
	DirectX::XMStoreFloat4x4(&m, DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationRollPitchYaw(v.x, v.y, v.z)));

	// Values are correct, but some are in wrong places (not transposed)
	HF::Swap(m._11, m._22);
	HF::Swap(m._12, m._21);
	HF::Swap(m._13, m._23);
	HF::Swap(m._31, m._32);

	return m;
}
