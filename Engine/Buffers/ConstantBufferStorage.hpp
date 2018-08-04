#ifndef CONSTANT_BUFFER_STORAGE_HPP
#define CONSTANT_BUFFER_STORAGE_HPP
#include "../Misc/Singleton.hpp"
#include "../Math/Vector3.hpp"

#include <DirectXMath.h>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Buffer;

class ConstantBufferStorage final : public Singleton<ConstantBufferStorage>
{
	friend class Singleton<ConstantBufferStorage>;

	ConstantBufferStorage();
	~ConstantBufferStorage();

public:
	bool Initialize(ID3D11Device* device);

	bool SetVSWorldMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX& matrix);
	bool SetVSViewMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX& matrix);
	bool SetVSProjectionMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX& matrix);
	bool SetVSPointLight(ID3D11DeviceContext* deviceContext, const Vector3f& position, float intensity);
	bool SetVSColor(ID3D11DeviceContext* deviceContext, const Vector3f& color);

	bool SetGSViewMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX& matrix);
	bool SetGSProjectionMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX& matrix);
	bool SetGSWorldMatrixArray(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX* matrices);
	//bool SetGSPointLight(ID3D11DeviceContext* deviceContext, const Vector3f& position, float intensity);
	bool SetGSNrOfObjects(ID3D11DeviceContext* deviceContext, int nrOfObjects);

	bool SetPSViewMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX& matrix);
	bool SetPSProjectionMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX& matrix);
	bool SetPSViewMatrixArray(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX* matrices);
	bool SetPSProjectionMatrixArray(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX* matrices);
	bool SetPSPointLight(ID3D11DeviceContext* deviceContext, const Vector3f& position, float intensity);
	bool SetPSPointLightArray(ID3D11DeviceContext* deviceContext, Vector3f* positions, float* intensities);
	bool SetPSNrOfLights(ID3D11DeviceContext* deviceContext, int nrOfLights);

private:
	bool MapWorldMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX& matrix);
	bool MapViewMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX& matrix);
	bool MapProjectionMatrix(ID3D11DeviceContext * deviceContext, const DirectX::XMMATRIX& matrix);
	bool MapWorldMatrixArray(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX* matrices);
	bool MapViewMatrixArray(ID3D11DeviceContext * deviceContext, DirectX::XMMATRIX* matrices);
	bool MapProjectionMatrixArray(ID3D11DeviceContext * deviceContext, DirectX::XMMATRIX* matrices);
	bool MapPointLight(ID3D11DeviceContext * deviceContext, const Vector3f& position, float intensity);
	bool MapPointLightArray(ID3D11DeviceContext * deviceContext, Vector3f* positions, float* intensities);
	bool MapColor(ID3D11DeviceContext * deviceContext, const Vector3f& color);
	bool MapNrOfLights(ID3D11DeviceContext * deviceContext, int nrOfLights);
	bool MapNrOfObjects(ID3D11DeviceContext* deviceContext, int nrOfObjects);

	ID3D11Buffer* m_worldBuffer;
	ID3D11Buffer* m_viewBuffer;
	ID3D11Buffer* m_projectionBuffer;
	ID3D11Buffer* m_pointLightBuffer;
	ID3D11Buffer* m_pointLightArrayBuffer;
	ID3D11Buffer* m_colorBuffer;
	ID3D11Buffer* m_viewArrayBuffer;
	ID3D11Buffer* m_projectionArrayBuffer;
	ID3D11Buffer* m_nrOfLightsBuffer;
	ID3D11Buffer* m_worldArrayBuffer;
	ID3D11Buffer* m_nrOfObjectsBuffer;
};

#endif