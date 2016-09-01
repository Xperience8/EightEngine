cbuffer CameraCB : register(b0)
{
	float4x4 gViewProj;
	float3 gCameraPosition;
};
cbuffer ModelCB : register(b1)
{
	float4x4 gWorld;
	float4x4 gWorldInverseTranspose;
};
struct VertexIn
{
	float3 PositionL : POSITION;
	float3 TangentL : TANGENT;
	float3 NormalL : NORMAL;
	float2 TexCoord : TEXCOORD;
};
struct VertexOut
{
	float4 PositionH : SV_POSITION;
	float3 PositionW : POSITION;
	float3 TangentW : TANGENT;
	float3 NormalW : NORMAL;
	float2 TexCoord : TEXCOORD;
};

VertexOut main(VertexIn vin)
{
	VertexOut vout;
	vout.PositionW = mul(float4(vin.PositionL, 1.f), gWorld).xyz;
	vout.PositionH = mul(float4(vout.PositionW, 1.f), gViewProj);

	vout.TangentW = mul(vin.TangentL, (float3x3)gWorld);
	vout.NormalW = mul(vin.NormalL, (float3x3) gWorldInverseTranspose);
	vout.TexCoord = vin.TexCoord;

	return vout;
}