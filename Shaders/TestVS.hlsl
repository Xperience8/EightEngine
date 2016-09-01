cbuffer CameraCB : register(c0)
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
	//float2 TexCoord : TEX_COORD;
};
struct VertexOut
{
	float4 PositionH : SV_Position;
	//float2 TexCoord : TEX_COORD;
};

VertexOut main(VertexIn vin)
{
	VertexOut vout;
	vout.PositionH = mul(mul(float4(vin.PositionL, 1.f), gWorld), gViewProj);
	//vout.TexCoord = vin.TexCoord;
	return vout;
}