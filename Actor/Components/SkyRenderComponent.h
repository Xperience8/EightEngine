#pragma once

#include "RenderComponent.h"
#include "EightEngine\Graphics\Vertex.h"

class Mesh;
class Material;
class RenderingDevice;

class SkyRenderComponent : public RenderComponent
{
public:
	SkyRenderComponent() { }
	~SkyRenderComponent() { }
	
	// Init sky render component from xml config file
	bool VInit(const XmlNode &configNode) override;

	// Sky render component has no depedencies on other components so this call is empty
	bool VPostInit() override { return true; }

	// Sky render component is only used for rendering so this call is empty
	void VUpdate(float DeltaTime) override { }

	// Render sky
	// Set mesh, material, update constant buffers and draw indexed
	void VRender(Renderer &renderer) override;

	// Save render component into xml config file(actuall status is saved)
	void VSaveToXML(XmlNode &configNode) override;

	// Return that this is Sky
	RenderingType VGetRenderingType() const override { return RenderingType::Sky; }
private:
	std::shared_ptr<Mesh> m_pMesh;
	std::shared_ptr<Material> m_pMaterial;

	std::string m_TextureName;

	// Generate sphere and build mesh
	bool BuildBuffers(float radius, UINT stackCount, UINT sliceCount,
		const std::shared_ptr<RenderingDevice> &pRenderingDevice);
	void GenerateVertexBufferData(float radius, UINT stackCount, UINT sliceCount,
		std::vector<PosVertex> &vertices);
	void GenerateIndexBufferData(UINT lastVertexID, UINT stackCount, UINT sliceCount,
		std::vector<USHORT> &indices);
};