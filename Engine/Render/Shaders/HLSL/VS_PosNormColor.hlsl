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

//cbuffer Perobject : register(b0)
//{
//	matrix<float, 4, 4> world;
//	float3 color;
//	float padding;
//};

struct VS_IN
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 color : COLOR;
};
struct VS_OUT
{
	float4 position : SV_POSITION;
	float3 worldPosition : POSITION;
	float3 normal : NORMAL;
	float3 color : COLOR;
};


VS_OUT main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	float4 worldPos = mul(float4(input.position, 1.0f), world);
	float4 norm = mul(float4(input.normal, 0.0f), world);

	output.position = mul(mul(worldPos, view), projection);
	output.worldPosition = worldPos.xyz;
	output.normal = normalize(norm.xyz);
	output.color = color;

	return output;
}