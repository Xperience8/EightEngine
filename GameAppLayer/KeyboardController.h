#pragma once

class KeyboardController
{
private:
	// Mapping from keys(like 'W') to events(like 'MoveForward')
	std::vector<bool> m_KeyToEventMapping;

	DirectX::XMFLOAT3 m_MovingDirection;

	bool m_IsGamePaused;
public:
	KeyboardController() : m_MovingDirection(0.f, 0.f, 0.f), m_IsGamePaused(true) { }
	~KeyboardController() { }

	void Init();
	void Destroy()
	{
		m_KeyToEventMapping.clear();
		m_MovingDirection = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		m_IsGamePaused = true;
	}

	bool ProcessInput(UINT msg, WPARAM wparam, LPARAM lparam);

	void KeyDownEvent(UINT keyCode);
	void KeyUpEvent(UINT keyCode);

	void SetGameStatus(bool isGamePaused)
	{
		m_IsGamePaused = isGamePaused;
	}
};