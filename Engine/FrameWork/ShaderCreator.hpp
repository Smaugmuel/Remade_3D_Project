#ifndef SHADER_CREATOR_HPP
#define SHADER_CREATOR_HPP
#include <string>
#include <map>
#include <vector>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D10Blob;
struct ID3D11VertexShader;
struct ID3D11HullShader;
struct ID3D11DomainShader;
struct ID3D11GeometryShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct D3D_SHADER_MACRO;
struct D3D11_INPUT_ELEMENT_DESC;

struct VS_And_Layout_Indices
{
	int vsIndex = -1;
	int layoutIndex = -1;
};

//enum class ShaderType
//{
//	VERTEX, GEOMETRY, PIXEL
//};
//enum class InputLayoutFormat
//{
//	FLOAT2, FLOAT3, FLOAT4
//};
//
//struct ShaderMacro
//{
//	std::string name;
//	std::string definition;
//};
//
//struct ShaderDescription
//{
//	std::string name;
//	ShaderType type;
//	std::vector<ShaderMacro> macros;
//};
//
//struct InputLayoutElement
//{
//	std::string name;
//	InputLayoutFormat format;
//};
//
//struct InputLayoutDescription
//{
//	std::vector<InputLayoutElement> elements;
//};
//
//struct Shader
//{
//	ShaderDescription shaderDescription;
//	InputLayoutDescription inputLayoutDescription;
//};

class ShaderCreator final
{
public:
	ShaderCreator();
	~ShaderCreator();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	int CreateInputLayoutFromVertexShader(int vsIndex, unsigned int nrOfInputElements = 0, D3D11_INPUT_ELEMENT_DESC* inputElements = nullptr);
	int CreateVertexShader(const std::string& name, D3D_SHADER_MACRO* macros = nullptr);
	int CreateHullShader(const std::string& name, D3D_SHADER_MACRO* macros = nullptr);
	int CreateDomainShader(const std::string& name, D3D_SHADER_MACRO* macros);
	int CreateGeometryShader(const std::string& name, D3D_SHADER_MACRO* macros);
	int CreatePixelShader(const std::string& name, D3D_SHADER_MACRO* macros);

private:
	ID3D11Device * m_device;
	ID3D11DeviceContext* m_deviceContext;

	std::map<std::string, int> m_vsNameLinker;
	std::map<std::string, int> m_hsNameLinker;
	std::map<std::string, int> m_dsNameLinker;
	std::map<std::string, int> m_gsNameLinker;
	std::map<std::string, int> m_psNameLinker;
	
	std::vector<ID3D10Blob*> m_vsBlobs;
	std::vector<ID3D11VertexShader*> m_vs;
	std::vector<ID3D11HullShader*> m_hs;
	std::vector<ID3D11DomainShader*> m_ds;
	std::vector<ID3D11GeometryShader*> m_gs;
	std::vector<ID3D11PixelShader*> m_ps;
	
	std::vector<ID3D11InputLayout*> m_layouts;
	//std::vector<Shader> m_shaders;
};

#endif