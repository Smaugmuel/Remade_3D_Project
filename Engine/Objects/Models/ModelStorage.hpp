#ifndef MODEL_STORAGE_HPP
#define MODEL_STORAGE_HPP
#include "../../Misc/Singleton.hpp"

#include <map>
#include <string>
#include <memory>

class SingleColorModel;
class TextureModel;
class LineModel;

class ModelStorage final : public Singleton<ModelStorage>
{
	friend class Singleton<ModelStorage>;
	ModelStorage();
	~ModelStorage();

public:
	bool LoadSingleColorModel(const std::string& name);
	bool LoadTextureModel(const std::string& name);
	bool CreateLineModel();

	SingleColorModel* GetSingleColorModel(const std::string& name);
	TextureModel* GetTextureModel(const std::string& name);
	LineModel* GetLineModel();

	bool HasSingleColorModel(const std::string& name) const;
	bool HasTextureModel(const std::string& name) const;
	bool HasLineModel() const;

	unsigned int GetNrOfSingleColorModels() const;
	unsigned int GetNrOfTextureModels() const;

	std::string GetSingleColorModelName(unsigned int i) const;
	std::string GetTextureModelName(unsigned int i) const;

private:
	std::map<std::string, std::unique_ptr<SingleColorModel>> m_singleColorModels;
	std::map<std::string, std::unique_ptr<TextureModel>> m_textureModels;
	std::unique_ptr<LineModel> m_lineModel;
};

#endif