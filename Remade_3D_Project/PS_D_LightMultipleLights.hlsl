Texture2D worldPosTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D colorTexture : register(t2);

SamplerState sampleState : register(s0);


// Change in SystemInformation.hpp aswell
static const unsigned int NR_OF_LIGHTS = 4;


cbuffer LightBuffer : register(b0)
{
	float4 lightData[NR_OF_LIGHTS];
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

float4 main(VS_OUT input) : SV_Target
{
	// =======================================
	// | Explanation of formula (two lights) |
	// =================================================
	// | Calculate the distance to each light: A and B |
	// ===========================================================================
	// | By dividing each distance by the distance sum, a percentage is achieved |
	// ---------------------------------------------------------------------------
	// |    A                 B                                                  |
	// | -------    and    -------                                               |
	// | (A + B)           (A + B)                                               |
	// ===================================================================================
	// | With A = 1 and B = 3 this would give 0.25 and 0.75 respectively                 |
	// | However, a nearer light should have a higher weight, not a lower                |
	// | To achieve this, invert the calculations (far = low weight, near = high weight) |
	// -----------------------------------------------------------------------------------
	// | (A + B)           (A + B)                                                       |
	// | -------    and    -------                                                       |
	// |    A                 B                                                          |
	// ===================================================================================
	// | These values need to add up to 1, which is achieved by normalizing them |
	// ---------------------------------------------------------------------------
	// |      (A + B)                           1                                |
	// |      -------                   (A + B)---                               |
	// |         A                              A                      1         |
	// | -----------------    =    --------------------    =    ---------------  |
	// | (A + B)   (A + B)                 / 1     1 \             / 1     1 \   |
	// | ------- + -------         (A + B)| --- + --- |         A | --- + --- |  |
	// |    A         B                    \ A     B /             \ A     B /   |
	// ===========================================================================


	// Deferred first pass values
	float4 worldPos;
	float4 normal;
	float4 color;

	// Vector from object to light
	float3 toLight;

	float diffuse = 0.0f;
	
	// Distance to each light
	float distance[NR_OF_LIGHTS];

	// Sum of 1 divided by each light distance
	float divSum = 0.0f;

	// Weight of each light on this pixel
	float weight[NR_OF_LIGHTS];
	
	unsigned int i;

	// Retrieve deferred values
	worldPos = worldPosTexture.Sample(sampleState, input.uv);
	normal = normalTexture.Sample(sampleState, input.uv);
	color = colorTexture.Sample(sampleState, input.uv);

	// Calculate distances and divSum
	for (i = 0; i < NR_OF_LIGHTS; i++)
	{
		distance[i] = length(lightData[i].xyz - worldPos.xyz);

		divSum += 1.0f / distance[i];
	}

	for (i = 0; i < NR_OF_LIGHTS; i++)
	{
		// Vector from object to light
		toLight = normalize(lightData[i].xyz - worldPos.xyz);

		// Calculate weights
		weight[i] = 1.0f / (distance[i] * divSum);

		// Add diffuse value
		diffuse += saturate(dot(toLight, normal.xyz)) * lightData[i].w * weight[i];
	}

	return float4(color.xyz * saturate(diffuse + 0.05f), 1.0f);
}