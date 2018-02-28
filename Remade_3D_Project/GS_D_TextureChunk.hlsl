// If changed, change in ChunkRenderSettings.hpp aswell
static const int CHUNK_SIZE = 28;

cbuffer View : register(b1)
{
	matrix<float, 4, 4> view;
};

cbuffer Projection : register(b2)
{
	matrix<float, 4, 4> projection;
};

cbuffer WorldArray : register(b3)
{
	matrix<float, 4, 4> worlds[CHUNK_SIZE];
};

cbuffer NrOfObjects : register(b4)
{
	int4 n;
};

struct GS_IN
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD0;
};

struct GS_OUT
{
	float4 position : SV_POSITION;
	float3 worldPosition : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD0;
};

[maxvertexcount(3 * CHUNK_SIZE)]
void main(triangle GS_IN input[3], inout TriangleStream<GS_OUT> outputStream)
{
	GS_OUT output;

	matrix<float, 4, 4> viewProj = mul(view, projection);

	for (unsigned int i = 0; i < n.x; i++)
	{
		for (unsigned int j = 0; j < 3; j++)
		{
			float4 worldPos = mul(float4(input[j].position, 1.0f), worlds[i]);
			output.position = mul(worldPos, viewProj);

			// Send the minumum amount of required information
			output.worldPosition = worldPos.xyz;
			output.normal = normalize(mul(float4(input[j].normal, 1.0f), worlds[i]));

			output.uv = input[j].uv;

			outputStream.Append(output);
		}
		outputStream.RestartStrip();
	}
}