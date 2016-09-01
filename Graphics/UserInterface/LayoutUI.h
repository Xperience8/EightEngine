#pragma once

#include "IUserInterface.h"
#include "IGuiElement.h"
#include "IActiveGuiElement.h"

class UIRenderingPass;

class LayoutUI
{
public:
	LayoutUI() : m_pParentLayout(nullptr), m_SelectedGuiElement(0), m_LayoutId(0) { }
	~LayoutUI() { }

	void Init(UINT layoutId, const LayoutUI &parentLayout)
	{
		m_LayoutId = layoutId;
		m_pParentLayout = &parentLayout;
	}
	// Highlight one element(default is 0)
	void PostInit()
	{
		if (!m_ClickableGuiElements.empty())
		{
			m_ClickableGuiElements[m_SelectedGuiElement]->VHighLightOn();
		}
	}

	// Render every element in this layout
	void Render(UIRenderingPass &renderingPass)
	{
		for (auto guiElement : m_GuiElements)
		{
			guiElement->VRender(renderingPass);
		}
	}

	void AddGuiElement(const std::shared_ptr<IGuiElement> &pGuiElement)
	{
		m_GuiElements.push_back(pGuiElement);

		if (pGuiElement->VIsActiveElement())
		{
			m_ClickableGuiElements.push_back(
				std::static_pointer_cast<IActiveGuiElement>(pGuiElement));
		}
	}
	const std::vector<std::shared_ptr<IGuiElement> > &GetGuiElements() const
	{
		return m_GuiElements;
	}

	UINT GetParentLayoutId() const { return m_pParentLayout->m_LayoutId; }
	UINT GetlayoutId() const { return m_LayoutId; }

	void MouseMove(int x, int y);
	void MouseClick(int x, int y);
	void MoveEvent(const DirectX::XMFLOAT3 &movingDirection);
	void KeyDown(UINT keyCode);
	void ActivateSelectedGuiElement()
	{
		m_ClickableGuiElements[m_SelectedGuiElement]->VRun();
	}
private:
	// Every visible 2D entity(text, buttons, message boxes etc)
	std::vector<std::shared_ptr<IGuiElement> > m_GuiElements;
	// Gui elements which hold some action if user activate them
	std::vector<std::shared_ptr<IActiveGuiElement> > m_ClickableGuiElements;

	const LayoutUI *m_pParentLayout;
	UINT m_LayoutId;

	// Gui element which is actually selected(only, guis with action can be selected)
	int m_SelectedGuiElement;
};