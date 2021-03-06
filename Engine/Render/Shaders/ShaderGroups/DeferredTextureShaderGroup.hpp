#ifndef DEFERRED_TEXTURE_SHADER_GROUP_HPP
#define DEFERRED_TEXTURE_SHADER_GROUP_HPP
#include "../ShaderGroup.hpp"

#include <DirectXMath.h>
//#include "../../../Math/Vector3.hpp"
#include <string>

struct ID3D11ShaderResourceView;

class DeferredTextureShaderGroup final : public ShaderGroup
{
public:
	DeferredTextureShaderGroup();
	~DeferredTextureShaderGroup();

	bool Initialize(ID3D11Device* device) final override;

	void SetupShaders(ID3D11DeviceContext* deviceContext) final override;
	void SetupPerFrameBuffer(ID3D11DeviceContext* deviceContext);
	void SetupPerObjectBuffer(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture);

private:
	std::string m_samplerName;
};

#endif