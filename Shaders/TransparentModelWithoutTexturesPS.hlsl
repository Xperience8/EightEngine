#include "LightHelper.hlsli"

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
	float3 NormalW : NORMAL;
};

float4 main(PixelIn pin) : SV_TARGET
{
	float3 toCameraW = normalize(gCameraPosition - pin.PositionW);

	float4 ambient;
	float4 diffuse;
	float4 specular;
	ComputeDirectionalLight(gDirectionalLightData, gMaterialData, normalize(pin.NormalW),
		toCameraW, ambient, diffuse, specular);

	float4 litColor = ambient + diffuse + specular;
	litColor.a = diffuse.a;

	return litColor;
}