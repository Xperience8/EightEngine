#pragma once

class TransformComponent;
class IRenderableEntity;
class GameCamera;
class Actor;

_declspec(align(16))
struct FrustumCullingModel
{
	DirectX::BoundingBox BoundingBox;

	// Actor which own this model -> used for Picking etc.
	std::shared_ptr<Actor> pActor;

	// Pointer to transformation component
	// This is direct pointer, so we don't have to ask actor every frame for his transform component
	// It is used to get world matrix
	TransformComponent *pTransform;

	// Pointer to render component
	// This is direct pointer, so we don't have to ask actor every frame for his render component
	// It is used for rendering
	IRenderableEntity *pRenderableEntity;

};

class FrustumCullingSystem
{
private:
	// List of every actor registered inside frustum culling system
	std::vector<FrustumCullingModel> m_Models;

	// Only visible actors, raw pointer is used because it's garanted that it remains valid
	// This list is rebuilt every frame
	std::vector<FrustumCullingModel*> m_VisibleModels;

	bool AABBFrustumIntersection(const FrustumCullingModel &model, 
		std::shared_ptr<GameCamera> &pCamera);
public:
	FrustumCullingSystem() { }
	~FrustumCullingSystem() { }

	// Add actor to frustum culling subsystem
	void RegisterActor(std::shared_ptr<Actor> &pActor);

	// Remove actor from frustum culling subsystem
	void RemoveActor(UINT ActorID);

	// New list of visible actors is created
	void BruteForceFrustumCulling(std::shared_ptr<GameCamera> &pCamera)
	{
		m_VisibleModels.clear();
		for (auto &model : m_Models)
		{
			if (AABBFrustumIntersection(model, pCamera))
			{
				m_VisibleModels.push_back(&model);
			}
		}
	}

	// Before this call you should call BruteForceFrustumCulling
	std::vector<FrustumCullingModel*> &GetVisibleModels() 
	{ 
		return m_VisibleModels; 
	}
};