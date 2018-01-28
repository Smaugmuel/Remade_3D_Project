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

	// UV coords at position above
	float2 lightScreenUV;

	// Distance to light
	float depthToNearestObject;
	float depthToThisObject;	

	float diffuse = 0.0f;


	//============= Split Screen =========================
	float2 uv_coords;
	if (input.uv.x <= 0.5f && input.uv.y <= 0.5f)
	{
		// Top left
		uv_coords = float2(input.uv.x * 2, input.uv.y * 2);
		return worldPosTexture.Sample(sampleState, uv_coords);
	}
	else if (input.uv.x >= 0.5f && input.uv.y <= 0.5f)
	{
		// Top right
		uv_coords = float2(input.uv.x * 2 - 1, input.uv.y * 2);
		return normalTexture.Sample(sampleState, uv_coords);
	}
	else if (input.uv.x <= 0.5f && input.uv.y >= 0.5f)
	{
		// Bottom left
		uv_coords = float2(input.uv.x * 2, input.uv.y * 2 - 1);
		float depthValue = pow(depthTexture.Sample(sampleState, uv_coords).x, 100);
		return float4(depthValue, depthValue, depthValue, 1);
	}
	else
	{
		// Bottom right
		uv_coords = float2(input.uv.x * 2 - 1, input.uv.y * 2 - 1);

		worldPos = worldPosTexture.Sample(sampleState, uv_coords);
		normal = normalTexture.Sample(sampleState, uv_coords);
		color = colorTexture.Sample(sampleState, uv_coords);

		lightPos = lightData.xyz;
		lightIntensity = lightData.w;

		toLight = normalize(lightPos - worldPos.xyz);

		lightScreenPos = mul(worldPos, mul(lightView, lightProj));
		lightScreenPos /= lightScreenPos.w;

		// Translate from [-1, 1] to [0, 1]
		lightScreenUV.x = (lightScreenPos.x + 1) * 0.5f;
		lightScreenUV.y = 1 - (lightScreenPos.y + 1) * 0.5f;

		if (saturate(lightScreenUV.x) == lightScreenUV.x && saturate(lightScreenUV.y) == lightScreenUV.y)
		{
			depthToNearestObject = depthTexture.Sample(sampleState, lightScreenUV).x;
			depthToThisObject = lightScreenPos.z - 0.0001f;

			if (depthToThisObject < depthToNearestObject)
			{
				diffuse += saturate(dot(toLight, normal.xyz));
			}
		}

		return float4(color.xyz * saturate(diffuse + 0.1f), 1.0f);
	}
	//*/



	// Retrieve deferred values
	worldPos = worldPosTexture.Sample(sampleState, input.uv);
	normal = normalTexture.Sample(sampleState, input.uv);
	color = colorTexture.Sample(sampleState, input.uv);

	// Retrieve light values
	lightPos = lightData.xyz;
	lightIntensity = lightData.w;

	// Vector from object to light
	toLight = normalize(lightPos - worldPos.xyz);


	//diffuse = saturate(dot(toLight, normal.xyz));

	// Object position seen from light
	lightScreenPos = mul(worldPos, mul(lightView, lightProj));
	lightScreenPos /= lightScreenPos.w;

	// Translate from [-1, 1] to [0, 1]
	lightScreenUV.x = (lightScreenPos.x + 1) * 0.5f;
	lightScreenUV.y = 1 - (lightScreenPos.y + 1) * 0.5f;

	//if ((projectedUV.x >= 0.0f && projectedUV.x <= 1.0f) && (projectedUV.y >= 0.0f && projectedUV.y <= 1.0f))
	//{
	//
	//}
	
	if (saturate(lightScreenUV.x) == lightScreenUV.x && saturate(lightScreenUV.y) == lightScreenUV.y)
	{
		// Object position fit into light frustum (was visible from light

		depthToNearestObject = depthTexture.Sample(sampleState, lightScreenUV).x;
		depthToThisObject = lightScreenPos.z - 0.0001f;

		if (depthToThisObject <= depthToNearestObject)
		{
			// This was the closest object

			// Add diffuse lighting
			diffuse += saturate(dot(toLight, normal.xyz));
		}
	}

	return float4(color.xyz * saturate(diffuse + 0.1f), 1.0f);
}