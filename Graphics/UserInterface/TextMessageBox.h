#pragma once

#include "IGuiElement.h"
#include "Sprite.h"
#include "TextSprite.h"

class TextMessageBox : public IGuiElement
{
public:
	TextMessageBox() { }
	~TextMessageBox() { }

	// Creating Text message box from xml config file
	void VInit(const XmlNode &configNode,
		const std::shared_ptr<Screen> &pScreen) override;

	// Rendering text message box is performed in batch
	void VRender(UIRenderingPass &renderingPass) override;

	const std::shared_ptr<TextSprite> &GetTextSprite() const
	{
		return m_pTextSprite;
	}
private:
	std::shared_ptr<TextSprite> m_pTextSprite;
};