SamplerState gWrapSS : register(s0);
Texture2D gDiffuseTexture : register(t0);

struct PixelIn
{
	float4 PositionH : SV_POSITION;
	float2 TexCoord : TEX_COORD;
};

float4 main(PixelIn pin) : SV_TARGET
{
	return gDiffuseTexture.Sample(gWrapSS, pin.TexCoord);
}