#pragma once

#include "IActiveGuiElement.h"
#include "TextSprite.h"
#include "Sprite.h"

class TextButton : public IActiveGuiElement
{
public:
	TextButton() : m_HighLightedColor(1.f, 0.f, 0.f, 1.f), m_NormalColor(1.f, 1.f, 1.f, 1.f),
		m_TopLeftCorner(-1.f, 1.f, 0.f), m_BottomRightCorner(1.f, -1.f, 0.f) { }
	~TextButton() { }

	// Creating text button from xml config file
	void VInit(const XmlNode &configNode, const std::shared_ptr<Screen> &pScreen) override;

	// Call this to execute script after button was pressed
	void VRun() override;

	// Rendering text button is performer in batch
	void VRender(UIRenderingPass &renderingPass) override;

	// What should button do if it was pressed is in this two values:
	// actionType -> either SendEvent or GoToLayout
	// actionName -> event with which name should be send or number of layout to go in
	void VRegisterAction(const std::string &actionType,
		const std::string &actionName) override
	{

	}

	// Test only whether mouse is inside button(nothing else, if you want to execute button script call VRun)
	bool VMouseEvent(int x, int y) override
	{
		return (x >= m_TopLeftCorner.x && x <= m_BottomRightCorner.x &&
			y >= m_TopLeftCorner.y && y <= m_BottomRightCorner.y);
	}
	void VHighLightOn() override
	{
		m_pTextSprite->ChangeColor(m_HighLightedColor);
	}
	void VHighLightOff() override
	{
		m_pTextSprite->ChangeColor(m_NormalColor);
	}
private:
	struct Action
	{
		std::string ActionType;
		std::string ActionParameters;
	};
	std::vector<Action> m_Actions;
	
	std::shared_ptr<TextSprite> m_pTextSprite;

	DirectX::XMFLOAT4 m_HighLightedColor;
	DirectX::XMFLOAT4 m_NormalColor;

	DirectX::XMFLOAT3 m_TopLeftCorner;
	DirectX::XMFLOAT3 m_BottomRightCorner;
};