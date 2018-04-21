#ifndef TEXTURE_OBJECT_HPP
#define TEXTURE_OBJECT_HPP
#include "Object.hpp"
#include "QuadTreeObject.hpp"

class TextureObject : public Object, public QuadTreeObject
{
public:
	TextureObject();
	~TextureObject();

	bool Initialize() override;
	bool Initialize(const std::string& modelName/*, const std::string& textureName*/);

	void SetTextureName(const std::string& name);
	void SetDefaultTexture();

	void SetPosition(const Vector3f& position) override;
	void SetPosition(float x, float y, float z) override;


	const std::string& GetTextureName() const;
	const std::string& GetDefaultTextureName() const;

private:
	void UpdateOBB() override;

	std::string m_textureName;
	std::string m_defaultTextureName;
};

#endif