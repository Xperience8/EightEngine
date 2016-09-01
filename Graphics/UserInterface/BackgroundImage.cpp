#include "stdafx.h"
#include "BackgroundImage.h"
#include "EightEngine\Graphics\Rendering\UIRenderingPass.h"
#include "EightEngine\Xml\XmlNode.h"
#include "EightEngine\ResourceCache\ResourceCache.h"
#include "EightEngine\GameAppLayer\Screen.h"

void BackgroundImage::VInit(const XmlNode &configNode,
	const std::shared_ptr<Screen> &pScreen)
{
	// Getting position
	auto positionNode = configNode.GetChildrenNode("Position");
	DirectX::XMFLOAT3 position;
	position.x = std::stof(positionNode.GetAttribute("x").GetValue());
	position.y = std::stof(positionNode.GetAttribute("y").GetValue());
	position.z = std::stof(positionNode.GetAttribute("z").GetValue());

	auto spriteNode = configNode.GetChildrenNode("Sprite");
	if (spriteNode.Exist())
	{
		m_pSprite = std::make_shared<Sprite>();
		m_pSprite->Init(position, spriteNode, pScreen);
	}
	else {
		EIGHT_ERROR("You are creating text message box without specifying size! This mean that handling input will be incorrect!");
	}
}
void BackgroundImage::VRender(UIRenderingPass &renderingPass)
{
	// Sending sprite vertices into batch
	auto &spriteBatch = renderingPass.GetSpriteBatch(m_pSprite->GetTexture());
	spriteBatch.Batch(m_pSprite->GetVertices());
}

//
void BackgroundQuad::VInit(const XmlNode &configNode,
	const std::shared_ptr<Screen> &pScreen)
{
	// Getting position
	auto positionNode = configNode.GetChildrenNode("Position");
	DirectX::XMFLOAT3 position;
	position.x = std::stof(positionNode.GetAttribute("x").GetValue());
	position.y = std::stof(positionNode.GetAttribute("y").GetValue());
	position.z = std::stof(positionNode.GetAttribute("z").GetValue());

	auto sizeNode = configNode.GetChildrenNode("Size");
	DirectX::XMFLOAT2 size;
	size.x = std::stof(sizeNode.GetAttribute("width").GetValue());
	size.y = std::stof(sizeNode.GetAttribute("height").GetValue());

	auto colorNode = configNode.GetChildrenNode("Color");
	DirectX::XMFLOAT4 color;
	color.x = std::stof(colorNode.GetAttribute("r").GetValue());
	color.y = std::stof(colorNode.GetAttribute("g").GetValue());
	color.z = std::stof(colorNode.GetAttribute("b").GetValue());
	color.w = std::stof(colorNode.GetAttribute("a").GetValue());

	float halfSizeX = size.x * 0.5f;
	float halfSizeY = size.y * 0.5f;

	float centerX = position.x + halfSizeX;
	float centerY = position.y + halfSizeY;

	m_Vertices.resize(4);

	m_Vertices[0].Position = DirectX::XMFLOAT3(centerX - halfSizeX, 
		centerY + halfSizeY, position.z);
	m_Vertices[0].Color = color;
	m_Vertices[1].Position = DirectX::XMFLOAT3(centerX - halfSizeX,
		centerY - halfSizeY, position.z);
	m_Vertices[1].Color = color;
	m_Vertices[2].Position = DirectX::XMFLOAT3(centerX + halfSizeX,
		centerY - halfSizeY, position.z);
	m_Vertices[2].Color = color;
	m_Vertices[3].Position = DirectX::XMFLOAT3(centerX + halfSizeX,
		centerY + halfSizeY, position.z);
	m_Vertices[3].Color = color;
}
void BackgroundQuad::VRender(UIRenderingPass &renderingPass)
{
	// Sending quad vertices into batch
	renderingPass.GetQuadBatch().Batch(m_Vertices);
}