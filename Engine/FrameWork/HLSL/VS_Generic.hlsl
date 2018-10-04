cbuffer PerFrameBuffer : register(b0)
{
#ifdef CBUFFER_VIEW_PROJECTION_MATRIX
	float4x4 viewProjectionMatrix;
#endif
};

cbuffer PerObjectBuffer : register(b1)
{
#ifdef CBUFFER_WORLD_MATRIX
	float4x4 worldMatrix;
#else if defined(CBUFFER_NR_OF_MATRICES_PER_BUFFER)
	int4 index;
#endif
};

cbuffer MatrixBuffer : register(b2)
{
#ifdef CBUFFER_NR_OF_MATRICES_PER_BUFFER
	float4x4 worldMatrices[CBUFFER_NR_OF_MATRICES_PER_BUFFER];
#endif
};

struct VS_INPUT_DATA
{
	float3 position : POSITION;
#ifdef VBUFFER_NORMAL
	float3 normal : NORMAL;
#endif
#ifdef VBUFFER_UV
	float2 uv : TEXCOORD;
#endif
};

struct VS_OUTPUT_DATA
{
#ifdef PASS_TO_GEOMETRY_SHADER
	float3 position : POSITION;
#else
	float4 position : SV_POSITION;
#endif
#ifdef VBUFFER_PASS_WPOS
	float3 worldPosition : POSITION;
#endif
#ifdef VBUFFER_NORMAL
	float3 normal : NORMAL;
#endif
#ifdef VBUFFER_UV
	float2 uv : TEXCOORD;
#endif
};

VS_OUTPUT_DATA main(VS_INPUT_DATA input)
{
	VS_OUTPUT_DATA output = (VS_OUTPUT_DATA)0;

	float4 position = float4(input.position, 1.0f);
	//output.position = float4(input.position, 1.0f);

#ifdef CBUFFER_WORLD_MATRIX
	//output.position = mul(output.position, worldMatrix);
	position = mul(position, worldMatrix);
#elif defined(CBUFFER_NR_OF_MATRICES_PER_BUFFER)
	//output.position = mul(output.position, worldMatrices[index.x]);
	position = mul(position, worldMatrices[index.x]);
#endif
#ifdef VBUFFER_PASS_WPOS
	//output.worldPosition = output.position.xyz;
	output.worldPosition = position.xyz;
#endif

#ifdef CBUFFER_VIEW_PROJECTION_MATRIX
	//output.position = mul(output.position, viewProjectionMatrix);
	position = mul(position, viewProjectionMatrix);
#endif

#ifdef PASS_TO_GEOMETRY_SHADER
	output.position = position.xyz;
#else
	output.position = position;
#endif


#ifdef VBUFFER_NORMAL
#ifdef CBUFFER_WORLD_MATRIX
	output.normal = mul(float4(input.normal, 0.0f), worldMatrix).xyz;
#elif defined(CBUFFER_NR_OF_MATRICES_PER_BUFFER)
	output.normal = mul(float4(input.normal, 0.0f), worldMatrices[index.x]).xyz;
#endif
#endif

#ifdef VBUFFER_UV
	output.uv = input.uv;
#endif

	return output;
}