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
	matrix<float, 4, 4> projection;
};

cbuffer Color : register(b4)
{
	float3 color;
	float padding;
};

struct VS_IN
{
	float3 position : POSITION;
	float3 color : COLOR;
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output;

	output.position = mul(mul(mul(float4(input.position, 1.0f), world), view), projection);
	output.color = float4(input.color, 1.0f);

	return output;
}