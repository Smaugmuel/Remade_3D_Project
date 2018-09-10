Texture2D colorTexture : register(t0);
SamplerState samplerState : register(s0);

struct PS_INPUT_DATA
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

float4 main(PS_INPUT_DATA input) : SV_TARGET
{
	return colorTexture.Sample(samplerState, input.uv);
}