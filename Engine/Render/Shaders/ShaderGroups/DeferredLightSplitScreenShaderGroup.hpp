#ifndef DEFERRED_LIGHT_SPLIT_SCREEN_SHADER_GROUP_HPP
#define DEFERRED_LIGHT_SPLIT_SCREEN_SHADER_GROUP_HPP
#include "../ShaderGroup.hpp"
#include "../../../Math/Vector3.hpp"

#include <DirectXMath.h>
#include <string>

struct ID3D11Buffer;
struct ID3D11ShaderResourceView;

class DeferredLightSplitScreenShaderGroup final : public ShaderGroup
{
	struct PS_PerFrameBuffer
	{
		Vector3f lightPosition;
		float lightIntensity;
		DirectX::XMMATRIX lightView;
		DirectX::XMMATRIX lightProj;
	};

public:
	DeferredLightSplitScreenShaderGroup();
	~DeferredLightSplitScreenShaderGroup();

	bool Initialize(ID3D11Device* device) final override;

	void SetupShaders(ID3D11DeviceContext* deviceContext) final override;
	void SetupPerFrameBuffer(ID3D11DeviceContext* deviceContext, unsigned int nrOfResources, ID3D11ShaderResourceView** resources, ID3D11ShaderResourceView* depthTexture);

private:
	std::string m_samplerName;

	ID3D11Buffer* m_psPerFrameBuffer;
};

#endif