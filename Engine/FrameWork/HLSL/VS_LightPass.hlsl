struct VS_INPUT_DATA
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
};

struct VS_OUTPUT_DATA
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

VS_OUTPUT_DATA main(VS_INPUT_DATA input)
{
	VS_OUTPUT_DATA output = (VS_OUTPUT_DATA)0;

	output.position = float4(input.position, 1.0f);
	output.uv = input.uv;

	return output;
}