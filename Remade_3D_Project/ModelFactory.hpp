#ifndef MODEL_FACTORY_HPP
#define MODEL_FACTORY_HPP
#include <d3d11.h>
#include "Model.hpp"
//#include "Singleton.hpp"

class Model;
struct ID3D11Device;

class ModelFactory
{
public:
	static Model* Create(const char* modelFileName, ID3D11Device* device);

private:
	static bool CheckIfModelHasTexcoords(const char* modelFileName);
};

//	Problems with using code below:
//		The factory needs to be deleted from somewhere (probably Game.cpp)
//		Different file names would map to the same class Create function

//class ModelFactory final : public Singleton<ModelFactory>
//{
//	friend class Singleton<ModelFactory>;
//
//	/*typedef Model* (*CreateFunctionPointer)(const std::string&, ID3D11Device* device);
//	typedef std::map<std::string, CreateFunctionPointer> ModelMap;*/
//
//	ModelFactory();
//	~ModelFactory();
//
//
//public:
//	//void Register(const std::string& modelFileName, CreateFunctionPointer function);
//
//	Model* Create(/*const std::string& modelType,*/ const char* modelFileName, ID3D11Device* device);
//
//private:
//	bool CheckIfModelHasTexcoords(const char* modelFileName);
//
////private:
////	ModelMap m_modelMap;
//};

#endif