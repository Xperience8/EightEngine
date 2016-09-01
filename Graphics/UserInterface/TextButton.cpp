#include "stdafx.h"
#include "TextButton.h"
#include "EightEngine\Xml\XmlNode.h"
#include "EightEngine\GameAppLayer\Screen.h"
#include "EightEngine\ResourceCache\ResourceCache.h"
#include "EightEngine\Graphics\Rendering\UIRenderingPass.h"
#include "EightEngine\EventManager\EventManager.h"
#include "EightEngine\EventManager\Events\PauseGameEventData.h"
#include "EightEngine\EventManager\Events\ExitGameEventData.h"
#include "EightEngine\EventManager\Events\StartGameEventData.h"
#include "EightEngine\EventManager\Events\ChangeMenuScreenEventData.h"
#include "EightEngine\EventManager\Events\ContinueGameEventData.h"
#include "EightEngine\EventManager\Events\GoToMainMenuEventData.h"

void TextButton::VInit(const XmlNode &configNode,
	const std::shared_ptr<Screen> &pScreen)
{
	// Load position in NDC space
	auto positionNode = configNode.GetChildrenNode("Position");
	DirectX::XMFLOAT3 position;
	position.x = std::stof(positionNode.GetAttribute("x").GetValue());
	position.y = std::stof(positionNode.GetAttribute("y").GetValue());
	position.z = std::stof(positionNode.GetAttribute("z").GetValue());

	// Load highlighted color
	auto colorNode = configNode.GetChildrenNode("HighlightedColor");
	m_HighLightedColor.x = std::stof(colorNode.GetAttribute("r").GetValue());
	m_HighLightedColor.y = std::stof(colorNode.GetAttribute("g").GetValue());
	m_HighLightedColor.z = std::stof(colorNode.GetAttribute("b").GetValue());
	m_HighLightedColor.w = std::stof(colorNode.GetAttribute("a").GetValue());

	// If there is text(in button), create it, find its size and save its color
	auto textSpriteNode = configNode.GetChildrenNode("TextSprite");
	if (textSpriteNode.Exist())
	{
		m_pTextSprite = std::make_shared<TextSprite>();
		m_pTextSprite->Init(position, textSpriteNode, pScreen);
		m_pTextSprite->FindTextDimension(m_TopLeftCorner, m_BottomRightCorner);

		m_NormalColor = m_pTextSprite->GetColor();
	}
	else {
		EIGHT_WARNING("You are creating text button without any text! This is quite unusual!");
	}

	// Save action -> what should we do if button was pressed
	auto actionNode = configNode.GetChildrenNode("Action").GetChildrenNode();
	while (actionNode.Exist())
	{
		Action action;
		action.ActionParameters = actionNode.GetValue();
		action.ActionType = actionNode.GetName();

		m_Actions.push_back(action);
		actionNode.GoToNextNode();
	}
}
void TextButton::VRun()
{
	if (!m_Actions.empty())
	{
		for (auto &action : m_Actions)
		{
			if (action.ActionType == "SendEvent")
			{
				if (action.ActionParameters == "PauseGameEventData")
				{
					g_EventManager.PushEvent(EventPtr<PauseGameEventData>());
				}
				else if (action.ActionParameters == "ExitGameEventData")
				{
					g_EventManager.PushEvent(EventPtr<ExitGameEventData>());
				}
				else if (action.ActionParameters == "StartGameEventData")
				{
					g_EventManager.PushEvent(EventPtr<StartGameEventData>());
				}
				else if (action.ActionParameters == "GoToMainMenuEventData")
				{
					g_EventManager.PushEvent(EventPtr<GoToMainMenuEventData>());
				}
				else if (action.ActionParameters == "ContinueGameEventData")
				{
					g_EventManager.PushEvent(EventPtr<ContinueGameEventData>());
				}
				else if (action.ActionParameters == "StartNewGameEventData")
				{
					g_EventManager.PushEvent(EventPtr<StartNewGameEventData>());
				}
			}
			else if (action.ActionType == "GoToLayout")
			{
				g_EventManager.PushEvent(EventPtr<ChangeMenuScreenEventData>(
					std::stoi(action.ActionParameters)));
			}
		}
	}
}
void TextButton::VRender(UIRenderingPass &renderingPass)
{
	// Sending text vertices into batch
	auto &textBatch = renderingPass.GetTextBatch(m_pTextSprite->GetFont());
	textBatch.Batch(m_pTextSprite->GetVertices());
}
