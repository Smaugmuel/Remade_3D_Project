#ifndef TEXTURE_OBJECT_HPP
#define TEXTURE_OBJECT_HPP
#include "Object.hpp"
//#include "QuadTreeObject.hpp"

class TextureObject final : public Object//, public QuadTreeObject
{
public:
	TextureObject();
	~TextureObject();

	bool Initialize(const std::string& modelName/*, const std::string& textureName*/);

	void SetModel(int index);
	void SetModelName(const std::string& name);
	void SetTexture(int index);
	void SetTextureName(const std::string& name);
	void SetDefaultTexture();

	void SetPosition(const Vector3f& position) final override;
	void SetPosition(float x, float y, float z) final override;

	int GetModelIndex() const;
	int GetTextureIndex() const;
	int GetDefaultTextureIndex() const;

	const std::string& GetModelName() const;
	const std::string& GetTextureName() const;
	const std::string& GetDefaultTextureName() const;

private:
	void UpdateOBB() final override;

	int m_modelIndex;
	int m_textureIndex;
	int m_defaultTextureIndex;
	//std::string m_textureName;
	//std::string m_defaultTextureName;
};

#endif