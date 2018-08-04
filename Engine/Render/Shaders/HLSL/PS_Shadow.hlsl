Texture2D depthTexture : register(t0);

SamplerState sampleStateClamp : register(s0);

cbuffer PerFrame : register(b0)
{
	float4 ambientColor;
	float4 diffuseColor;
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float3 color : COLOR;
	float4 lightViewPosition : TEXCOORD0;
	float3 toLight : TEXCOORD1;
};

float4 main(VS_OUT input) : SV_TARGET
{
	float bias;
	float4 color;
	float2 projectedUV;
	float depthValue;
	float lightDepthValue;
	float lightIntensity;
	float4 objectColor;

	bias = 0.001f;

	color = ambientColor;

	projectedUV.x = (input.lightViewPosition.x / input.lightViewPosition.w + 1) * 0.5f;
	projectedUV.y = (-input.lightViewPosition.y / input.lightViewPosition.w + 1) * 0.5f;

	if ((saturate(projectedUV.x) == projectedUV.x) && (saturate(projectedUV.y) == projectedUV.y))
	{
		depthValue = depthTexture.Sample(sampleStateClamp, projectedUV).r;
		lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;
		lightDepthValue = lightDepthValue - bias;

		if (lightDepthValue < depthValue)
		{
			lightIntensity = saturate(dot(input.normal, input.toLight));

			if (lightIntensity > 0.0f)
			{
				color += diffuseColor * lightIntensity;
				color = saturate(color);
			}
		}
	}

	objectColor = float4(input.color, 1.0f);

	color = color * objectColor;

	return color;
}