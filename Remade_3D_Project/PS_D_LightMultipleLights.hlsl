Texture2D worldPosTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D colorTexture : register(t2);

SamplerState sampleState : register(s0);


// Change in SystemInformation.hpp aswell
//static const unsigned int NR_OF_LIGHTS = 4;


cbuffer LightBuffer : register(b0)
{
	float3 lightPos;//[NR_OF_LIGHTS];
	float lightIntensity;// [NR_OF_LIGHTS];
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

float4 main(VS_OUT input) : SV_Target
{
	// Deferred first pass values
	float4 worldPos;
	float4 normal;
	float4 color;

	// Vector from object to light
	float3 toLight;

	float diffuse = 0.0f;


	// Retrieve deferred values
	worldPos = worldPosTexture.Sample(sampleState, input.uv);
	normal = normalTexture.Sample(sampleState, input.uv);
	color = colorTexture.Sample(sampleState, input.uv);

	//for (unsigned int i = 0; i < 1; i++)
	//{
	//	// Vector from object to light
	//	toLight = normalize(lightPos[i] - worldPos.xyz);

	//	diffuse += saturate(dot(toLight, normal.xyz)) * lightIntensity[i];
	//}

	toLight = normalize(lightPos - worldPos.xyz);

	diffuse += saturate(dot(toLight, normal.xyz));

	return float4(color.xyz * saturate(diffuse + 0.1f), 1.0f);
}