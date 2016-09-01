SamplerState gWrapSS : register(s0);
Texture2D gMap : register(t0);

struct VertexOut
{
	float4 PositionH : SV_POSITION;
	float2 TexCoord : TEX_COORD;
};

float4 main(VertexOut pin) : SV_TARGET
{
	float a = gMap.SampleLevel(gWrapSS, pin.TexCoord, 0).a;
	return float4(a, a, a, 1.f);
}