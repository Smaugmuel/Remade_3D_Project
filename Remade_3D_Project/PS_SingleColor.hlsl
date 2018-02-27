cbuffer PointLight : register(b2)
{
	float4 lightData;
}

struct VS_OUT
{
	float4 position : SV_POSITION;
	float3 worldPosition : POSITION;
	float3 normal : NORMAL;
	float3 color : COLOR;
};

float4 main(VS_OUT input) : SV_Target
{
	float3 toLight = normalize(lightData.xyz - input.worldPosition);
	float diffuse = dot(toLight, input.normal);
	return float4(input.color * diffuse * lightData.w, 1.0f);
}