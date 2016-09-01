#include "stdafx.h"
#include "MenuUI.h"

void MenuUI::ChangeScreenRequest(const EventPtr<IEventData> &pEvent)
{
	UINT newScreenId = StaticPointerCast<ChangeMenuScreenEventData>(
		pEvent)->GetNewMenuScreenId();

	if (newScreenId == -1)
	{
		newScreenId = m_pWorkingLayout->GetParentLayoutId();
	}
	m_pWorkingLayout = &m_Layouts[newScreenId];
}