#ifndef BASE_SHADER_HPP
#define BASE_SHADER_HPP

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11VertexShader;
struct ID3D11GeometryShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;


/*
What belongs with a shader:
VS + layout, HS, DS, GS, PS
Constant buffers via indices
Defines

Defines and constant buffers are optional

*/


/*
A shader class should contain information about its
VS, GS, PS, layout

*/


struct BaseConstantBuffer
{
	BaseConstantBuffer()
	{
	}
	virtual ~BaseConstantBuffer()
	{
	}

};


class BaseShader
{
public:
	BaseShader();
	virtual ~BaseShader();

	virtual bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

protected:
	ID3D11Device * m_device;
	ID3D11DeviceContext* m_deviceContext;

	ID3D11VertexShader* m_vertexShader;
	ID3D11GeometryShader* m_geometryShader;
	ID3D11PixelShader* m_pixelShader;

	ID3D11InputLayout* m_inputLayout;
};

#endif