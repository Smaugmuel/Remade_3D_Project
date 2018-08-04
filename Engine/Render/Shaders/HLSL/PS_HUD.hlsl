Texture2D tex : register(t0);
SamplerState sampleState : register(s0);

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

float4 main(VS_OUT input) : SV_TARGET
{
	float4 textureColor;

	textureColor = tex.Sample(sampleState, input.uv);

	return textureColor;
}