#pragma once

class Renderer;
class Effect;
class GameCamera;

struct FrustumCullingModel;

class RenderingPass
{
protected:
	// Entities for rendering a.k.a Rendering commands
	std::vector<const FrustumCullingModel*> m_RenderableEntities;

	// Rendering pipeline which is set for entities in this class(only in normal rendering,
	// pipeline in debug rendering is set by Rendering system)
	std::shared_ptr<Effect> m_pEffect;
public:
	RenderingPass() { }
	virtual ~RenderingPass() { }

	virtual void Render(Renderer &renderer,
		const std::shared_ptr<GameCamera> &pCamera);

	void RenderDebug(Renderer &renderer);

	// Clearing command list
	void PostRender() 
	{ 
		m_RenderableEntities.clear(); 
	}

	// Adding renderable entity a.k.a rendering command(only pushing into list)
	void AddRenderableEntity(const FrustumCullingModel &model)
	{
		m_RenderableEntities.push_back(&model);
	}

	void SetRenderingPipeline(const std::shared_ptr<Effect> &pEffect) 
	{ 
		m_pEffect = pEffect; 
	}
};