#include "Frustum.hpp"

//void Frustum::Create(const Matrix & view, const Matrix & projection)
//{
//	Matrix matrix = view * projection;
//	/*DirectX::XMMATRIX viewProjection = DirectX::XMMatrixMultiply(view, projection);
//	DirectX::XMFLOAT4X4 matrix;*/
//	float invLength;
//
//	//DirectX::XMStoreFloat4x4(&matrix, viewProjection);
//
//	// Top clipping plane
//	//planes[0].normal.x = -(matrix._14 - matrix._12);
//	//planes[0].normal.y = -(matrix._24 - matrix._22);
//	//planes[0].normal.z = -(matrix._34 - matrix._32);
//	//planes[0].d = -(matrix._44 - matrix._42);
//
//	//// Bottom clipping plane
//	//planes[1].normal.x = -(matrix._14 + matrix._12);
//	//planes[1].normal.y = -(matrix._24 + matrix._22);
//	//planes[1].normal.z = -(matrix._34 + matrix._32);
//	//planes[1].d = -(matrix._44 + matrix._42);
//
//	//// Left clipping plane
//	//planes[2].normal.x = -(matrix._14 + matrix._11);
//	//planes[2].normal.y = -(matrix._24 + matrix._21);
//	//planes[2].normal.z = -(matrix._34 + matrix._31);
//	//planes[2].d = -(matrix._44 + matrix._41);
//
//	//// Right clipping plane
//	//planes[3].normal.x = -(matrix._14 - matrix._11);
//	//planes[3].normal.y = -(matrix._24 - matrix._21);
//	//planes[3].normal.z = -(matrix._34 - matrix._31);
//	//planes[3].d = -(matrix._44 - matrix._41);
//
//	//// Near clipping plane
//	//planes[4].normal.x = -(matrix._14 + matrix._13);
//	//planes[4].normal.y = -(matrix._24 + matrix._23);
//	//planes[4].normal.z = -(matrix._34 + matrix._33);
//	//planes[4].d = -(matrix._44 + matrix._43);
//
//	//// Far clipping plane
//	//planes[5].normal.x = -(matrix._14 - matrix._13);
//	//planes[5].normal.y = -(matrix._24 - matrix._23);
//	//planes[5].normal.z = -(matrix._34 - matrix._33);
//	//planes[5].d = -(matrix._44 - matrix._43);
//
//	// Normalize plane
//	for (int i = 0; i < 6; i++)
//	{
//		invLength = 1.0f / planes[i].normal.length();
//
//		/* Vector::Normalize could be used, 
//		but since the inverse length needs to be calculated for the float anyway, 
//		it would require twice the amount of square roots and divisions */
//		planes[i].normal.x *= invLength;
//		planes[i].normal.y *= invLength;
//		planes[i].normal.z *= invLength;
//		planes[i].d *= invLength;
//	}
//}

//void FrustumCorners(const Matrix & view, const Matrix & projection, Vector3f * cornerArray)
//{
//	/*DirectX::XMMATRIX matrix;
//	DirectX::XMVECTOR determinant;
//	DirectX::XMMATRIX inverse;
//
//	DirectX::XMVECTOR corners[8];*/
//
//	// Boundaries in NDC space
//	/*corners[0] = { -1, -1, 0, 1 };
//	corners[1] = { -1, -1, 1, 1 };
//	corners[2] = { -1,  1, 0, 1 };
//	corners[3] = { -1,  1, 1, 1 };
//	corners[4] = {  1, -1, 0, 1 };
//	corners[5] = {  1, -1, 1, 1 };
//	corners[6] = {  1,  1, 0, 1 };
//	corners[7] = {  1,  1, 1, 1 };*/
//
//	/* ========================================= Conversion from world to NDC-coordinates =========================================== */
//	//DirectX::XMVECTOR originWorldSpace = { 0.0f, 0.0f, 0.0f, 1 };
//	//DirectX::XMVECTOR originViewSpace = DirectX::XMVector4Transform(originWorldSpace, DirectX::XMMatrixTranspose(view));
//	//DirectX::XMVECTOR originProjSpace = DirectX::XMVector4Transform(originViewSpace, DirectX::XMMatrixTranspose(projection));
//	//DirectX::XMVECTOR originNDCSpace = originProjSpace;
//	//
//	//// Convert to [(-1, -1, 0), (1, 1, 1)]
//	//// The z-value is non-linear, being ~0.48 when distance is ~0.5 and ~1 when distance is ~1000
//	//originNDCSpace.m128_f32[0] /= originNDCSpace.m128_f32[3];
//	//originNDCSpace.m128_f32[1] /= originNDCSpace.m128_f32[3];
//	//originNDCSpace.m128_f32[2] /= originNDCSpace.m128_f32[3];
//
//
//
//	// Convert to view space
//	/*matrix = DirectX::XMMatrixTranspose(projection);
//	determinant = DirectX::XMMatrixDeterminant(matrix);
//	inverse = DirectX::XMMatrixInverse(&determinant, matrix);
//	for (unsigned int i = 0; i < 8; i++)
//	{
//		corners[i] = DirectX::XMVector4Transform(corners[i], inverse);
//	}
//
//
//	// Convert to world space
//	matrix = DirectX::XMMatrixTranspose(view);
//	determinant = DirectX::XMMatrixDeterminant(matrix);
//	inverse = DirectX::XMMatrixInverse(&determinant, matrix);
//	for (unsigned int i = 0; i < 8; i++)
//	{
//		corners[i] = DirectX::XMVector4Transform(corners[i], inverse);
//
//		cornerArray[i] = corners[i];
//		cornerArray[i] *= (1.0f / corners[i].m128_f32[3]);
//	}*/
//
//	return;
//}

//AABB FrustumAABB(const Matrix & view, const Matrix & projection)
//{
//	Vector3f cornerArray[8];
//	Vector3f aabbMin, aabbMax;
//	AABB aabb;
//
//	//FrustumCorners(view, projection, cornerArray);
//
//	for (unsigned int i = 0; i < 8; i++)
//	{
//		if (cornerArray[i].x < aabbMin.x)
//			aabbMin.x = cornerArray[i].x;
//		if (cornerArray[i].y < aabbMin.y)
//			aabbMin.y = cornerArray[i].y;
//		if (cornerArray[i].z < aabbMin.z)
//			aabbMin.z = cornerArray[i].z;
//		if (cornerArray[i].x > aabbMax.x)
//			aabbMax.x = cornerArray[i].x;
//		if (cornerArray[i].y > aabbMax.y)
//			aabbMax.y = cornerArray[i].y;
//		if (cornerArray[i].z > aabbMax.z)
//			aabbMax.z = cornerArray[i].z;
//	}
//
//	aabb.center = (aabbMin + aabbMax) * 0.5f;
//	aabb.halfSides = (aabbMax - aabbMin) * 0.5f;
//
//	return aabb;
//}