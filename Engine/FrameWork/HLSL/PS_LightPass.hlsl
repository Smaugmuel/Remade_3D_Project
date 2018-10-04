Texture2D worldPosTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D colorTexture : register(t2);
SamplerState samplerState : register(s0);

#ifdef CBUFFER_MAX_NR_OF_LIGHTS
struct Light
{
	float3 position;
	float dropoff;
	float3 diffuseColor;
	float padding;
};
cbuffer LightBuffer : register(b0)
{
	Light lights[CBUFFER_MAX_NR_OF_LIGHTS];	// Contains position and linear dropoff
	float3 ambientColor;
	int nrOfLights;
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
	/*float4 wpos = worldPosTexture.Load(int3(input.position.xy, 0));
	float4 normal = normalTexture.Load(int3(input.position.xy, 0));
	float4 color = colorTexture.Load(int3(input.position.xy, 0));*/

	/*
	Default colors with no lights
	*/
	float3 ambientInfluence = float3(0.1, 0.1, 0.1);
	float3 diffuseInfluence = float3(0, 0, 0);

#ifdef CBUFFER_MAX_NR_OF_LIGHTS
	float3 toLight;
	float influence;

	for (unsigned int i = 0; i < nrOfLights; i++)
	{
		toLight = lights[i].position - wpos.xyz;
		influence = length(toLight) * lights[i].dropoff + 1;

		if (influence > 0)
		{
			float diffuseFactor = saturate(dot(normalize(toLight), normal.xyz));
			diffuseInfluence += lights[i].diffuseColor * diffuseFactor * influence;
		}
	}

	/*
	Ambient color
	*/
	ambientInfluence = ambientColor;
#endif

	return color * float4(saturate(ambientInfluence + diffuseInfluence), 1);
}