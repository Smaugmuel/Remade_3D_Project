cbuffer World : register(b0)
{
	matrix<float, 4, 4> world;
};

cbuffer View : register(b1)
{
	matrix<float, 4, 4> view;
};

cbuffer Projection : register(b2)
{
	matrix<float, 4, 4> proj;
};

cbuffer PointLight : register(b3)
{
	float4 lightData;
};

struct VS_IN
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD0;
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL0;
	float3 toLight : NORMAL1;
	float2 uv : TEXCOORD0;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output;

	output.position = mul(float4(input.position, 1.0f), world);

	output.toLight = normalize(lightData.xyz - output.position.xyz);

	output.position = mul(output.position, view);
	output.position = mul(output.position, proj);
	output.normal = normalize(mul(input.normal, (float3x3)world));
	output.uv = input.uv;

	return output;
}