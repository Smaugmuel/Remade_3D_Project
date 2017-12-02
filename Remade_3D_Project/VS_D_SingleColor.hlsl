cbuffer PerFrame : register(b0)
{
	matrix<float, 4, 4> view;
	matrix<float, 4, 4> projection;
};
cbuffer Perobject : register(b1)
{
	matrix<float, 4, 4> world;
};

struct VS_IN
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 color : COLOR;
};
struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 worldPosition : POSITION;
	float4 normal : NORMAL;
	float4 color : COLOR;
};


VS_OUT main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.worldPosition = mul(float4(input.position, 1.0f), world);
	output.position = mul(mul(output.worldPosition, view), projection);
	output.normal = float4(normalize(mul(float4(input.normal, 0.0f), world).xyz), 0.0f);
	output.color = float4(input.color, 1.0f);

	return output;
}