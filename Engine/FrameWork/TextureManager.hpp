#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP
#include <vector>
#include <map>
#include <string>

struct ID3D11ShaderResourceView;
struct ID3D11Device;
struct ID3D11DeviceContext;

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
	std::map<std::string, int> m_nameToIndexLinker;
	std::vector<ID3D11ShaderResourceView*> m_textures;

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
};

#endif