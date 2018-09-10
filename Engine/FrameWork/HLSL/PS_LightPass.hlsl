Texture2D worldPosTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D colorTexture : register(t2);
SamplerState samplerState : register(s0);

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
	float4 wpos = worldPosTexture.Load(int3(input.position.xy, 0));
	float4 normal = normalTexture.Load(int3(input.position.xy, 0));
	float4 color = colorTexture.Load(int3(input.position.xy, 0));
	float totalLightInfluence = 0.0f;

#ifdef MAX_NR_OF_LIGHTS
	float3 toLight, lightPosition;
	float lightInfluence, lightDropoff;
	for (unsigned int i = 0; i < nrOfLights; i++)
	{
		lightPosition = lights[i].xyz;
		lightDropoff = lights[i].w;

		toLight = lightPosition - wpos.xyz;
		lightInfluence = length(toLight) * lightDropoff + 1;
		
		if (lightInfluence > 0)
		{
			totalLightInfluence += saturate(dot(toLight, normal.xyz)) * lightInfluence;
		}
	}
#else
	totalLightInfluence = 1.0f;
#endif

	return color * saturate(totalLightInfluence);
}