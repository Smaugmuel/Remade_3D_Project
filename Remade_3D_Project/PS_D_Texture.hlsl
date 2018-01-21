Texture2D tex : register(t0);
SamplerState sampleState : register(s0);

struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 worldPosition : POSITION;
	float4 normal : NORMAL;
	float2 uv : TEXCOORD;
};

struct PS_OUT
{
	float4 worldPosition : SV_Target0;
	float4 normal : SV_Target1;
	float4 color : SV_Target2;
};

PS_OUT main(VS_OUT input)
{
	PS_OUT output;

	output.worldPosition = input.worldPosition;
	output.normal = input.normal;
	output.color = tex.Sample(sampleState, input.uv);

	return output;
}