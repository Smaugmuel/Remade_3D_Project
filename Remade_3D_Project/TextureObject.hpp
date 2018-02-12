#ifndef TEXTURE_OBJECT_HPP
#define TEXTURE_OBJECT_HPP
#include "Object.hpp"

class TextureObject : public Object
{
public:
	TextureObject();
	~TextureObject();

	bool Initialize() override;

	void SetTextureName(const std::string& name);

	const std::string& GetTextureName() const;

private:
	std::string m_textureName;
};

#endif