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
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 depthPosition : TEXTURE0;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.position = mul(float4(input.position, 1.0f), world);
	output.position = mul(mul(output.position, view), proj);
	output.depthPosition = output.position;

	return output;
}