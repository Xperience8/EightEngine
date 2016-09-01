#pragma once

#include "IUserInterface.h"
#include "LayoutUI.h"
#include "EightEngine\EventManager\EventManager.h"
#include "EightEngine\EventManager\Events\ChangeMenuScreenEventData.h"


class MenuUI : public IUserInterface
{
public:
	MenuUI() : m_pWorkingLayout(nullptr) { }
	~MenuUI() { }


	void VInit(UINT layoutSize) override
	{
		m_Layouts.resize(layoutSize);
		m_pWorkingLayout = &m_Layouts[0];
	}
	void VPostInit() override
	{
		for (auto &layout : m_Layouts)
		{
			layout.PostInit();
		}
	}
	void VRegisterEvents() override
	{
		g_EventManager.AddListener(EventListener(this, &MenuUI::ChangeScreenRequest),
			ChangeMenuScreenEventData::s_TYPE);
	}
	void VUnregisterEvents() override
	{
		g_EventManager.RemoveListener(reinterpret_cast<UINT>(this),
			ChangeMenuScreenEventData::s_TYPE);
	}
	void VRender(UIRenderingPass &renderingPass) override
	{
		m_pWorkingLayout->Render(renderingPass);
	}

	void VCreateLayout(UINT layoutId, UINT parentLayoutId) override
	{
		m_Layouts[layoutId].Init(layoutId, m_Layouts[parentLayoutId]);
	}
	void VAddGuiElementToLayout(UINT layoutId, 
		const std::shared_ptr<IGuiElement> &guiElement) override
	{
		m_Layouts[layoutId].AddGuiElement(guiElement);
	}

	const LayoutUI &GetVisibleLayout() const
	{
		return *m_pWorkingLayout;
	}

	const std::vector<std::shared_ptr<IGuiElement> > &VGetGuiElements() const override
	{
		EIGHT_ASSERT(m_pWorkingLayout, "Working layout doesn't exist");
		return m_pWorkingLayout->GetGuiElements();
	}

	void VMouseMove(int x, int y) override
	{
		EIGHT_ASSERT(m_pWorkingLayout, "Working layout doesn't exist");
		m_pWorkingLayout->MouseMove(x, y);
	}
	void VMouseClick(int x, int y) override
	{
		EIGHT_ASSERT(m_pWorkingLayout, "Working layout doesn't exist");
		m_pWorkingLayout->MouseClick(x, y);
	}
	void VMoveEvent(const DirectX::XMFLOAT3 &movingDirection) override
	{
		EIGHT_ASSERT(m_pWorkingLayout, "Working layout doesn't exist");
		m_pWorkingLayout->MoveEvent(movingDirection);
	}
	void ActivateSelectedGuiElement() override
	{
		m_pWorkingLayout->ActivateSelectedGuiElement();
	}
	void VKeyDown(UINT keyCode) override
	{
		EIGHT_ASSERT(m_pWorkingLayout, "Working layout doesn't exist");
		m_pWorkingLayout->KeyDown(keyCode);
	}
private:
	std::vector<LayoutUI> m_Layouts;
	LayoutUI *m_pWorkingLayout;

	void ChangeScreenRequest(const EventPtr<IEventData> &pEvent);
};