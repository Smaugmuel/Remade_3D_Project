Texture2D worldPosTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D colorTexture : register(t2);
//Texture2D depthTexture : register(t3);
Texture2D torgueTexture : register(t3);

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
	float4 worldPos;
	float4 normal;
	float4 color;
	float3 lightPos;
	float lightIntensity;
	float3 toLight;
	float diffuse = 0.0f;
	float4 projectedPos;
	float2 projectedUV;
	float depthValue;
	float depthValueLight;
	
	
	float2 uv_splitScreen;

	if (input.uv.x <= 0.5f)
	{
		if (input.uv.y <= 0.5f)
		{
			uv_splitScreen = float2(input.uv.x * 2, input.uv.y * 2);
			return worldPosTexture.Sample(sampleState, uv_splitScreen);
		}
		else
		{
			uv_splitScreen = float2(input.uv.x * 2, input.uv.y * 2 - 1);

			worldPos = worldPosTexture.Sample(sampleState, uv_splitScreen);
			normal = normalTexture.Sample(sampleState, uv_splitScreen);
			color = colorTexture.Sample(sampleState, uv_splitScreen);

			lightPos = lightData.xyz;
			lightIntensity = lightData.w;

			toLight = normalize(lightPos - worldPos.xyz);
			diffuse = saturate(dot(toLight, normal.xyz));
			return float4(color.xyz * saturate(diffuse + 0.1f) * lightIntensity, 1.0f);
		}
	}
	else
	{
		if (input.uv.y <= 0.5f)
		{
			uv_splitScreen = float2(input.uv.x * 2 - 1, input.uv.y * 2);
			return normalTexture.Sample(sampleState, uv_splitScreen);
		}
		else
		{
			uv_splitScreen = float2(input.uv.x * 2 - 1, input.uv.y * 2 - 1);
			return torgueTexture.Sample(sampleState, uv_splitScreen);
			
			//depthValue = pow(depthTexture.Sample(sampleState, uv_splitScreen).x, 1);
			//return float4(depthValue, depthValue, depthValue, 1);
		}
	}
	//*/


	worldPos = worldPosTexture.Sample(sampleState, input.uv);
	normal = normalTexture.Sample(sampleState, input.uv);
	color = colorTexture.Sample(sampleState, input.uv);

	lightPos = lightData.xyz;
	lightIntensity = lightData.w;

	toLight = normalize(lightPos - worldPos.xyz);
	diffuse = saturate(dot(toLight, normal.xyz));



	projectedPos = mul(worldPos, mul(lightView, lightProj));
	
	projectedPos /= projectedPos.w;
	projectedUV.x = (projectedPos.x + 1) * 0.5f;
	projectedUV.y = (-projectedPos.y + 1) * 0.5f;

	if (saturate(projectedUV.x) == projectedUV.x && saturate(projectedUV.y) == projectedUV.y)
	{
		//depthValue = depthTexture.Sample(sampleState, projectedUV).x;
		depthValueLight = projectedPos.z - 0.01f;

		//if (depthValue < 0.1)
		//{
		//	return float4(0, 1, 0, 1);
		//}
		//else
		//{
		//	return float4(1, 0, 0, 1);
		//}

		/*if (depthValueLight < depthValue)
		{
			diffuse = saturate(dot(toLight, normal.xyz));
		}*/
	}

	return float4(color.xyz * saturate(diffuse + 0.1f), 1.0f);
}