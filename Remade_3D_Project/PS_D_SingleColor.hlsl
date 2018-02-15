struct VS_OUT
{
	float4 position : SV_POSITION;
	float3 worldPosition : POSITION;
	float3 normal : NORMAL;
	float3 color : COLOR;
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

	output.worldPosition = float4(input.worldPosition, 1.0f);
	output.normal = float4(input.normal, 0.0f);
	output.color = float4(input.color, 1.0f);

	return output;
}