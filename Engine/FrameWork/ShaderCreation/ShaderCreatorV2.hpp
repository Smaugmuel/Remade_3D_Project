#ifndef SHADER_CREATOR_V2_HPP
#define SHADER_CREATOR_V2_HPP

struct ID3D11Device;
struct ID3D11VertexShader;
struct ID3D11HullShader;
struct ID3D11DomainShader;
struct ID3D11GeometryShader;
struct ID3D11PixelShader;
struct ID3D10Blob;

struct ShaderDefine
{
	const char* name = nullptr;
	const char* value = nullptr;
};

struct VertexShaderData
{
	ID3D11VertexShader* vs = nullptr;
	ID3D10Blob* blob = nullptr;
};

class ShaderCreatorV2
{
public:
	ShaderCreatorV2();
	~ShaderCreatorV2();

	bool Initialize(ID3D11Device* device);

	VertexShaderData CompileAndCreateVertexShader(int nrOfDefines = 0, ShaderDefine* defines = nullptr);
	ID3D11GeometryShader* CompileAndCreateGeometryShaderFromFile(const char* fileName, int nrOfDefines = 0, ShaderDefine* defines = nullptr);
	ID3D11PixelShader* CompileAndCreatePixelShaderFromFile(const char* fileName, int nrOfDefines = 0, ShaderDefine* defines = nullptr);

private:
	ID3D11Device* m_device;
};

#endif