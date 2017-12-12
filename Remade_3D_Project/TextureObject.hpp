#ifndef TEXTURE_OBJECT_HPP
#define TEXTURE_OBJECT_HPP
#include "Object.hpp"
#include <DirectXMath.h>
#include "Vector3.hpp"
#include <memory>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11ShaderResourceView;

class TextureModel;
class Texture;

class TextureObject : public Object
{
public:
	TextureObject();
	~TextureObject();

	bool Initialize(ID3D11Device* device, const char* modelName) override;
	void Render(ID3D11DeviceContext* deviceContext) override;

	bool LoadTexture(ID3D11Device* device, const char* textureName);

	ID3D11ShaderResourceView* GetTexture();

private:
	std::unique_ptr<TextureModel> m_model;
	std::unique_ptr<Texture> m_texture;
};

#endif