Texture2D worldPosTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D colorTexture : register(t2);

SamplerState sampleState : register(s0);

cbuffer LightBuffer : register(b0)
{
	float4 lightData;
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

float4 main(VS_OUT input) : SV_Target
{
	float4 worldPos = worldPosTexture.Sample(sampleState, input.uv);
	float4 normal = normalTexture.Sample(sampleState, input.uv);
	float4 color = colorTexture.Sample(sampleState, input.uv);

	float3 lightPos = lightData.xyz;
	float lightIntensity = lightData.w;

	float3 toLight = normalize(lightPos - worldPos.xyz);
	float diffuse = saturate(dot(toLight, normal.xyz));

	//return normal;
	//return float4(0, 0, 1, 1);
	return float4(color.xyz * saturate(diffuse + 0.1f) * lightIntensity, 1.0f);
}