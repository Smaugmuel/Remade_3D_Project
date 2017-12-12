#ifndef SHADER_MANAGER_HPP
#define SHADER_MANAGER_HPP
#include "Singleton.hpp"

#include <memory>
//#include <string>
#include "Vector3.hpp"

class Object;
class Camera;

struct ID3D11Device;
struct ID3D11DeviceContext;

class SingleColorShaderGroup;
class DeferredSingleColorShaderGroup;
class DeferredLightShaderGroup;
class DepthShaderGroup;
class DeferredShadowShaderGroup;
class TextureShaderGroup;

enum ShaderType
{
	SINGLE_COLOR,
	D_SINGLE_COLOR,
	D_LIGHT,
	DEPTH,
	D_SHADOW,
	TEXTURE
};

class ShaderManager : public Singleton<ShaderManager>
{
	friend class Singleton<ShaderManager>;
	ShaderManager();
	~ShaderManager();

public:
	bool Initialize(ID3D11Device* device);

	void SetShaderType(ID3D11DeviceContext* deviceContext, const ShaderType& shaderType);
	
	void SetPerFrameConstantBuffer(ID3D11DeviceContext* deviceContext, Camera* camera, Camera* lightCamera, float lightIntensity);
	void SetPerObjectConstantBuffer(ID3D11DeviceContext* deviceContext, Object* object);

private:
	std::unique_ptr<SingleColorShaderGroup> m_colorShaders;
	std::unique_ptr<DeferredSingleColorShaderGroup> m_d_colorShaders;
	std::unique_ptr<DeferredLightShaderGroup> m_d_lightShaders;
	std::unique_ptr<DepthShaderGroup> m_depthShaders;
	std::unique_ptr<DeferredShadowShaderGroup> m_s_shaders;
	std::unique_ptr<TextureShaderGroup> m_textureShaders;

	ShaderType m_currentShaderType;
};

#endif