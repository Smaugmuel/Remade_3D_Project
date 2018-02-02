#ifndef SHADER_STORAGE_HPP
#define SHADER_STORAGE_HPP
#include "Singleton.hpp"

#include <map>
#include <memory>

struct ID3D11VertexShader;
//struct ID3D11GeometryShader;
struct ID3D11PixelShader;
struct ID3D11Device;
struct ID3D11InputLayout;

class ShaderStorage : public Singleton<ShaderStorage>
{
	friend class Singleton<ShaderStorage>;

	struct VS_Components
	{
		ID3D11VertexShader* vs = nullptr;
		ID3D11InputLayout* layout = nullptr;
	};

	typedef std::map<std::string, VS_Components> VSMap;
	//typedef std::map<std::string, ID3D11GeometryShader*> GSMap;
	typedef std::map<std::string, ID3D11PixelShader*> PSMap;

	ShaderStorage();
	~ShaderStorage();

public:
	bool Initialize(ID3D11Device* device);
	/*bool CreateVertexShader(ID3D11Device* device, wchar_t* shaderName);
	bool CreateGeometryShader(ID3D11Device* device, wchar_t* shaderName);
	bool CreatePixelShader(ID3D11Device* device, wchar_t* shaderName);*/
	//Shader* GetShader(wchar_t* shaderName) const;

private:
	bool LoadShader(wchar_t* shaderName);

	VSMap vsMap;
	//GSMap gsMap;
	PSMap psMap;

	//ShaderMap m_shaderMap;
};

#endif