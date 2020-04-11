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

	/*
	One idea which might be faster (but probably not) is to only set three textures as output from this shader
	texture1.rgb = input.worldPosition
	texture1.a = input.normal.x
	texture2.r = input.normal.y
	texture2.gba = input.color

	Then, in the second pass, the normal is recreated since it has a length of 1
	However, this would require the calculation z = sqrt(1 - x^2 + y^2) for each fragment, which seems more expensive than rendering to the third texture
	This should be tested though, before deeming it a bad solution
	*/

	output.worldPosition = float4(input.worldPosition, 1.0f);
	output.normal = float4(input.normal, 0.0f);
	output.color = tex.Sample(samplerState, float2(input.uv.x, 1 - input.uv.y));

	return output;
}