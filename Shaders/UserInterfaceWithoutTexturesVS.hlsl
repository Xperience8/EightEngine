struct VertexIn
{
	float3 PositionW : POSITION;
	float4 Color : COLOR;
};
struct VertexOut
{
	float4 PositionH : SV_POSITION;
	float4 Color : COLOR;
};

VertexOut main(VertexIn vin)
{
	VertexOut vout;
	vout.PositionH = float4(vin.PositionW, 1.f);
	vout.Color = vin.Color;
	return vout;
}