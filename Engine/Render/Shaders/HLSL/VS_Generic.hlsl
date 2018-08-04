struct IN
{
#ifdef IN_POS
	float3 position : POSITION;
#endif
#ifdef IN_NORMAL
	float3 normal : NORMAL;
#endif
#ifdef IN_COLOR
	float3 color : COLOR;
#endif
#ifdef IN_UV
	float2 uv : TEXCOORD0;
#endif
};

struct OUT
{
#ifdef OUT_POS
	float3 position : POSITION;
#endif
#ifdef OUT_NORMAL
	float3 normal : NORMAL;
#endif
#ifdef OUT_COLOR
	float3 color : COLOR;
#endif
#ifdef OUT_UV
	float2 uv : TEXCOORD0;
#endif
};

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}