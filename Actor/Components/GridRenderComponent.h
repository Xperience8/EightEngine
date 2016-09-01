#pragma once

#include "RenderComponent.h"
#include "EightEngine\Graphics\Vertex.h"

class Mesh;
class Material;
class Effect;
class RenderingDevice;

class GridRenderComponent : public RenderComponent
{
public:
	GridRenderComponent() : m_TotalWidth(0.f), m_TotalDepth(0.f), 
		m_TextureScaling(1.f, 1.f) { }
	~GridRenderComponent() { }

	// Creating grid from xml config file
	bool VInit(const XmlNode &configNode) override;

	// Grid render component has no depedencies on other components so this call is empty
	bool VPostInit() override { return true; }
	
	// This is static terrain, so this call is empty
	void VUpdate(float DeltaTime) override { }

	// Rendering grid(frustum culling is performed in gpu)
	// Constant buffers are updated with new data, mesh and material is set into rendering pipeline
	// and draw indexed call is performed
	void VRender(Renderer &renderer) override;

	// Returning rendering type -> Terrain
	RenderingType VGetRenderingType() const override { return RenderingType::Terrain; }

	// Test wheter point is inside or outside
	bool InBound(float x, float y, float z) const 
	{ 
		return ((x >= -0.5f * m_TotalWidth && x <= 0.5f * m_TotalWidth) && (z >= -0.5f * m_TotalDepth && z <= 0.5f * m_TotalDepth));
	}

	// Save grid render component into xml config file(actual status is saved)
	void VSaveToXML(XmlNode &configNode) override;
private:
	std::shared_ptr<Mesh> m_pMesh;
	std::shared_ptr<Material> m_pMaterial;

	DirectX::XMFLOAT2 m_TextureScaling;

	float m_TotalWidth;
	float m_TotalDepth;

	// Editor stuff
	std::string m_DiffuseTextureName;
	std::string m_BumpTextureName;
	UINT m_HeightMapWidth;
	UINT m_HeightMapHeight;
	float m_CellSpace;
	UINT m_CellsPerPatch;

	bool CreateBuffers(const std::shared_ptr<RenderingDevice> &pRenderingDevice);
};