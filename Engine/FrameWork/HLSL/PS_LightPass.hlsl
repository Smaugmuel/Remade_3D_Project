Texture2D worldPosTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D colorTexture : register(t2);
SamplerState samplerState : register(s0);

/*
Additional resources for lights and shadows
*/
//Texture2D lightDepthTextures[MAX_NR_OF_LIGHTS];
//
//cbuffer PerFrame : register(b0)
//{
//	float4 lightPositions[MAX_NR_OF_LIGHTS];
//	float4x4 lightViewMatrices[MAX_NR_OF_LIGHTS];
//	float4x4 lightProjMatrices[MAX_NR_OF_LIGHTS];
//	uint nrOfLights;
//	uint3 padding;
//};
//#endif

#ifdef MAX_NR_OF_LIGHTS
cbuffer LightBuffer : register(b0)
{
	float4 lights[MAX_NR_OF_LIGHTS];	// Contains position and linear dropoff
	int nrOfLights;
	int3 padding;
};
#endif

struct PS_INPUT_DATA
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

float4 main(PS_INPUT_DATA input) : SV_TARGET
{
	float4 wpos = worldPosTexture.Sample(samplerState, input.uv);
	float4 normal = normalTexture.Sample(samplerState, input.uv);
	float4 color = colorTexture.Sample(samplerState, input.uv);
	float lightInfluence = 0.0f;

#ifdef MAX_NR_OF_LIGHTS
	float3 toLight, lightPosition;
	float influence, lightDropoff;
	for (unsigned int i = 0; i < nrOfLights; i++)
	{
		lightPosition = lights[i].xyz;
		lightDropoff = lights[i].w;

		toLight = lightPosition - wpos.xyz;
		influence = length(toLight) * lightDropoff + 1;
		
		if (influence > 0)
		{
			lightInfluence += saturate(dot(toLight, normal.xyz)) * influence;
		}
	}
#else
	lightInfluence = 1.0f;
#endif

	return color * saturate(lightInfluence);
}