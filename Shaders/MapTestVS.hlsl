struct VertexIn
{
	float3 PositionL : POSITION;
	float2 TexCoord : TEX_COORD;
};
struct VertexOut
{
	float4 PositionH : SV_POSITION;
	float2 TexCoord : TEX_COORD;
};
VertexOut main(VertexIn vin)
{
	VertexOut vout;
	vout.PositionH = float4(vin.PositionL, 1.f);
	vout.TexCoord = vin.TexCoord;
	return vout;
}