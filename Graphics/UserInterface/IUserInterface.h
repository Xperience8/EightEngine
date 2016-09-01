#pragma once

class IGuiElement;
class UIRenderingPass;

class IUserInterface
{
public:
	IUserInterface() { }
	virtual ~IUserInterface() { }

	virtual void VInit(UINT layoutCount) = 0;
	virtual void VPostInit() = 0;

	virtual void VRender(UIRenderingPass &renderingPass) = 0;

	virtual void VCreateLayout(UINT layoutId, UINT parentLayoutId) = 0;
	virtual void VAddGuiElementToLayout(UINT layoutId, 
		const std::shared_ptr<IGuiElement> &guiElement) = 0;

	virtual void VRegisterEvents() = 0;
	virtual void VUnregisterEvents() = 0;

	virtual const std::vector<std::shared_ptr<IGuiElement> > &VGetGuiElements() const = 0;

	virtual void VMouseClick(int x, int y) = 0;
	virtual void VMouseMove(int x, int y) = 0;
	virtual void VMoveEvent(const DirectX::XMFLOAT3 &movingDirection) = 0;
	virtual void ActivateSelectedGuiElement() = 0;
	virtual void VKeyDown(UINT keyCode) = 0;
};