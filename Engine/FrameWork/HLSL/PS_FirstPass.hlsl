Texture2D tex : register(t0);
SamplerState samplerState : register(s0);

struct PS_INPUT_DATA
{
	float4 position : SV_POSITION;
	float3 worldPosition : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

struct PS_OUTPUT_DATA
{
	float4 worldPosition : SV_Target0;
	float4 normal : SV_Target1;
	float4 color : SV_Target2;
};

PS_OUTPUT_DATA main(PS_INPUT_DATA input)
{
	PS_OUTPUT_DATA output = (PS_OUTPUT_DATA)0;

	output.worldPosition = float4(input.worldPosition, 1.0f);
	output.normal = float4(input.normal, 1.0f);
	output.color = tex.Sample(samplerState, input.uv);

	return output;
}