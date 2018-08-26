#ifndef MODEL_STORAGE_V2_HPP
#define MODEL_STORAGE_V2_HPP
//#include "../../Misc/Singleton.hpp"

#include <vector>
#include <string>
#include <memory>

class SingleColorModel;
class TextureModel;
class LineModel;

class FrameWork;

class ModelStorageV2 final //: public Singleton<ModelStorageV2>
{
public:
	ModelStorageV2();
	~ModelStorageV2();

	bool Initialize(FrameWork* frameWork);

	int LoadSingleColorModel(const std::string& name);
	int LoadTextureModel(const std::string& name);
	bool CreateLineModel();

	int GetSingleColorModelIndexFromName(const std::string& name);
	int GetTextureModelIndexFromName(const std::string& name);
	std::string GetSingleColorModelNameFromIndex(int index);
	std::string GetTextureModelNameFromIndex(int index);

	SingleColorModel* GetSingleColorModel(int index);
	//SingleColorModel* GetSingleColorModelByName(const std::string& name);
	
	TextureModel* GetTextureModel(int index);
	//TextureModel* GetTextureModelByName(const std::string& name);
	
	LineModel* GetLineModel();

	unsigned int GetNrOfSingleColorModels() const;
	unsigned int GetNrOfTextureModels() const;

private:
	//friend class Singleton<ModelStorageV2>;

	struct S_C_Model
	{
		std::string name = "";
		std::unique_ptr<SingleColorModel> model = nullptr;
	};
	struct T_Model
	{
		std::string name = "";
		std::unique_ptr<TextureModel> model = nullptr;
	};


	FrameWork* m_frameWork;

	std::vector<S_C_Model> m_singleColorModels;
	std::vector<T_Model> m_textureModels;

	std::unique_ptr<LineModel> m_lineModel;
};

#endif