cbuffer CameraCB : register(b0)
{
	float4x4 gViewProj;
	float3 gCameraPosition;
};

struct GeometryIn
{
	float3 PositionW : POSITION;
	float3 NormalW : NORMAL;
};
struct GeometryOut
{
	float4 PositionH : SV_POSITION;
};
[maxvertexcount(6)]
void main(triangle GeometryIn gin[3], inout LineStream<GeometryOut> output)
{	
	[unroll]
	for (int i = 0; i < 3; i++)
	{
		GeometryOut start;
		GeometryOut end;
		start.PositionH = mul(float4(gin[i].PositionW, 1.f), gViewProj);
		end.PositionH = mul(float4(gin[i].PositionW + gin[i].NormalW, 1.f), gViewProj);

		output.Append(start);
		output.Append(end);
		output.RestartStrip();
	}
}
