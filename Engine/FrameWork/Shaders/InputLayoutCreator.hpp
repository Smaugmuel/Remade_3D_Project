#ifndef INPUT_LAYOUT_CREATOR_HPP
#define INPUT_LAYOUT_CREATOR_HPP

struct ID3D11Device;
struct ID3D10Blob;
struct ID3D11InputLayout;

enum class ElementFormat
{
	FLOAT1, FLOAT2, FLOAT3, FLOAT4
};

struct InputElement
{
	const char* semanticName;
	ElementFormat format;
};

class InputLayoutCreator final
{
public:
	InputLayoutCreator();
	~InputLayoutCreator();

	bool Initialize(ID3D11Device* device);

	ID3D11InputLayout* CreateInputLayout(ID3D10Blob* vsBlob, int nrOfElements = 0, InputElement* elements = nullptr);

private:
	ID3D11Device * m_device;
};

#endif