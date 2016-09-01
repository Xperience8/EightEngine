#include "stdafx.h"
#include "TextMessageBox.h"
#include "EightEngine\Graphics\Rendering\UIRenderingPass.h"
#include "EightEngine\Xml\XmlNode.h"
#include "EightEngine\ResourceCache\ResourceCache.h"
#include "EightEngine\GameAppLayer\Screen.h"

void TextMessageBox::VInit(const XmlNode &configNode,
	const std::shared_ptr<Screen> &pScreen)
{
	// Getting position
	auto positionNode = configNode.GetChildrenNode("Position");
	DirectX::XMFLOAT3 position;
	position.x = std::stof(positionNode.GetAttribute("x").GetValue());
	position.y = std::stof(positionNode.GetAttribute("y").GetValue());
	position.z = std::stof(positionNode.GetAttribute("z").GetValue());

	// If this message box has text
	auto textSpriteNode = configNode.GetChildrenNode("TextSprite");
	if (textSpriteNode.Exist())
	{
		m_pTextSprite = std::make_shared<TextSprite>();
		m_pTextSprite->Init(position, textSpriteNode, pScreen);
	}
	else {
		EIGHT_WARNING("You are creating text message box without any text! This is quite unusual!");
	}
}
void TextMessageBox::VRender(UIRenderingPass &renderingPass)
{
	auto &textBatch = renderingPass.GetTextBatch(m_pTextSprite->GetFont());
	textBatch.Batch(m_pTextSprite->GetVertices());
}