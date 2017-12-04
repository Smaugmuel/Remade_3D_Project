Texture2D depthTexture : register(t0);

SamplerState sampleStateClamp : register(s0);

struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 worldPosition : POSITION;
	float3 normal : NORMAL;
	float4 color : COLOR;
	float4 lightViewPosition : TEXCOORD0;
	float3 toLight : TEXCOORD2;
};

float4 main(VS_OUT input) : SV_TARGET
{
	float bias = 0.001f;
	float2 projectedUV;
	float depthValue;
	float lightDepthValue;
	float ambientIntensity = 0.1f;
	float diffuseIntensity = 0.0f;

	projectedUV.x = (input.lightViewPosition.x / input.lightViewPosition.w + 1) * 0.5f;
	projectedUV.y = (-input.lightViewPosition.y / input.lightViewPosition.w + 1) * 0.5f;

	if ((saturate(projectedUV.x) == projectedUV.x) && (saturate(projectedUV.y) == projectedUV.y))
	{
		depthValue = depthTexture.Sample(sampleStateClamp, projectedUV).r;
		lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w - bias;

		if (lightDepthValue < depthValue)
		{
			diffuseIntensity = saturate(dot(input.normal, input.toLight));
		}
	}

	return input.color * saturate(ambientIntensity + diffuseIntensity);
}