cbuffer PerFrameBuffer : register(b0)
{
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
};

cbuffer PerObjectBuffer : register(b1)
{
	float4x4 worldMatrix;
};

struct INPUT_DATA
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};
struct OUTPUT_DATA
{
	float4 position : SV_POSITION;
	float3 worldPosition : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

OUTPUT_DATA main(INPUT_DATA input)
{
	OUTPUT_DATA output = (OUTPUT_DATA)0;

	output.position = mul(float4(input.position, 1.0f), worldMatrix);
	output.worldPosition = output.position.xyz;
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	
	output.normal = mul(float4(input.normal, 0.0f), worldMatrix).xyz;

	output.uv = input.uv;

	return output;
}