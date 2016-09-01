/*
struct Vertex
{
	float3 PositionL : POSITION;
	// x -> left offset, y -> top offset, z -> right offset, w -> bottom offset
	float4 Rectangle : RECTANGLE;
	float4 Color : COLOR;
};

Vertex main(Vertex vin)
{
	return vin;
}
*/

struct VertexIn
{
	float3 PositionL : POSITION;
	float4 Color : COLOR;
	float2 TexCoord : TEX_COORD;
};
struct VertexOut
{
	float4 PositionH : SV_POSITION;
	float4 Color : COLOR;
	float2 TexCoord : TEX_COORD;
};

VertexOut main(VertexIn vin)
{
	VertexOut vout;
	vout.PositionH = float4(vin.PositionL, 1.f);
	vout.Color = vin.Color;
	vout.TexCoord = vin.TexCoord;
	return vout;
}