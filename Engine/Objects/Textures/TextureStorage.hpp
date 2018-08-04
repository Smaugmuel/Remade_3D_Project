#ifndef TEXTURE_STORAGE_HPP
#define TEXTURE_STORAGE_HPP
#include "../../Misc/Singleton.hpp"
#include <map>
#include <string>
#include <memory>

class Texture;
struct ID3D11Device;
struct ID3D11ShaderResourceView;

class TextureStorage final : public Singleton<TextureStorage>
{
	friend class Singleton<TextureStorage>;

	TextureStorage();
	~TextureStorage();

public:
	bool LoadTexture(ID3D11Device* device, const std::string& name);

	ID3D11ShaderResourceView* GetTexture(const std::string& name);

	bool HasTexture(const std::string& name) const;

private:
	std::map<std::string, std::unique_ptr<Texture>> m_textures;
};

#endif