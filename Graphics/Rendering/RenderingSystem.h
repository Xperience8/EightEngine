#pragma once

#include "RenderingPass.h"
#include "FrustumCullingSystem.h"
#include "EightEngine\Graphics\IRenderableEntity.h"

class DirectionalLightComponent;
class GameCamera;

class LightManager
{
private:
	std::shared_ptr<DirectionalLightComponent> m_pLights;
public:
	LightManager() { }
	~LightManager() { }

	void Init();

	void AddLights(std::shared_ptr<DirectionalLightComponent> &pLights)
	{
		m_pLights = pLights;
	}
	void RemoveLights(UINT actorId);

	std::shared_ptr<DirectionalLightComponent> &GetLights() { return m_pLights; }
};

class RenderingSystem
{
private:
	std::vector<std::unique_ptr<RenderingPass> > m_RenderingPasses;

	LightManager m_LightManager;
public:
	RenderingSystem() { }
	~RenderingSystem() { }

	// Create rendering passes
	void Initialize(Renderer &pRenderer);

	// Render visible entities
	void Render(Renderer &renderer, std::shared_ptr<GameCamera> &pCamera);

	void AddLights(std::shared_ptr<DirectionalLightComponent> &pLights)
	{
		m_LightManager.AddLights(pLights);
	}
	void RemoveLights(UINT actorId)
	{
		m_LightManager.RemoveLights(actorId);
	}

	void AddRenderableEntity(const FrustumCullingModel &model)
	{
		m_RenderingPasses[static_cast<int>(
			model.pRenderableEntity->VGetRenderingType())]->AddRenderableEntity(model);
	}
};