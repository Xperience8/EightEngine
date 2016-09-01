#pragma once

#include "IGuiElement.h"

class EventManager;

class IActiveGuiElement : public IGuiElement
{
public:
	IActiveGuiElement() { }
	virtual ~IActiveGuiElement() { }

	bool VIsActiveElement() const override { return true; }

	virtual void VRegisterAction(const std::string &actionType,
		const std::string &actionName) = 0;

	virtual bool VMouseEvent(int x, int y) = 0;
	virtual void VRun() = 0;

	virtual void VHighLightOn() = 0;
	virtual void VHighLightOff() = 0;
};