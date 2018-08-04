struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 depthPosition : TEXTURE0;
};

float4 main(VS_OUT input) : SV_TARGET
{
	float depthValue = input.depthPosition.z / input.depthPosition.w;
	float4 color;
	

	//depthValue = pow(input.position.z, 100);
	//return float4(depthValue, depthValue, depthValue, 1.0f);

	if (depthValue > 0.999f)
	{
		color = float4(0.0f, 0.0f, 1.0f, 1.0f);
	}
	else if (depthValue > 0.995f)
	{
		color = float4(0.0f, 1.0f, 0.0f, 1.0f);
	}
	else
	{
		color = float4(1.0f, 0.0f, 0.0f, 1.0f);
	}

	return color;
}