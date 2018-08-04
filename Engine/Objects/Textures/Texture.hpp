#ifndef TEXTURE_HPP
#define TEXTURE_HPP

struct ID3D11Device;
struct ID3D11ShaderResourceView;

class Texture final
{
public:
	Texture();
	~Texture();

	bool Initialize(ID3D11Device* device, const char* fileName);

	ID3D11ShaderResourceView* GetShaderResourceView();

private:
	ID3D11ShaderResourceView* m_shaderResourceView;
};

#endif