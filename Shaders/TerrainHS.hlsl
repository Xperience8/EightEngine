struct HullVertex
{
	float3 PositionW : POSITION;
	float2 TexCoord : TEX_COORD;
};
struct PatchTess
{
	float EdgeTessFactor[4] : SV_TessFactor;
	float InsideTessFactor[2] : SV_InsideTessFactor;
};

PatchTess ConstantHS(InputPatch<HullVertex, 4> vertices)
{
	PatchTess pt;
	pt.EdgeTessFactor[0] = 64.f;
	pt.EdgeTessFactor[1] = 64.f;
	pt.EdgeTessFactor[2] = 64.f;
	pt.EdgeTessFactor[3] = 64.f;
	pt.InsideTessFactor[0] = 64.f;
	pt.InsideTessFactor[1] = 64.f;
	return pt;
}

[domain("quad")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.f)]
HullVertex main(InputPatch<HullVertex, 4> vertices, int i : SV_OutputControlPointID)
{
	return vertices[i];
}