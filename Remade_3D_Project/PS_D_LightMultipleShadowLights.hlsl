Texture2D worldPosTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D colorTexture : register(t2);


// Change in SystemInformation.hpp aswell
static const unsigned int MAX_NR_OF_LIGHTS = 8;
Texture2D depthTextures[MAX_NR_OF_LIGHTS];

SamplerState sampleState : register(s0);



struct LightData
{
	float3 position;
	float intensity;
	matrix<float, 4, 4> view;
	matrix<float, 4, 4> proj;
};

cbuffer LightBuffer : register(b0)
{
	LightData lights[MAX_NR_OF_LIGHTS];
	unsigned int nrOfLights;
	int3 padding;
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

float4 main(VS_OUT input) : SV_Target
{
	// ==============================================
	// | Explanation of weight formula (two lights) |
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

	float4 worldPos, normal, color, lightScreenPos;
	float3 toLightDirection;
	float2 lightScreenUV;
	float distance[MAX_NR_OF_LIGHTS], depthToNearestObject, depthToThisObject, weight, divSum = 0.0f, diffuse = 0.0f;
	unsigned int i;

	// Retrieve deferred values
	worldPos = worldPosTexture.Sample(sampleState, input.uv);
	normal = normalTexture.Sample(sampleState, input.uv);
	color = colorTexture.Sample(sampleState, input.uv);

	for (i = 0; i < nrOfLights; i++)
	{
		// Calculate the distance to this light and add the inverse
		distance[i] = length(lights[i].position - worldPos.xyz);
		divSum += 1.0f / distance[i];
	}

	for (i = 0; i < nrOfLights; i++)
	{
		// Project this position to this light
		lightScreenPos = mul(worldPos, mul(lights[i].view, lights[i].proj));
		lightScreenPos /= lightScreenPos.w;

		// Translate fragment positions from [-1, 1] to [0, 1]
		lightScreenUV.x = (lightScreenPos.x + 1) * 0.5f;
		lightScreenUV.y = 1 - (lightScreenPos.y + 1) * 0.5f;

		if (lightScreenUV.x >= 0.0f && lightScreenUV.x <= 1.0f && lightScreenUV.y >= 0.0f && lightScreenUV.y <= 1.0f)
		{
			// This fragment is within this light's frustum

			depthToNearestObject = depthTextures[i].Sample(sampleState, lightScreenUV).x;
			depthToThisObject = lightScreenPos.z - 0.0001f;

			if (depthToThisObject > depthToNearestObject)
			{
				// This fragment wasn't visible from this light and receives no lighting
				continue;
			}
		}

		// This fragment receives lighting due to one of the following
		// a) It was visible from this light
		// b) It was outside this light's frustum

		toLightDirection = normalize(lights[i].position - worldPos.xyz);
		weight = 1.0f / (distance[i] * divSum);
		diffuse += saturate(dot(toLightDirection, normal.xyz)) * lights[i].intensity * weight;
	}

	return float4(color.xyz * saturate(diffuse + 0.05f), 1.0f);
}