#ifndef MODEL_STORAGE_HPP
#define MODEL_STORAGE_HPP
#include "Singleton.hpp"

#include <map>
#include <string>
#include <memory>

struct ID3D11Device;

class SingleColorModel;
class TextureModel;

class ModelStorage : public Singleton<ModelStorage>
{
	friend class Singleton<ModelStorage>;
	ModelStorage();
	~ModelStorage();

public:
	bool LoadSingleColorModel(ID3D11Device* device, const std::string& name);
	bool LoadTextureModel(ID3D11Device* device, const std::string& name);

	SingleColorModel* GetSingleColorModel(const std::string& name);
	TextureModel* GetTextureModel(const std::string& name);

	bool HasSingleColorModel(const std::string& name) const;
	bool HasTextureModel(const std::string& name) const;

private:
	std::map<std::string, std::unique_ptr<SingleColorModel>> m_singleColorModels;
	std::map<std::string, std::unique_ptr<TextureModel>> m_textureModels;
};

#endif