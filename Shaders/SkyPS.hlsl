SamplerState gWrapSS : register(s0);
TextureCube gEnvironmentMap : register(t0);

struct PixelIn
{
	float4 PositionH : SV_POSITION;
	float3 TexCoord : TEXCOORD;
};

float4 main(PixelIn pin) : SV_Target
{
	return gEnvironmentMap.Sample(gWrapSS, pin.TexCoord);
}