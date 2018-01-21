#ifndef HUD_SHADER_GROUP_HPP
#define HUD_SHADER_GROUP_HPP

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11SamplerState;
struct ID3D11ShaderResourceView;

class HUDShaderGroup
{
public:
	HUDShaderGroup();
	~HUDShaderGroup();

	bool Initialize(ID3D11Device* device);

	void SetupShaders(ID3D11DeviceContext* deviceContext);
	void SetupPerObjectBuffer(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture);

private:
	ID3D11VertexShader* m_vs;
	ID3D11PixelShader* m_ps;

	ID3D11InputLayout* m_layout;

	ID3D11SamplerState* m_samplerState;
};

#endif