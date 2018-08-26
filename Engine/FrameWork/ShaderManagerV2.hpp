#ifndef SHADER_MANAGER_HPP
#define SHADER_MANAGER_HPP
#include <vector>
#include <string>

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
	FIRST_PASS,
	LIGHT_PASS,
	SHADOW_PASS
};

class ShaderManagerV2 final
{
public:
	ShaderManagerV2();
	~ShaderManagerV2();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

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

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;

	ID3D11VertexShader* m_vertexShaders[2];
	ID3D11GeometryShader* m_geometryShaders[2];
	ID3D11PixelShader* m_pixelShaders[2];

	ID3D11InputLayout* m_inputLayouts[2];
};

#endif