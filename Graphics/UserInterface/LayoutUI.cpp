#include "stdafx.h"
#include "LayoutUI.h"
#include "EightEngine\EventManager\EventManager.h"
#include "EightEngine\EventManager\Events\ChangeMenuScreenEventData.h"

void LayoutUI::KeyDown(UINT keyCode)
{

}
void LayoutUI::MoveEvent(const DirectX::XMFLOAT3 &movingDirection)
{
	EIGHT_ASSERT(movingDirection.z >= -1.f || movingDirection.z <= 1.f,
		"Moving direction must be in <-1, 1> range");

	m_ClickableGuiElements[m_SelectedGuiElement]->VHighLightOff();

	m_SelectedGuiElement -= static_cast<int>(movingDirection.z);

	if (m_SelectedGuiElement > static_cast<int>(m_ClickableGuiElements.size()) - 1)
	{
		m_SelectedGuiElement = 0;
	}
	else if (m_SelectedGuiElement < 0)
	{
		m_SelectedGuiElement = static_cast<int>(m_ClickableGuiElements.size() - 1);
	}

	m_ClickableGuiElements[m_SelectedGuiElement]->VHighLightOn();
}
void LayoutUI::MouseClick(int x, int y)
{
	for (auto &guiElement : m_ClickableGuiElements)
	{
		if (guiElement->VMouseEvent(x, y))
		{
			guiElement->VRun();
			break;
		}
	}
}
void LayoutUI::MouseMove(int x, int y)
{
	for (UINT i = 0; i < m_ClickableGuiElements.size(); i++)
	{
		auto &guiElement = m_ClickableGuiElements[i];
		if (guiElement->VMouseEvent(x, y))
		{
			m_ClickableGuiElements[m_SelectedGuiElement]->VHighLightOff();
			m_SelectedGuiElement = i;
			guiElement->VHighLightOn();
		}
	}
}

