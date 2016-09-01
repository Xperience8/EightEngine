#include "stdafx.h"
#include "Sprite.h"
#include "EightEngine\Graphics\ApiWrapper\Texture2D.h"
#include "EightEngine\GameAppLayer\Screen.h"
#include "EightEngine\Xml\XmlNode.h"
#include "EightEngine\ResourceCache\ResourceCache.h"

void Sprite::Init(const DirectX::XMFLOAT3 &position, const XmlNode &configNode,
	const std::shared_ptr<Screen> &pScreen)
{
	m_pTexture = g_ResourceCache.RequestResource<Texture2D>(
		configNode.GetChildrenNode("Texture").GetValue());

	// Getting size in NDC space
	auto sizeNode = configNode.GetChildrenNode("Size");
	float sizeWidth = std::stof(sizeNode.GetAttribute("width").GetValue());
	float sizeHeight = std::stof(sizeNode.GetAttribute("height").GetValue());

	const DirectX::XMFLOAT3 topLeftCorner = position;
	const DirectX::XMFLOAT3 bottomRightCorner(
		position.x + sizeWidth, position.y + sizeHeight, position.z);

	// Bottom left
	m_Vertices[0].Position = DirectX::XMFLOAT3(topLeftCorner.x, bottomRightCorner.y,
		topLeftCorner.z);
	m_Vertices[0].TexCoord = DirectX::XMFLOAT2(0.f, 1.f);

	// Top left
	m_Vertices[1].Position = DirectX::XMFLOAT3(topLeftCorner.x, topLeftCorner.y,
		topLeftCorner.z);
	m_Vertices[1].TexCoord = DirectX::XMFLOAT2(0.f, 0.f);

	// Top right
	m_Vertices[2].Position = DirectX::XMFLOAT3(bottomRightCorner.x, topLeftCorner.y,
		topLeftCorner.z);
	m_Vertices[2].TexCoord = DirectX::XMFLOAT2(1.f, 0.f);

	// Bottom right
	m_Vertices[3].Position = DirectX::XMFLOAT3(bottomRightCorner.x, bottomRightCorner.y,
		topLeftCorner.z);
	m_Vertices[3].TexCoord = DirectX::XMFLOAT2(1.f, 1.f);
}