//SamplerState gWrapSS : register(s0);
//Texture2D gDiffuseMap : register(t0);

struct VertexOut
{
	float4 PositionH : SV_Position;
	//float2 TexCoord : TEX_COORD;
};

float4 main(VertexOut pin) : SV_TARGET
{
	return float4(1.f, 0.f, 0.f, 0.5f);
	//return gDiffuseMap.Sample(gWrapSS, pin.TexCoord);
}