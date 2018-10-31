cbuffer PerFrameBuffer : register(b0)
{
#ifdef CBUFFER_VIEW_PROJECTION_MATRIX
	float4x4 viewProjectionMatrix;
#endif
};

cbuffer PerCallBuffer : register(b1)
{
#ifdef CBUFFER_NR_OF_MATRICES_PER_BUFFER
	int4 amounts;
	// x : The start index of the matrices to read from during this pass
	// y : The number of matrices to read during this pass
#endif
};

cbuffer MatrixBuffer : register(b2)
{
#ifdef CBUFFER_NR_OF_MATRICES_PER_BUFFER
	float4x4 worldMatrices[CBUFFER_NR_OF_MATRICES_PER_BUFFER];
#endif
	float4x4 worldMatrices[1024];
};

struct GS_INPUT_DATA
{
	float3 position : POSITION;
#ifdef VBUFFER_NORMAL
	float3 normal : NORMAL;
#endif
#ifdef VBUFFER_UV
	float2 uv : TEXCOORD;
#endif
};

struct GS_OUTPUT_DATA
{
	float4 position : SV_POSITION;
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

#ifdef MAX_INSTANCING_AMOUNT
[maxvertexcount(3 * MAX_INSTANCING_AMOUNT)]
#else
[maxvertexcount(3)]
#endif
void main(triangle GS_INPUT_DATA input[3], inout TriangleStream<GS_OUTPUT_DATA> outputStream)
{
	GS_OUTPUT_DATA outputs[3];

	/*
	Explanation:
	The matrix buffer holds 1024 matrices
	The amount buffer tells where this geometry pass should start retreiving matrices,
	and how many it should retrieve

	I.e.
	Duplicate this object 28 times, starting at 0 (i : 0-27)
	Duplicate this object 28 times, starting at 28 (i : 28-55)
	*/


#ifdef CBUFFER_NR_OF_MATRICES_PER_BUFFER
	for (uint i = amounts.x; i < amounts.x + amounts.y; i++)
	{
		outputs[0].position = mul(float4(input[0].position, 1.0f), worldMatrices[i]);
		outputs[1].position = mul(float4(input[1].position, 1.0f), worldMatrices[i]);
		outputs[2].position = mul(float4(input[2].position, 1.0f), worldMatrices[i]);

#ifdef VBUFFER_PASS_WPOS
		outputs[0].worldPosition = outputs[0].position.xyz;
		outputs[1].worldPosition = outputs[1].position.xyz;
		outputs[2].worldPosition = outputs[2].position.xyz;
#endif
#ifdef CBUFFER_VIEW_PROJECTION_MATRIX
		outputs[0].position = mul(outputs[0].position, viewProjectionMatrix);
		outputs[1].position = mul(outputs[1].position, viewProjectionMatrix);
		outputs[2].position = mul(outputs[2].position, viewProjectionMatrix);
#endif
#ifdef VBUFFER_NORMAL
		outputs[0].normal = normalize(mul(float4(input[0].normal, 0.0f), worldMatrices[i]).xyz);
		outputs[1].normal = normalize(mul(float4(input[1].normal, 0.0f), worldMatrices[i]).xyz);
		outputs[2].normal = normalize(mul(float4(input[2].normal, 0.0f), worldMatrices[i]).xyz);
#endif
#ifdef VBUFFER_UV
		outputs[0].uv = input[0].uv;
		outputs[1].uv = input[1].uv;
		outputs[2].uv = input[2].uv;
#endif

		outputStream.Append(outputs[0]);
		outputStream.Append(outputs[1]);
		outputStream.Append(outputs[2]);
		outputStream.RestartStrip();
	}
#endif
}