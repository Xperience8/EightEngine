#pragma once

#include "FrustumCullingSystem.h"
#include "RenderingSystem.h"

class GameCamera;
#include "EightEngine\EventManager\IEventData.h"

class RenderingScene
{
public:
	RenderingScene() { }
	~RenderingScene() { }

	// Creating and initializing frustum culling system and rendering system
	void Initialize(Renderer &renderer);

	// Register events listeners(e.g CreateActor, DestroyActor)
	void RegisterEvents();

	// Unregister events listeners
	void UnregisterEvents();

	// Catch NewActorEventData event and register actor in frustum culling system
	void AddRenderableEntity(const EventPtr<IEventData> &pEvent);

	// Remove actor from frustum culling system
	void RemoveRenderableEntity(const EventPtr<IEventData> &pEvent);

	//UINT Picking(int sx, int sy, std::shared_ptr<GameCamera> &pCamera);
	UINT Picking(DirectX::FXMVECTOR rayOrigin, DirectX::FXMVECTOR rayDirection);

	// Perform frustum culling test(actually brute force frustum culling)
	// and copy visible entities into rendering subsystem
	// Rendering subsystem register them and sorts them based on effects which is 
	// needed for their rendering and other criterias(such as meshes, textures, etc.)
	void Update(std::shared_ptr<GameCamera> &pCamera, Renderer &renderer);

	// Rendering system renders scene
	void Render(std::shared_ptr<GameCamera> &pCamera, Renderer &renderer)
	{
		m_pRenderingSystem->Render(renderer, pCamera);
	}
private:
	std::vector<FrustumCullingModel> m_AllwaysVisibleEntities;

	std::unique_ptr<FrustumCullingSystem> m_pFrustumCullingSystem;
	std::unique_ptr<RenderingSystem> m_pRenderingSystem;
};