#pragma once

#include "IGameView.h"
#include "GameCamera.h"
#include "PlayerController.h"
#include "EightEngine\Actor\Actor.h"
#include "EightEngine\GameAppLayer\Screen.h"

class PlayerView : public IGameView
{
public:
	PlayerView(std::weak_ptr<Actor> &pActor) : m_pActor(pActor), m_DebugCamera(false) { }
	~PlayerView() { }

	// Register events which can be handled by player view and player controller
	void VRegisterEvents() override;
	void VUnregisterEvents() override;

	// Initialize player view
	void VOnRestore() override;
	
	// Update camera and mouse controller
	void VOnResize(const std::shared_ptr<Screen> &pScreen) override
	{
		m_pCamera->CreateProjMatrix(0.5f * DirectX::XM_PI, 
			pScreen->GetScreenAspectRatio(), 0.01f, 1000.f);
	}

	// Update camera, read key inputs from PlayerController and update 
	// physics accordingly to it
	void VUpdate(float DeltaTime) override;
	
	void VRender() override;

	// Return id of actor who is linked with this view 
	UINT VGetActorID() const { return m_pActor._Get()->GetActorID(); }

	UINT VGetType() const override { return GameViewType::PLAYER_VIEW; }
protected:
	std::shared_ptr<GameCamera> m_pCamera;
	std::unique_ptr<PlayerController> m_pController;

	std::weak_ptr<Actor> m_pActor;

	bool m_DebugCamera;

	void ChangeDebugCameraStatus(const EventPtr<IEventData> &pEvent);
};

