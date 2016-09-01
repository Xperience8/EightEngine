#include "LightHelper.hlsli"

SamplerState gWrapSS : register(s0);
Texture2D gDiffuseMap : register(t0);
Texture2D gNormalMap : register(t1);

cbuffer LightCB : register(b0)
{
	DirectionalLight gDirectionalLightData;
};
cbuffer CameraCB : register(b1)
{
	float4x4 gViewProj;
	float3 gCameraPosition;
};
cbuffer MaterialCB : register(b2)
{
	Material gMaterialData;
};
struct PixelIn
{
	float4 PositionH : SV_POSITION;
	float3 PositionW : POSITION;
	float4 TangentW : TANGENT;
	float3 NormalW : NORMAL;
	float2 TexCoord : TEXCOORD;
};

float4 main(PixelIn pin) : SV_TARGET
{
	float4 textureColor = gDiffuseMap.Sample(gWrapSS, pin.TexCoord);
	float3 bumpedNormalW = DecompressNormalVector(gNormalMap.Sample(gWrapSS, pin.TexCoord).xyz, normalize(pin.NormalW), pin.TangentW);

	float3 toCameraW = normalize(gCameraPosition - pin.PositionW);

	float4 ambient;
	float4 diffuse;
	float4 specular;
	ComputeDirectionalLight(gDirectionalLightData, gMaterialData, bumpedNormalW, toCameraW, ambient, diffuse, specular);

	float4 litColor = textureColor * (ambient + diffuse) + specular;

	return litColor;
}