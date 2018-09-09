/*
Constant buffer for everything sent once per frame
*/
cbuffer PerFrame : register(b0)
{
#ifdef VIEW_MATRIX
	matrix<float, 4, 4> view;
#endif
#ifdef PROJECTION_MATRIX
	matrix<float, 4, 4> projection;
#endif
};

/*
Constant buffer for everything sent once per object
*/
cbuffer PerObject : register(b1)
{
#ifdef WORLD_MATRIX
	matrix<float, 4, 4> world;
#endif
};

/*
Constant buffer for an array of matrices sent once per frame
*/
#ifdef NR_OF_MATRICES
cbuffer MatrixBuffer : register(b2)
{
	matrix<float, 4, 4> buffer[NR_OF_MATRICES];
};
#endif

/*
Vertex buffer input for an object
*/
struct IN
{
#ifdef INPUT_POS
	float3 position : POSITION;
#endif
#ifdef INPUT_NORMAL
	float3 normal : NORMAL;
#endif
#ifdef INPUT_COLOR
	float3 color : COLOR;
#endif
#ifdef INPUT_UV
	float2 uv : TEXCOORD0;
#endif
};

/*
Output to the next shader for an object
*/
struct OUT
{
#ifdef OUTPUT_POS
	float4 position : SV_POSITION;
#endif
#ifdef WORLD_MATRIX
	#ifdef OUTPUT_WORLD_POS
		float4 worldPosition : POSITION;
	#endif
#endif
#ifdef OUTPUT_NORMAL
	float4 normal : NORMAL;
#endif
#ifdef OUTPUT_COLOR
	float4 color : COLOR;
#endif
#ifdef OUTPUT_UV
	float2 uv : TEXCOORD0;
#endif
};

OUT main(IN input)
{
	OUT output = (OUT)0;

	// Position													What this shader at least has
#ifdef INPUT_POS												// In pos
	#ifdef OUTPUT_POS											// In pos, out pos
		output.position = float4(input.position, 1.0f);
		#ifdef WORLD_MATRIX										// In pos, out pos, world matrix
			output.position = mul(output.position, world);
			#ifdef OUTPUT_WORLD_POS								// In pos, out pos, world matrix, world pos
				output.worldPosition = output.position.xyz;
			#endif
		#endif
		#ifdef VIEW_MATRIX										// In pos, out pos, view matrix
			output.position = mul(output.position, view);
		#endif
		#ifdef PROJECTION_MATRIX								// In pos, out pos, proj matrix
			output.position = mul(output.position, projection);
		#endif
	#endif
#elif OUTPUT_POS												// Out pos
	output.position = float4(0.0f, 0.0f, 0.0f, 1.0f);
	#ifdef WORLD_MATRIX											// Out pos, world matrix
		output.position = mul(output.position, world);
		#ifdef OUTPUT_WORLD_POS									// Out pos, world, world pos
			output.worldPosition = output.position.xyz;
		#endif
	#endif
	#ifdef VIEW_MATRIX											// Out pos, view matrix
		output.position = mul(output.position, view);
	#endif
	#ifdef PROJECTION_MATRIX									// Out pos, proj matrix
		output.position = mul(output.position, projection);
	#endif
#endif

	// Normal													What this shader at least has
#ifdef INPUT_NORMAL												// In normal
	#ifdef OUTPUT_NORMAL										// In normal, out normal
		output.normal = float4(input.normal, 0.0f);
		#ifdef WORLD_MATRIX										// In normal, out normal, world matrix
			output.normal = mul(output.normal, world);
		#endif
	#endif
#elif OUTPUT_NORMAL												// Out normal
	output.normal = float4(1.0f, 0.0f, 0.0f, 0.0f);
	#ifdef WORLD_MATRIX											// Out normal, world matrix
		output.normal = mul(output.normal, world);
	#endif
#endif

	// Color													What this shader at least has
#ifdef INPUT_COLOR												// In color
	#ifdef OUTPUT_COLOR											// In color, out color
		output.color = float4(input.color, 1.0f);
	#endif
#elif OUTPUT_COLOR												// Out color
	output.color = float4(0.0f, 0.0f, 0.0f, 1.0f);
#endif

	// UV														What this shader at least has
#ifdef INPUT_UV													// In uv
	#ifdef OUTPUT_UV											// In uv, out uv
	output.uv = input.uv;
	#endif
#elif OUTPUT_UV													// Out uv
	output.uv = float2(0.0f, 0.0f);
#endif

	return output;
}