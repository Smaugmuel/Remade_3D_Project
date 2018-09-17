#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP
#include <vector>
#include <map>
#include <string>

struct ID3D11ShaderResourceView;
struct ID3D11Device;
struct ID3D11DeviceContext;

static const unsigned int MAX_NR_OF_TEXTURES = 16U;

class TextureManager final
{
public:
	TextureManager();
	~TextureManager();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	int LoadTexture(const std::string& fileName);

	bool SetTextureToPixelShader(int id, int slot);

	int GetTextureIndex(const std::string& fileName);
	ID3D11ShaderResourceView* GetTexture(int id);

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;

	std::map<std::string, int> m_nameToIndexLinker;
	ID3D11ShaderResourceView* m_textures[MAX_NR_OF_TEXTURES];
	unsigned int m_nrOfTextures;
};

#endif