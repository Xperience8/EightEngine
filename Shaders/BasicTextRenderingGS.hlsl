cbuffer cbTextBuffer : register(b0)
{
	float gFontTextureWidth;
	float gFontTextureHeight;
	float gScreenWidth;
	float gScreenHeight;
};

struct GeometryIn
{
	float3 PositionL : POSITION;
	// x -> left offset, y -> top offset, z -> right offset, w -> bottom offset
	float4 Rectangle : RECTANGLE;
	float4 Color : COLOR;
};
struct GeometryOut
{
	float4 PositionH : SV_POSITION;
	float4 Color : COLOR;
	float2 TexCoord : TEXCOORD;
};

[maxvertexcount(4)]
void main(point GeometryIn character[1], inout TriangleStream<GeometryOut> stream)
{
	float width = character[0].Rectangle.z - character[0].Rectangle.x;
	float height = character[0].Rectangle.w - character[0].Rectangle.y;

	float leftPosition = 2.f * (character[0].PositionL.x / gScreenWidth) - 1.f;
	float rightPosition = 2.f * ((character[0].PositionL.x + width) / gScreenWidth) - 1.f;
	float topPosition = -2.f * (character[0].PositionL.y / gScreenHeight) + 1.f;
	float bottomPosition = -2.f * ((character[0].PositionL.y + height) / gScreenHeight) + 1.f;

	float leftTexCoord = character[0].Rectangle.x / gFontTextureWidth;
	float topTexCoord = character[0].Rectangle.y / gFontTextureHeight;
	float rightTexCoord = character[0].Rectangle.z / gFontTextureWidth;
	float bottomTexCoord = character[0].Rectangle.w / gFontTextureHeight;

	GeometryOut gout;
	gout.Color = character[0].Color;

	// Left Down
	gout.PositionH = float4(leftPosition, bottomPosition, character[0].PositionL.z, 1.f);
	gout.TexCoord = float2(leftTexCoord, bottomTexCoord);
	stream.Append(gout);

	// Left Up
	gout.PositionH = float4(leftPosition, topPosition, character[0].PositionL.z, 1.f);
	gout.TexCoord = float2(leftTexCoord, topTexCoord);
	stream.Append(gout);

	// Right Down
	gout.PositionH = float4(rightPosition, bottomPosition, character[0].PositionL.z, 1.f);
	gout.TexCoord = float2(rightTexCoord, bottomTexCoord);
	stream.Append(gout);

	// Right Up
	gout.PositionH = float4(rightPosition, topPosition, character[0].PositionL.z, 1.f);
	gout.TexCoord = float2(rightTexCoord, topTexCoord);
	stream.Append(gout);

	stream.RestartStrip();
}
