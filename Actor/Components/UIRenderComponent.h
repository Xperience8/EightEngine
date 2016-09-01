#pragma once

#include "RenderComponent.h"
#include "EightEngine\Graphics\UserInterface\IGuiElement.h"

#include "EightEngine\Graphics\UserInterface\IUserInterface.h"
#include "EightEngine\Graphics\UserInterface\LayoutUI.h"
#include "EightEngine\Graphics\UserInterface\MenuUI.h"

class UIRenderComponent : public RenderComponent
{
public:
	UIRenderComponent() { }
	~UIRenderComponent() 
	{ 
		if (m_UserInterface)
		{
			m_UserInterface->VUnregisterEvents();
		}
	}

	// Loading user interface from  xml config file
	bool VInit(const XmlNode &configNode) override;
	
	// UIRenderComponent has no depedencies on other components therefore this call is empty
	bool VPostInit() override { return true; }

	// UIRenderComponent only stores and renders gui elements therefore this call is empty
	void VUpdate(float deltaTime) override { }

	// UIRenderComponent is rendered in batch but not with this call -> this call is empty
	void VRender(Renderer &renderer) override { }

	// UIRenderComponent i rendered in batch but not with this call -> this call is empty
	// Debug rendering is actually not implemented
	void VRenderDebug(Renderer &renderer) override { }

	// Save UIRenderComponent into xml config file(actuall status is saved)
	void VSaveToXML(XmlNode &configNode) override;

	// Return that this is text(actually not true, future work)
	RenderingType VGetRenderingType() const override { return RenderingType::Text; }

	// Return user interface
	// Throught this IUserInterface you can update and render gui elemets
	const std::shared_ptr<IUserInterface> &GetUserInterface() const
	{
		return m_UserInterface;
	}

	// Event when you pressed left mouse button
	void MouseClick(int x, int y)
	{
		m_UserInterface->VMouseClick(x, y);
	}

	// Event when you move with mouse
	void MouseMove(int x, int y)
	{
		m_UserInterface->VMouseMove(x, y);
	}

	// Event when you want to go to other element(such as you press Up key)
	void MoveEvent(const DirectX::XMFLOAT3 &movingDirection)
	{
		m_UserInterface->VMoveEvent(movingDirection);
	}
	void ActivateSelectedGuiElement()
	{
		m_UserInterface->ActivateSelectedGuiElement();
	}
	// Event when you press key
	void KeyDown(UINT keyCode)
	{
		m_UserInterface->VKeyDown(keyCode);
	}
private:
	std::shared_ptr<IUserInterface> m_UserInterface;

	// Load whole layout from xml config file
	void LoadLayout(const XmlNode &layoutConfigNode);
	
	// Load, construct and initialize exactly one gui element
	std::shared_ptr<IGuiElement> LoadGuiElement(const XmlNode &elementConfigNode);

	// Consturct gui element of selected type
	std::shared_ptr<IGuiElement> ConstructGuiElement(const std::string &guiElementType);
};