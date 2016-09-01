#pragma once

///////////////////////////////////////////////////////////////////////////////
//				Completely rework, too long, too litle work 				 //
///////////////////////////////////////////////////////////////////////////////

#include "EightEngine\Graphics\ApiWrapper\Buffers\ConstantBuffer.h"

#include "EightEngine\GameView\GameCamera.h"
#include "EightEngine\Actor\Components\DirectionalLightComponent.h"

static DirectX::XMMATRIX InverseTranspose(DirectX::CXMMATRIX M)
{
	DirectX::XMMATRIX A = M;
	A.r[3] = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
	return DirectX::XMMatrixInverse(&DirectX::XMMatrixDeterminant(A), A);
}
struct ModelCBData
{
	static const char *NAME;

	DirectX::XMFLOAT4X4 WorldMatrix;
	DirectX::XMFLOAT4X4 WorldInverseTransposeMatrix;

	ModelCBData() { }
	explicit ModelCBData(DirectX::CXMMATRIX worldMatrix)
	{
		DirectX::XMStoreFloat4x4(&WorldMatrix, DirectX::XMMatrixTranspose(worldMatrix));
		DirectX::XMStoreFloat4x4(&WorldInverseTransposeMatrix, InverseTranspose(worldMatrix));
	}
};
struct CameraCBData
{
	static const char *NAME;

	DirectX::XMFLOAT4X4 ViewProjMatrix;
	DirectX::XMFLOAT3 Position;

	CameraCBData() { }
	CameraCBData(DirectX::CXMMATRIX viewProjMatrix, const DirectX::XMFLOAT3 &position)
	{
		DirectX::XMStoreFloat4x4(&ViewProjMatrix, DirectX::XMMatrixTranspose(viewProjMatrix));
		Position = position;
	}
	CameraCBData(DirectX::CXMMATRIX viewProjMatrix, DirectX::FXMVECTOR position)
	{
		DirectX::XMStoreFloat4x4(&ViewProjMatrix, DirectX::XMMatrixTranspose(viewProjMatrix));
		DirectX::XMStoreFloat3(&Position, position);
	}
};
struct MaterialCBData
{
	static const char *NAME;

	LightMaterial Material;

	MaterialCBData() { }
	explicit MaterialCBData(const LightMaterial &material) : Material(material) { }
};
struct LightCBData
{
	static const char *NAME;

	DirectionalLight DirectionalLightData;

	LightCBData() { }
	explicit LightCBData(const DirectionalLight &dirLight) : DirectionalLightData(dirLight) { }
};
struct TerrainCBData
{
	static const char *NAME;

	DirectX::XMFLOAT2 TextureScaling;

	TerrainCBData() { }
	explicit TerrainCBData(const DirectX::XMFLOAT2 &textureScale) : TextureScaling(textureScale) { }
};
struct TextCBData
{
	static const char *NAME;

	float FontSheetWidth;
	float FontSheetHeight;
	float ScreenWidth;
	float ScreenHeight;

	TextCBData() { }
	TextCBData(float fontSheetWidth, float fontSheetHeight, float screenWidth,
		float screenHeight) : FontSheetWidth(fontSheetWidth), FontSheetHeight(fontSheetHeight),
		ScreenWidth(screenWidth), ScreenHeight(screenHeight) { }
};

class RenderingDevice;
class RenderingDeviceContext;

class ConstantBufferCache
{
private:
	std::unordered_map<std::string, std::shared_ptr<ConstantBuffer> > m_ConstantBuffersMap;
public:
	ConstantBufferCache() { }
	~ConstantBufferCache() { }

	bool OnRestore(const std::shared_ptr<RenderingDevice> &pRenderingDevice);

	std::shared_ptr<ConstantBuffer> &GetBuffer(const std::string &BufferName)
	{
		return m_ConstantBuffersMap[BufferName];
	}
};
///////////////////////////////////////////////////////////////////////////////
//				Completely rework, too long, too litle work 				 //
///////////////////////////////////////////////////////////////////////////////
