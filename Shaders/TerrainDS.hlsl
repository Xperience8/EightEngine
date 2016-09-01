cbuffer CameraCB : register(b0)
{
	float4x4 gViewProj;
	float3 gCameraPosition;
};

cbuffer TerrainCB : register(b1)
{
	float2 gTexScale;
};

struct DomainIn
{
	float3 PositionW : POSITION;
	float2 TexCoord : TEX_COORD;
};
struct DomainOut
{
	float4 PositionH : SV_POSITION;
	float3 PositionW : POSITION;
	float2 TexCoord : TEX_COORD;
	float2 LayeredTexCoord : TEX_COORD1;
};
struct PatchTess
{
	float EdgeTessFactor[4] : SV_TessFactor;
	float InsideTessFactor[2] : SV_InsideTessFactor;
};
[domain("quad")]
DomainOut main(PatchTess pt, float2 uv : SV_DomainLocation, OutputPatch<DomainIn, 4> patch)
{
	DomainOut dout;
	dout.PositionW = lerp(lerp(patch[0].PositionW, patch[1].PositionW, uv.x), lerp(patch[2].PositionW, patch[3].PositionW, uv.x), uv.y);
	dout.TexCoord = lerp(lerp(patch[0].TexCoord, patch[1].TexCoord, uv.x), lerp(patch[2].TexCoord, patch[3].TexCoord, uv.x), uv.y);
	dout.LayeredTexCoord = dout.TexCoord * gTexScale;
	dout.PositionH = mul(float4(dout.PositionW, 1.f), gViewProj);
	return dout;
}
