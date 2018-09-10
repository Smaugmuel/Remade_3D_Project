#ifndef SHADER_MANAGER_HPP
#define SHADER_MANAGER_HPP
#include <vector>
#include <string>
#include "Shaders/InputLayoutCreator.hpp"
#include "Shaders/ShaderCreatorV2.hpp"

#pragma comment (lib, "d3dcompiler.lib")

struct ID3D11VertexShader;
struct ID3D11GeometryShader;
struct ID3D11PixelShader;

struct ID3D11InputLayout;

struct ID3D11Device;
struct ID3D11DeviceContext;

//enum class VSFlags
//{
//	RECEIVE_NORMAL = 1,
//	RECEIVE_UV = 2,
//	PASS_WPOS = 4
//};

enum class ShaderTypeV2
{
	GEOMETRY_PASS,
	LIGHT_PASS,
	SHADOW_PASS,
	FINAL_PASS
};

static const unsigned int NR_OF_SHADER_TYPES = static_cast<unsigned int>(ShaderTypeV2::FINAL_PASS) + 1U;

class ShaderManagerV2 final
{
public:
	ShaderManagerV2();
	~ShaderManagerV2();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int maxNrOfLights = 1);

	/*int CreateVertexShader(unsigned int flags);
	//int CreateVertexShader(const std::string& fileName);
	int CreateGeometryShader(const std::string& fileName);
	int CreatePixelShader(const std::string& fileName);*/

	/*bool SetVertexShaderToPipeline(int id);
	bool SetGeometryShaderToPipeline(int id);
	bool SetPixelShaderToPipeline(int id);*/

	bool SetShaders(ShaderTypeV2 shaderType);

private:
	/*bool ValidVSID(int id);
	bool ValidGSID(int id);
	bool ValidPSID(int id);*/

	ShaderCreatorV2 m_shaderCreator;
	InputLayoutCreator m_inputLayoutCreator;

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;

	ID3D11VertexShader* m_vertexShaders[NR_OF_SHADER_TYPES];
	ID3D11GeometryShader* m_geometryShaders[NR_OF_SHADER_TYPES];
	ID3D11PixelShader* m_pixelShaders[NR_OF_SHADER_TYPES];

	ID3D11InputLayout* m_inputLayouts[NR_OF_SHADER_TYPES];
};

#endif