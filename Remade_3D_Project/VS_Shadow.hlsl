cbuffer PerFrame : register(b0)
{
	matrix<float, 4, 4> view;
	matrix<float, 4, 4> proj;
	matrix<float, 4, 4> lightView;
	matrix<float, 4, 4> lightProj;
	float3 lightPosition;
	float padding1;
};

cbuffer PerObject : register(b1)
{
	matrix<float, 4, 4> world;
	float3 color;
	float padding2;
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
	float3 normal : NORMAL;
	float4 color : COLOR;
	float4 lightViewPosition : TEXCOORD0;
	float3 toLight : TEXCOORD2;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.worldPosition = mul(float4(input.position, 1.0f), world);
	output.position = mul(mul(output.worldPosition, view), proj);
	output.lightViewPosition = mul(mul(output.worldPosition, lightView), lightProj);
	output.toLight = normalize(lightPosition - output.worldPosition.xyz);
	output.color = float4(input.color, 1.0f);
	output.normal = normalize(mul(input.normal, (float3x3)world));

	return output;
}