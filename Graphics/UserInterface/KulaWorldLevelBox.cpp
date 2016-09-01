#include "stdafx.h"
#include "KulaWorldLevelBox.h"
#include "EightEngine\Xml\XmlNode.h"

#include "EightEngine\ResourceCache\ResourceCache.h"
#include "EightEngine\EventManager\EventManager.h"
#include "EightEngine\EventManager\Events\StartLevelEventData.h"

#include "EightEngine\GameLogic\LevelDatabase.h"

void KulaWorldLevelBox::VInit(const XmlNode &configNode, const std::shared_ptr<Screen> &pScreen)
{
	XmlNode guiNode = configNode.GetChildrenNode("Guis");

	m_Images.resize(std::stoi(guiNode.GetAttribute("count").GetValue()) - 1);

	guiNode.GoToChildrenNode();

	while (guiNode.Exist())
	{
		EIGHT_ASSERT(guiNode.GetName() == "MessageBox" || guiNode.GetName() == "BackgroundImage",
			"Unsupported gui type in KulaWorldLevelBox");

		if (guiNode.GetName() == "MessageBox")
		{
			m_LevelLabel.VInit(guiNode, pScreen);
		}
		else
		{
			UINT imageId = std::stoi(guiNode.GetAttribute("id").GetValue()) - 1;
			EIGHT_ASSERT(imageId <= m_Images.size(), "There is invalid background image id");

			m_Images[imageId].VInit(guiNode, pScreen);
		}

		guiNode.GoToNextNode();
	}


	// Load position in NDC space
	auto positionNode = configNode.GetChildrenNode("Position");
	m_TopLeftCorner.x = std::stof(positionNode.GetAttribute("x").GetValue());
	m_TopLeftCorner.y = std::stof(positionNode.GetAttribute("y").GetValue());
	m_TopLeftCorner.z = std::stof(positionNode.GetAttribute("z").GetValue());

	m_BottomRightCorner = m_TopLeftCorner;
	positionNode = configNode.GetChildrenNode("Size");
	m_BottomRightCorner.x += std::stof(positionNode.GetAttribute("width").GetValue());
	m_BottomRightCorner.y += std::stof(positionNode.GetAttribute("height").GetValue());
	
	m_TopLeftCorner = pScreen->PointFromNDCToPx(m_TopLeftCorner);
	m_BottomRightCorner = pScreen->PointFromNDCToPx(m_BottomRightCorner);

	// Load highlighted color
	auto colorNode = configNode.GetChildrenNode("HighlightedColor");
	m_HighLightedColor.x = std::stof(colorNode.GetAttribute("r").GetValue());
	m_HighLightedColor.y = std::stof(colorNode.GetAttribute("g").GetValue());
	m_HighLightedColor.z = std::stof(colorNode.GetAttribute("b").GetValue());
	m_HighLightedColor.w = std::stof(colorNode.GetAttribute("a").GetValue());

	m_LevelName = configNode.GetChildrenNode("LevelName").GetValue();

	m_NormalColor = m_LevelLabel.GetTextSprite()->GetColor();

	// Save action -> what should we do if button was pressed
	auto actionNode = configNode.GetChildrenNode("Action").GetChildrenNode();
	m_ActionType = actionNode.GetName();
	m_Action = actionNode.GetValue();

	auto pLevelConfig = g_LevelDatabase.GetLevelInfo(m_LevelName);

	// Remove lock image
	if (pLevelConfig.GetChildrenNode("Unlocked").GetValue() == "1")
	{
		m_Images.pop_back();
		m_Locked = false;
	}

	auto pDoneTexture = g_ResourceCache.RequestResource<Texture2D>(
		configNode.GetChildrenNode("StarDone").GetValue());
	UINT numStars = std::stoi(pLevelConfig.GetChildrenNode("Stars").GetValue());
	for (UINT i = 0; i < numStars; ++i)
	{
		m_Images[GuiPart::Star1 + i].GetSprite()->ChangeTexture(pDoneTexture);
	}
}

void KulaWorldLevelBox::VRun()
{
	if (!m_ActionType.empty() && !m_Locked)
	{
		if (m_ActionType == "SendEvent")
		{
			if (m_Action == "StartLevelEventData")
			{
				g_EventManager.PushEvent(EventPtr<StartLevelEventData>(m_LevelName));
			}
		}
	}
}

void KulaWorldLevelBox::VRender(UIRenderingPass &renderingPass)
{
	m_LevelLabel.VRender(renderingPass);

	for (auto &image : m_Images)
	{
		image.VRender(renderingPass);
	}
}