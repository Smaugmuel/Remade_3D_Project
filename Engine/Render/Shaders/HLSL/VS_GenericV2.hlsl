#ifdef MATRIX_BUFFER_SIZE
cbuffer MatrixBuffer : register(b0)
{
	float4x4 matrices[MATRIX_BUFFER_SIZE];
	int viewIndex;
	int projIndex;
	int2 padding;
};
cbuffer PerObjectBuffer : register(b1)
{
	int worldIndex;
	int3 padding;
};
#endif

struct IN
{
	float3 pos : POSITION;
#ifdef RECEIVE_NORMAL
	float3 normal : NORMAL;
#endif
#ifdef RECEIVE_UV
	float2 uv : TEXCOORD;
#else
	float3 color : COLOR;
#endif
};

struct OUT
{
	float4 pos : SV_POSITION;
#ifdef PASS_WPOS
	float3 worldPos : POSITION;
#endif
#ifdef RECEIVE_NORMAL
	float3 normal : NORMAL;
#endif
#ifdef RECEIVE_UV
	float2 uv : TEXCOORD;
#else
	float3 color : COLOR;
#endif
};

OUT main(IN input)
{
	OUT output = (OUT)0;

#ifdef MATRIX_BUFFER_SIZE
	output.pos = mul(float4(input.pos, 1.0f), matrices[worldIndex]);

#ifdef PASS_WPOS
	output.worldPos = output.pos;
#endif

	output.pos = mul(output.pos, matrices[viewIndex]);
	output.pos = mul(output.pos, matrices[projIndex]);
#endif

#ifdef RECEIVE_NORMAL
	output.normal = normalize(mul(float4(input.normal, 1.0f), worldMatrix).xyz);
#endif

#ifdef RECEIVE_UV
	output.uv = input.uv;
#else
	output.color = input.color;
#endif

	return output;
}