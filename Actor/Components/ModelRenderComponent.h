#pragma once

#include "RenderComponent.h"

class Model;
class DebugModel;



class ModelRenderComponent : public RenderComponent
{
public:
	ModelRenderComponent() : m_ModelName("None"), m_RenderingType(RenderingType::Actor) { }
	~ModelRenderComponent() { }

	// Create model render component from xml config file
	bool VInit(const XmlNode &configNode) override;

	// Model render component has no depedencies on other component, so this call is empty
	bool VPostInit() override;

	// Model render component is used only for rendering so it does not have to be updated
	void VUpdate(float DeltaTime) override { }

	// Render model
	void VRender(Renderer &renderer) override;

	// Render in debug mode
	// Debug model must be added first
	void VRenderDebug(Renderer &renderer) override;
	
	// Return rendering as Actor
	RenderingType VGetRenderingType() const override { return m_RenderingType; }

	// Save model render component into xml config file
	void VSaveToXML(XmlNode &configNode) override;

	// Return bounding box, it is used for frustum culling
	const DirectX::BoundingBox &GetBoundingBox() const { return m_ModelVolume; }

	const std::shared_ptr<Model> &GetModel() const { return m_pModel; }

#if defined(EIGHT_EDITOR)
	void ModifyModel(const XmlNode &configNode);
#endif
private:
	std::string m_ModelName;

	// This is only used for frustum culling, physics has it's own collision model
	DirectX::BoundingBox m_ModelVolume;

	std::shared_ptr<Model> m_pModel;

	std::shared_ptr<DebugModel> m_pDebugModel;

	RenderingType m_RenderingType;
};

