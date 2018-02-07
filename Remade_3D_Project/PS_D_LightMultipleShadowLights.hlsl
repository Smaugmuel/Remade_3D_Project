Texture2D worldPosTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D colorTexture : register(t2);

SamplerState sampleState : register(s0);


// Change in SystemInformation.hpp aswell
static const unsigned int MAX_NR_OF_LIGHTS = 8;
Texture2D depthTextures[MAX_NR_OF_LIGHTS];

struct PointLight
{
	float3 position;
	float intensity;
	matrix<float, 4, 4> view;
	matrix<float, 4, 4> proj;
};

cbuffer LightBuffer : register(b0)
{
	PointLight lights[MAX_NR_OF_LIGHTS];
	int nrOfLights;
	int3 padding;
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
	float distance[MAX_NR_OF_LIGHTS];

	float weight;

	// Sum of 1 divided by each light distance
	float divSum = 0.0f;

	// Object position on screen, from light perspective
	float4 lightScreenPos;

	// UV coords at position above
	float2 lightScreenUV;

	// Distance to light
	float depthToNearestObject;
	float depthToThisObject;


	// Retrieve deferred values
	worldPos = worldPosTexture.Sample(sampleState, input.uv);
	normal = normalTexture.Sample(sampleState, input.uv);
	color = colorTexture.Sample(sampleState, input.uv);

	// Calculate distances and divSum
	for (unsigned int i = 0; i < nrOfLights; i++)
	{
		distance[i] = length(lights[i].position - worldPos.xyz);
	
		divSum += 1.0f / distance[i];
	}

	for (unsigned int i = 0; i < nrOfLights; i++)
	{
		// Vector from object to light
		toLight = normalize(lights[i].position - worldPos.xyz);
	
		// Object position seen from light
		lightScreenPos = mul(worldPos, mul(lights[i].view, lights[i].proj));
		lightScreenPos /= lightScreenPos.w;

		// Translate from [-1, 1] to [0, 1]
		lightScreenUV.x = (lightScreenPos.x + 1) * 0.5f;
		lightScreenUV.y = 1 - (lightScreenPos.y + 1) * 0.5f;

		if (saturate(lightScreenUV.x) == lightScreenUV.x && saturate(lightScreenUV.y) == lightScreenUV.y)
		{
			// Object position fit into light frustum (was visible from light)

			depthToNearestObject = depthTextures[i].Sample(sampleState, lightScreenUV).x;
			depthToThisObject = lightScreenPos.z - 0.0001f;

			if (depthToThisObject <= depthToNearestObject)
			{
				// This was the closest object

				// Calculate weights
				weight = 1.0f / (distance[i] * divSum);

				// Add diffuse lighting
				diffuse += saturate(dot(toLight, normal.xyz)) * weight;
			}
		}
		else
		{
			// Calculate weights
			weight = 1.0f / (distance[i] * divSum);

			// Add diffuse lighting
			diffuse += saturate(dot(toLight, normal.xyz)) * weight;
		}
	}
	
	return float4(color.xyz * saturate(diffuse + 0.05f), 1.0f);
}