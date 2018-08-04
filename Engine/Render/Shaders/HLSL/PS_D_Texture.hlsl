Texture2D tex : register(t0);
SamplerState sampleState : register(s0);

struct PS_IN
{
	float4 position : SV_POSITION;
	float3 worldPosition : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

struct PS_OUT
{
	float4 worldPosition : SV_Target0;
	float4 normal : SV_Target1;
	float4 color : SV_Target2;
};

PS_OUT main(PS_IN input)
{
	PS_OUT output;

	output.worldPosition = float4(input.worldPosition, 1.0f);
	
	// Recreate the z-value
	//float z = sqrt(1 - (input.normal.x * input.normal.x + input.normal.y * input.normal.y));
	output.normal = float4(input.normal, 0.0f);

	output.color = tex.Sample(sampleState, input.uv);

	return output;
}