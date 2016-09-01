#pragma once

#include "IActiveGuiElement.h"

#include "BackgroundImage.h"
#include "TextMessageBox.h"

class KulaWorldLevelBox : public IActiveGuiElement
{
public:
	KulaWorldLevelBox() : m_Locked(true) { }
	~KulaWorldLevelBox() { }

	void VInit(const XmlNode &configNode, const std::shared_ptr<Screen> &pScreen) override;

	void VRun() override;

	void VRender(UIRenderingPass &renderingPass) override;

	void VRegisterAction(const std::string &actionType,
		const std::string &actionName) override
	{
		m_Action = actionName;
		m_ActionType = actionType;
	}

	bool VMouseEvent(int x, int y) override
	{
		return (x >= m_TopLeftCorner.x && x <= m_BottomRightCorner.x &&
			y >= m_TopLeftCorner.y && y <= m_BottomRightCorner.y);
	}

	void VHighLightOn() override
	{
		m_LevelLabel.GetTextSprite()->ChangeColor(m_HighLightedColor);
	}
	
	void VHighLightOff() override
	{
		m_LevelLabel.GetTextSprite()->ChangeColor(m_NormalColor);
	}

private:
	static const UINT NUM_STARS = 3;

	enum GuiPart
	{
		Border,
		Star1,
		Star2,
		Star3,
		Lock
	};

	std::vector<BackgroundImage> m_Images;

	TextMessageBox m_LevelLabel;

	bool m_Locked;

	std::string m_LevelName;

	std::string m_Action;
	std::string m_ActionType;

	DirectX::XMFLOAT4 m_HighLightedColor;
	DirectX::XMFLOAT4 m_NormalColor;

	DirectX::XMFLOAT3 m_TopLeftCorner;
	DirectX::XMFLOAT3 m_BottomRightCorner;
};