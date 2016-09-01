#include "stdafx.h"
#include "UIRenderComponent.h"
#include "EightEngine\ResourceCache\ResourceCache.h"
#include "EightEngine\Graphics\ApiWrapper\Renderer.h"
#include "EightEngine\GameAppLayer\Screen.h"
#include "EightEngine\EventManager\EventManager.h"
#include "EightEngine\Graphics\UserInterface\HUD.h"
#include "EightEngine\Graphics\UserInterface\TextMessageBox.h"
#include "EightEngine\Graphics\UserInterface\TextButton.h"
#include "EightEngine\Graphics\UserInterface\BackgroundImage.h"
#include "EightEngine\Graphics\UserInterface\KulaWorldLevelBox.h"


// Separate Menu from UIRenderComponent or rename

bool UIRenderComponent::VInit(const XmlNode &configNode)
{
	// Try to find user interface type
	auto uiNode = configNode.GetChildrenNode("UserInterface");
	std::string uiType = uiNode.GetAttribute("type").GetValue();

	if (uiType == "Menu")
	{
		m_UserInterface = std::make_shared<MenuUI>();
	}
	else if (uiType == "HUD")
	{
		m_UserInterface = std::make_shared<HUD>();
	}
	else
	{
		EIGHT_ASSERT(false, "Bad User Interface type in UIRenderComponents config");
	}

	// Construct layouts
	// Every layout is composed from gui elements, but only one layout is rendered and updated
	auto layoutsNode = uiNode.GetChildrenNode("Layouts");
	if (layoutsNode.Exist())
	{
		// How many layouts user interface has
		UINT numLayouts = std::stoi(layoutsNode.GetAttribute("count").GetValue());
		m_UserInterface->VInit(numLayouts);

		// Load and initialize every layout
		auto layoutNode = layoutsNode.GetChildrenNode();
		while (layoutNode.Exist())
		{
			LoadLayout(layoutNode);

			layoutNode.GoToNextNode();
		}
	}

	m_UserInterface->VRegisterEvents();
	m_UserInterface->VPostInit();

	auto visibleLayoutNode = uiNode.GetChildrenNode("VisibleLayoutId");
	if (visibleLayoutNode.Exist())
	{
		g_EventManager.TriggerEvent(EventPtr<ChangeMenuScreenEventData>(
			std::stoi(visibleLayoutNode.GetValue())));
	}

	return true;
}
void UIRenderComponent::LoadLayout(const XmlNode &layoutConfigNode)
{
	UINT layoutId = std::stoi(layoutConfigNode.GetAttribute("id").GetValue());
	UINT parentlayoutId = std::stoi(layoutConfigNode.GetAttribute("parentId").GetValue());
	m_UserInterface->VCreateLayout(layoutId, parentlayoutId);

	// Load gui elements
	auto guiElementNode = layoutConfigNode.GetChildrenNode();
	while (guiElementNode.Exist())
	{
		auto guiElement = LoadGuiElement(guiElementNode);

		m_UserInterface->VAddGuiElementToLayout(layoutId, guiElement);

		guiElementNode.GoToNextNode();
	}
}
std::shared_ptr<IGuiElement> UIRenderComponent::LoadGuiElement(
	const XmlNode &elementConfigNode)
{
	// Construct gui element of specific type
	const std::string guiElementType = elementConfigNode.GetName();
	auto guiElement = ConstructGuiElement(guiElementType);

	guiElement->VInit(elementConfigNode, g_Renderer.GetScreen());

	return guiElement;
}
std::shared_ptr<IGuiElement> UIRenderComponent::ConstructGuiElement(
	const std::string &guiElementType)
{
	if (guiElementType == "MessageBox")
	{
		return std::make_shared<TextMessageBox>();
	}
	else if (guiElementType == "TextButton")
	{
		return std::make_shared<TextButton>();
	}
	else if (guiElementType == "BackgroundImage")
	{
		return std::make_shared<BackgroundImage>();
	}
	else if (guiElementType == "BackgroundQuad")
	{
		return std::make_shared<BackgroundQuad>();
	}
	else if (guiElementType == "KulaWorldLevelBox")
	{
		return std::make_shared<KulaWorldLevelBox>();
	}
	else {
		EIGHT_ERROR("You are trying to create gui element whose type is not registered in engine");
		return std::shared_ptr<IGuiElement>();
	}
}
void UIRenderComponent::VSaveToXML(XmlNode &configNode) 
{ 
	// Future Work
	EIGHT_ASSERT(false, "UIRenderComponent::VSaveToXML is not fully created");
}





