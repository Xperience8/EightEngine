cbuffer ModelCB : register(b0)
{
	float4x4 gWorld;
	float4x4 gWorldInverseTranspose;
};

struct VertexIn
{
	float3 PositionL : POSITION;
	float4 TangentL : TANGENT;
	float3 NormalL : NORMAL;
	float2 TexCoord : TEXCOORD;
};
struct VertexOut
{
	float3 PositionW : POSITION;
	float3 NormalW : NORMAL;
};
VertexOut main(VertexIn vin)
{
	VertexOut vout;
	vout.PositionW = mul(float4(vin.PositionL, 1.f), gWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInverseTranspose);
	return vout;
}