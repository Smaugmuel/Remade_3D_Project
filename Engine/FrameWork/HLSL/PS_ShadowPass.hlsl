Texture2D colorTexture : register(t0);
SamplerState samplerState : register(s0);

#ifdef MAX_NR_OF_LIGHTS
Texture2D depthTextures[MAX_NR_OF_LIGHTS];
#endif

cbuffer LightBuffer : register(b0)
{
#ifdef MAX_NR_OF_LIGHTS
	float4 lightData[MAX_NR_OF_LIGHTS];
	float4x4 viewMatrix[MAX_NR_OF_LIGHTS];
	float4x4 projectionMatrix[MAX_NR_OF_LIGHTS];
#endif
	int nrOfLights;
	int3 padding;
};

struct PS_INPUT_DATA
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

float4 main(PS_INPUT_DATA input) : SV_TARGET
{
	float4 color = colorTexture.Sample(samplerState, input.uv);

	return color;
}