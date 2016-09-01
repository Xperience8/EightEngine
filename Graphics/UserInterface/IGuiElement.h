#pragma once

class XmlNode;
class ResourceCache;
class Screen;
class UIRenderingPass;

class IGuiElement
{
public:
	IGuiElement() { }
	virtual ~IGuiElement() { }

	virtual void VInit(const XmlNode &configNode,
		const std::shared_ptr<Screen> &pScreen) = 0;

	virtual void VRender(UIRenderingPass &renderingPass) = 0;

	virtual bool VIsActiveElement() const { return false; }
};