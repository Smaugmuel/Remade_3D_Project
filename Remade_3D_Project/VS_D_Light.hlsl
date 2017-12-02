//cbuffer PerFrame : register(b0)
//{
//	matrix<float, 4, 4> view;
//	matrix<float, 4, 4> projection;
//};
//
//cbuffer PerObject : register(b1)
//{
//	matrix<float, 4, 4> world;
//};

struct VS_IN
{
	float3 position : POSITION;
	float2 uv : TEXCOORD0;
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output;
	
	output.position = float4(input.position, 1.0f);
	output.uv = input.uv;

	return output;
}