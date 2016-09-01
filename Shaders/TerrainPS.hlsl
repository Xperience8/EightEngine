#include "LightHelper.hlsli"

SamplerState gWrapSS : register(s0);
Texture2D gDiffuseMap : register(t0);
Texture2D gNormalMap : register(t1);

cbuffer LightCB : register(c0)
{
	DirectionalLight gDirectionalLightData;
};
cbuffer CameraCB : register(c1)
{
	float4x4 gViewProj;
	float3 gCameraPosition;
};
cbuffer MaterialCB : register(c2)
{
	Material gMaterialData;
};
struct PixelIn
{
	float4 PositionH : SV_POSITION;
	float3 PositionW : POSITION;
	float2 TexCoord : TEX_COORD;
	float2 LayeredTexCoord : TEX_COORD1;
};

float4 main(PixelIn pin) : SV_TARGET
{
	float4 textureColor = gDiffuseMap.Sample(gWrapSS, pin.LayeredTexCoord);
	float3 bumpedNormalW = DecompressNormalVector(gNormalMap.Sample(gWrapSS, pin.LayeredTexCoord).xyz, float3(0.f, 1.f, 0.f), float4(1.f, 0.f, 0.f, 1.f));

	float3 toCameraW = normalize(gCameraPosition - pin.PositionW);

	float4 ambient;
	float4 diffuse;
	float4 specular;
	ComputeDirectionalLight(gDirectionalLightData, gMaterialData, bumpedNormalW, toCameraW, ambient, diffuse, specular);

	float4 litColor = textureColor * (ambient + diffuse) + specular;

		return litColor;
}