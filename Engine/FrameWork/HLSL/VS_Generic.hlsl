cbuffer PerFrameBuffer : register(b0)
{
#ifdef CBUFFER_HAS_VIEW_PROJECTION_MATRIX
	float4x4 viewProjectionMatrix;
#endif
};

cbuffer PerObjectBuffer : register(b1)
{
#ifdef CBUFFER_HAS_WORLD_MATRIX
	float4x4 worldMatrix;
#endif
};

struct VS_INPUT_DATA
{
	float3 position : POSITION;
#ifdef VBUFFER_HAS_NORMAL
	float3 normal : NORMAL;
#endif
#ifdef VBUFFER_HAS_UV
	float2 uv : TEXCOORD;
#endif
};

struct VS_OUTPUT_DATA
{
	float4 position : SV_POSITION;
#if defined(CBUFFER_HAS_WORLD_MATRIX) && defined(VBUFFER_PASS_WPOS)
	float3 worldPosition : POSITION;
#endif
#ifdef VBUFFER_HAS_NORMAL
	float3 normal : NORMAL;
#endif
#ifdef VBUFFER_HAS_UV
	float2 uv : TEXCOORD;
#endif
};

VS_OUTPUT_DATA main(VS_INPUT_DATA input)
{
	VS_OUTPUT_DATA output = (VS_OUTPUT_DATA)0;

	output.position = float4(input.position, 1.0f);

#ifdef CBUFFER_HAS_WORLD_MATRIX
	output.position = mul(output.position, worldMatrix);
#ifdef VBUFFER_PASS_WPOS
	output.worldPosition = output.position.xyz;
#endif
#endif

#ifdef CBUFFER_HAS_VIEW_PROJECTION_MATRIX
	output.position = mul(output.position, viewProjectionMatrix);
#endif

#if defined(VBUFFER_HAS_NORMAL) && defined(CBUFFER_HAS_WORLD_MATRIX)
	output.normal = mul(float4(input.normal, 0.0f), worldMatrix).xyz;
#endif

#ifdef VBUFFER_HAS_UV
	output.uv = input.uv;
#endif

	return output;
}