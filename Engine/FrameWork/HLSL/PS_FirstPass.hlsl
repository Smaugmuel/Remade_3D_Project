Texture2D tex : register(t0);
SamplerState samplerState : register(s0);

struct INPUT_DATA
{
	float4 position : SV_POSITION;
	float3 worldPosition : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

struct OUTPUT_DATA
{
	float4 worldPosition : SV_Target0;
	float4 normal : SV_Target1;
	float4 color : SV_Target2;
};

OUTPUT_DATA main(INPUT_DATA input)
{
	OUTPUT_DATA output = (OUTPUT_DATA)0;

	output.worldPosition = float4(input.worldPosition, 1.0f);
	output.normal = float4(input.normal, 1.0f);
	output.color = tex.Sample(samplerState, input.uv);

	return output;
}