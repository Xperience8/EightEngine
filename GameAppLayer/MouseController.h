#pragma once

class Screen;

class MouseController
{
private:
	const float m_PercentOfNew;
	const float m_PercentOfOld;

	float m_DeltaX;
	float m_DeltaY;

	bool m_IsCursorVisible;
public:
	MouseController();
	~MouseController() { }

	void Init(const std::shared_ptr<Screen> &pScreen);
	void Destroy();

	bool ProcessInput(UINT msg, WPARAM wparam, LPARAM lparam);

	void MouseMove(int mousePosX, int mousePosY);
	void ShowMouseCursor(bool cursorIsVisible);
};
