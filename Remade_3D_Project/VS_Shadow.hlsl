cbuffer PerFrame1 : register(b0)
{
	matrix<float, 4, 4> world;
	matrix<float, 4, 4> view;
	matrix<float, 4, 4> proj;
	matrix<float, 4, 4> lightView;
	matrix<float, 4, 4> lightProj;
};

cbuffer PerFrame2 : register(b1)
{
	float3 lightPosition;
	float padding;
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
	float3 normal : NORMAL;
	float3 color : COLOR;
	float4 lightViewPosition : TEXCOORD0;
	float3 toLight : TEXCOORD1;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;
	float4 worldPosition;

	worldPosition = mul(float4(input.position, 1.0f), world);
	output.position = mul(worldPosition, view);
	output.position = mul(output.position, proj);

	output.lightViewPosition = mul(worldPosition, lightView);
	output.lightViewPosition = mul(output.lightViewPosition, lightProj);

	output.normal = mul(input.normal, (float3x3)world);
	output.normal = normalize(output.normal);

	output.color = input.color;

	output.toLight = lightPosition - worldPosition.xyz;
	output.toLight = normalize(output.toLight);

	return output;
}