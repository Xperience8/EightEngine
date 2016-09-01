#pragma once

#include "IGuiElement.h"
#include "Sprite.h"

class BackgroundImage : public IGuiElement
{
public:
	BackgroundImage() { }
	~BackgroundImage() { }

	// Creating background image from xml config file
	void VInit(const XmlNode &configNode,
		const std::shared_ptr<Screen> &pScreen) override;

	// Rendering is performed in batch
	void VRender(UIRenderingPass &renderingPass) override;

	const std::shared_ptr<Sprite> &GetSprite() const
	{
		return m_pSprite;
	}
private:
	std::shared_ptr<Sprite> m_pSprite;
};

class BackgroundQuad : public IGuiElement
{
public:
	BackgroundQuad() { }
	~BackgroundQuad() { }

	// Creating background quad from xml config file
	void VInit(const XmlNode &configNode,
		const std::shared_ptr<Screen> &pScreen) override;

	// Rendering is performed in batch
	void VRender(UIRenderingPass &renderingPass) override;

private:
	std::vector<PosColVertex> m_Vertices;
};