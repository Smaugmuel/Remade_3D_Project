Texture2D worldPosTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D colorTexture : register(t2);
Texture2D depthTexture : register(t3);

SamplerState sampleState : register(s0);

cbuffer LightBuffer : register(b0)
{
	float4 lightData;
	matrix<float, 4, 4> lightView;
	matrix<float, 4, 4> lightProj;
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

float4 main(VS_OUT input) : SV_Target
{
	float4 worldPos, normal, color, lightScreenPos;
	float3 lightPos, toLightDirection;
	float2 lightScreenUV;
	float depthToNearestObject, depthToThisObject, lightIntensity, diffuse = 0.0f;

	// Retrieve deferred values
	worldPos = worldPosTexture.Sample(sampleState, input.uv);
	normal = normalTexture.Sample(sampleState, input.uv);
	color = colorTexture.Sample(sampleState, input.uv);

	// Retrieve light values
	lightPos = lightData.xyz;
	lightIntensity = lightData.w;

	// Project this position to this light
	lightScreenPos = mul(worldPos, mul(lightView, lightProj));
	lightScreenPos /= lightScreenPos.w;

	// Translate fragment positions from [-1, 1] to [0, 1]
	lightScreenUV.x = (lightScreenPos.x + 1) * 0.5f;
	lightScreenUV.y = 1 - (lightScreenPos.y + 1) * 0.5f;
	
	if (lightScreenUV.x >= 0.0f && lightScreenUV.x <= 1.0f && lightScreenUV.y >= 0.0f && lightScreenUV.y <= 1.0f)
	{
		// This fragment is within this light's frustum
		depthToNearestObject = depthTexture.Sample(sampleState, lightScreenUV).x;
		depthToThisObject = lightScreenPos.z - 0.0001f;

		if (depthToThisObject < depthToNearestObject)
		{
			// This fragment was visible from this light and receives lighting
			toLightDirection = normalize(lightPos - worldPos.xyz);
			diffuse += saturate(dot(toLightDirection, normal.xyz)) * lightIntensity;
		}
	}
	else
	{
		// This fragment was outside this light's frustum and receives lighting
		toLightDirection = normalize(lightPos - worldPos.xyz);
		diffuse += saturate(dot(toLightDirection, normal.xyz)) * lightIntensity;
	}

	return float4(color.xyz * saturate(diffuse + 0.05f), 1.0f);
}