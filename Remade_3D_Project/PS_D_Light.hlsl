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
	float4 worldPos;
	float4 normal;
	float4 color;
	float3 lightPos;
	float lightIntensity;
	float3 toLight;
	float diffuse = 0.0f;
	float4 projectedPos;
	float2 projectedUV;
	
	
	worldPos = worldPosTexture.Sample(sampleState, input.uv);
	normal = normalTexture.Sample(sampleState, input.uv);
	color = colorTexture.Sample(sampleState, input.uv);

	lightPos = lightData.xyz;
	lightIntensity = lightData.w;

	toLight = normalize(lightPos - worldPos.xyz);

	projectedPos = mul(worldPos, mul(lightView, lightProj));
	
	projectedPos /= projectedPos.w;
	projectedUV.x = (projectedPos.x + 1) * 0.5f;
	projectedUV.y = (-projectedPos.y + 1) * 0.5f;

	if (saturate(projectedUV.x) == projectedUV.x && saturate(projectedUV.y) == projectedUV.y)
	{
		float depthValue = depthTexture.Sample(sampleState, projectedUV).x;
		float depthValueLight = projectedPos.z - 0.01f;



		/*if (depthValueLight < depthValue)
		{
			diffuse = saturate(dot(toLight, normal.xyz));
		}*/
	}

	diffuse = saturate(dot(toLight, normal.xyz));

	//float depth = pow(depthTexture.Sample(sampleState, input.uv).r, 100);
	//return float4(depth, depth, depth, 1.0f);

	//return normal;
	//return float4(0, 0, 1, 1);
	return float4(color.xyz * saturate(diffuse + 0.1f), 1.0f);
}