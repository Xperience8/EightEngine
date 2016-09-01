SamplerState gBorderSS : register(s0);
Texture2D gFontTexture : register(t0);
struct PixelIn
{
	float4 PositionH : SV_POSITION;
	float4 Color : COLOR;
	float2 TexCoord : TEX_COORD;
};
float4 main(PixelIn pin) : SV_TARGET
{
	return gFontTexture.SampleLevel(gBorderSS, pin.TexCoord, 0) * pin.Color;
}