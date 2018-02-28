Texture2D worldPosTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D colorTexture : register(t2);
Texture2D depthTexture : register(t3);

SamplerState sampleState : register(s0);


cbuffer View : register(b0)
{
	matrix<float, 4, 4> lightView;
};
cbuffer Projection : register(b1)
{
	matrix<float, 4, 4> lightProj;
};
cbuffer PointLight : register(b2)
{
	float4 lightData;
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

float4 main(VS_OUT input) : SV_Target
{
	// Deferred first pass values
	float4 worldPos;
	float4 normal;
	float4 color;

	// Light values
	float3 lightPos;
	float lightIntensity;

	// Vector from object to light
	float3 toLight;

	// Object position on screen, from light perspective
	float4 lightScreenPos;
	float w;

	// UV coords at position above
	float2 lightScreenUV;

	// Distance to light
	float depthToNearestObject;
	float depthToThisObject;

	float diffuse = 0.0f;


	// Retrieve deferred values
	worldPos = worldPosTexture.Sample(sampleState, input.uv);
	normal = normalTexture.Sample(sampleState, input.uv);
	color = colorTexture.Sample(sampleState, input.uv);

	// Retrieve light values
	lightPos = lightData.xyz;
	lightIntensity = lightData.w;

	// Vector from object to light
	toLight = normalize(lightPos - worldPos.xyz);

	// Object position seen from light
	lightScreenPos = mul(worldPos, mul(lightView, lightProj));
	w = lightScreenPos.w;
	lightScreenPos /= lightScreenPos.w;

	// Translate from [-1, 1] to [0, 1]
	lightScreenUV.x = (lightScreenPos.x + 1) * 0.5f;
	lightScreenUV.y = 1 - (lightScreenPos.y + 1) * 0.5f;
	
	if (w > 0 && lightScreenUV.x >= 0.0f && lightScreenUV.x <= 1.0f && lightScreenUV.y >= 0.0f && lightScreenUV.y <= 1.0f)
	{
		// Object position fit into light frustum (was visible from light)

		depthToNearestObject = depthTexture.Sample(sampleState, lightScreenUV).x;
		depthToThisObject = lightScreenPos.z - 0.0001f;

		if (depthToThisObject <= depthToNearestObject)
		{
			// This was the closest object

			// Add diffuse lighting
			diffuse += saturate(dot(toLight, normal.xyz));
		}
	}
	else
	{
		// This object was outside light frustum

		// Add diffuse lighting
		diffuse += saturate(dot(toLight, normal.xyz));
	}

	return float4(color.xyz * saturate(diffuse + 0.1f), 1.0f);
}