#ifndef TEXTURE_STORAGE_V2_HPP
#define TEXTURE_STORAGE_V2_HPP
//#include "../../Misc/Singleton.hpp"
#include <vector>

struct ID3D11ShaderResourceView;
class FrameWork;

class TextureStorageV2 //: public Singleton<TextureStorageV2>
{
public:
	TextureStorageV2();
	~TextureStorageV2();

	bool Initialize(FrameWork* frameWork);

	int LoadTexture(const std::string& name);

	int GetIndexByName(const std::string& name);
	const std::string& GetNameByIndex(int index);
	ID3D11ShaderResourceView* GetTexture(int index);
	//ID3D11ShaderResourceView* GetTextureByName(const std::string& name);

	unsigned int GetNrOfTextures() const;

private:
	//friend class Singleton<TextureStorageV2>;

	struct Texture
	{
		Texture(const std::string& _name, ID3D11ShaderResourceView* _texture) : name(_name), texture(_texture)
		{
		}

		std::string name = "";
		ID3D11ShaderResourceView* texture = nullptr;
	};

	std::vector<Texture> m_textures;

	FrameWork* m_frameWork;
};

#endif