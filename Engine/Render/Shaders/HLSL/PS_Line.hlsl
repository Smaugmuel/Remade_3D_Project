struct PS_IN
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

float4 main(PS_IN input) : SV_Target
{
	return input.color;
}