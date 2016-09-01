cbuffer ModelCB : register(b0)
{
	float4x4 gWorld;
	float4x4 gWorldInverseTranspose;
};
struct VertexIn
{
	float3 PositionL : POSITION;
	float2 TexCoord : TEX_COORD;
};
struct VertexOut
{
	float3 PositionW : POSITION;
	float2 TexCoord : TEX_COORD;
};
VertexOut main(VertexIn vin)
{
	VertexOut vout;
	vout.PositionW = mul(float4(vin.PositionL, 1.f), gWorld).xyz;
	vout.TexCoord = vin.TexCoord;
	return vout;
}