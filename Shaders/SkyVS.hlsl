cbuffer ModelCB : register(c0)
{
	float4x4 gWorld;
}

struct VertexOut
{
	float4 PositionH : SV_POSITION;
	float3 TexCoord : TEXCOORD;
};

VertexOut main(float3 PositionL : POSITION)
{
	VertexOut vout;
	vout.TexCoord = PositionL;
	vout.PositionH = mul(float4(PositionL, 1.f), gWorld).xyww;
	return vout;
}