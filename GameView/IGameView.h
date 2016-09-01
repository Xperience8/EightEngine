#pragma once

class Renderer;
class EventManager;
class Screen;

enum GameViewType
{
	NO_VIEW = 0x00,
	PLAYER_VIEW = 0x01,
	MAIN_MENU_VIEW = 0x02,
	MENU_VIEW = 0x03
};

class IGameView
{
public:
	IGameView() { }
	virtual ~IGameView() { }

	// Register events which are needed for this view
	virtual void VRegisterEvents() = 0;

	// Unregister events
	virtual void VUnregisterEvents() = 0;

	// Initialize view
	virtual void VOnRestore() = 0;

	// Resize camera or controller
	virtual void VOnResize(const std::shared_ptr<Screen> &pScreen) = 0;
	
	// Update camera and controller
	virtual void VUpdate(float DeltaTime) = 0;

	// If this view has camera send its to renderer
	virtual void VRender() = 0;
	
	// Get id of actor who is linked with this view
	virtual UINT VGetActorID() const = 0;

	virtual UINT VGetType() const = 0;
};