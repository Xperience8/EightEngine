#pragma once

#include "Mesh.h"
#include "Material.h"
#include "SubsetTable.h"
#include "EightEngine\Xml\XmlDocument.h"

class RenderingModelResourceLoader;

class Model
{
	friend RenderingModelResourceLoader;
protected:
	std::vector<Mesh> m_Meshes;
	std::vector<Material> m_Materials;
	std::vector<SubsetTable> m_Subsets;

	DirectX::BoundingBox m_FrustumCullingVolume;
public:
	Model() { }
	~Model() { }

	// Rendering model from subset table
	// meshes and materials are binding as pair
	// Then mesh is drown indexed
	void Render(const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
	{
		for (auto &subset : m_Subsets)
		{
			m_Meshes[subset.GetMeshID()].Apply(pRenderingDeviceContext);
			m_Materials[subset.GetMaterialID()].Apply(pRenderingDeviceContext);

			pRenderingDeviceContext->DrawIndexed(subset.GetIndexCount(),
				subset.GetIndexStart(), 0);
		}
	}

	// Getting frustum cullin volume
	// This volume is only used in frustum culling, physics has its own model
	const DirectX::BoundingBox &GetFrustumCullingVolume() const 
	{ 
		return m_FrustumCullingVolume; 
	}

	bool HasTextures() const
	{
		EIGHT_ASSERT(!m_Materials.empty(), "This model doesn't have any material");

		bool hasTextures = true;
		for (auto &material : m_Materials)
		{
			hasTextures &= material.HasTextures();
		}

		return hasTextures;
	}

	std::vector<Material> &GetMaterials()
	{
		return m_Materials;
	}

#if defined(EIGHT_EDITOR)
	void SaveDescToXml(XmlDocument &doc)
	{
		doc.Init("RenderingModelDesc");
		
		auto materialsNode = doc.GetRootNode().CreateChildNode("Materials");
		materialsNode.CreateAttribute("count", std::to_string(m_Materials.size()));
		
		for (auto &material : m_Materials)
		{
			material.SaveToXml(materialsNode.CreateChildNode("Material"));
		}
	}
	void Modify(const XmlNode &node)
	{
		auto materialNode = node.GetChildrenNode("Materials").GetChildrenNode();
		for (auto &material : m_Materials)
		{
			material.Modify(materialNode);
			materialNode.GoToNextNode();
		}
	}
#endif
};
class DebugModel
{
private:
	Mesh m_DebugMesh;
public:
	DebugModel() { }
	~DebugModel() { }

	// Creating Mesh
	// There can be only one debug rendering mesh
	bool Init(const MeshDesc &meshDesc, 
		const std::shared_ptr<RenderingDevice> &pRenderingDevice)
	{
		return m_DebugMesh.Init(meshDesc, pRenderingDevice);
	}

	// Debug mesh is binded and then it is drown indexed
	void Render(const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
	{
		m_DebugMesh.Apply(pRenderingDeviceContext);
		
		pRenderingDeviceContext->DrawIndexed(m_DebugMesh.GetIndexCount(), 0, 0);
	}
};