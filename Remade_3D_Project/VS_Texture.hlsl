cbuffer PerFrame : register(b0)
{
	matrix<float, 4, 4> view;
	matrix<float, 4, 4> proj;
};
cbuffer PerObject : register(b1)
{
	matrix<float, 4, 4> world;
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
	float3 normal : NORMAL;
	float2 uv : TEXCOORD0;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output;

	output.position = mul(float4(input.position, 1.0f), world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, proj);
	output.normal = mul(input.normal, (float3x3)world);
	output.uv = input.uv;

	return output;
}