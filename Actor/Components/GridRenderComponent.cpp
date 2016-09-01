#include "stdafx.h"
#include "GridRenderComponent.h"
#include "EightEngine\Graphics\ApiWrapper\Renderer.h"
#include "EightEngine\ResourceCache\ResourceCache.h"
#include "EightEngine\Graphics\Model.h"
#include "EightEngine\Graphics\Effect.h"
#include "EightEngine\Actor\Components\TransformComponent.h"
#include "EightEngine\EventManager\EventManager.h"

bool GridRenderComponent::VInit(const XmlNode &configNode)
{
	if (m_pMaterial)
		m_pMaterial.reset();
	if (m_pMesh)
		m_pMesh.reset();

	m_pMesh = std::make_shared<Mesh>();
	m_pMaterial = std::make_shared<Material>();
	m_pMaterial->Init(g_Renderer.GetRenderingDeviceContext());

	auto parameterNode = configNode.GetChildrenNode();
	while (parameterNode.Exist())
	{
		std::string parameterName = parameterNode.GetName();
		if (parameterName == "Texture")
		{
			std::string textureName = parameterNode.GetValue();
			if (textureName.find("_nmap") != std::string::npos)
				m_BumpTextureName = textureName;
			else m_DiffuseTextureName = textureName;

			m_pMaterial->AddTexture(g_ResourceCache.RequestResource<Texture2D>(textureName));
		}
		else if (parameterName == "HeightMapWidth")
			m_HeightMapWidth = std::stoi(parameterNode.GetValue());
		else if (parameterName == "HeightMapHeight")
			m_HeightMapHeight = std::stoi(parameterNode.GetValue());
		else if (parameterName == "CellsSpace")
			m_CellSpace = std::stof(parameterNode.GetValue());
		else if (parameterName == "CellsPerPatch")
			m_CellsPerPatch = std::stoi(parameterNode.GetValue());
		else if (parameterName == "TextureScaling")
		{
			m_TextureScaling.x = std::stof(parameterNode.GetAttribute("x").GetValue());
			m_TextureScaling.y = std::stof(parameterNode.GetAttribute("y").GetValue());
		}

		parameterNode.GoToNextNode();
	}


	if (!CreateBuffers(g_Renderer.GetRenderingDevice()))
		return false;

	LightMaterial material;
	material.Ambient = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.f);
	material.Diffuse = DirectX::XMFLOAT4(0.9f, 0.9f, 0.9f, 1.f);
	material.Specular = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 32.f);
	material.Reflection = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f);

	m_pMaterial->SetLightMaterial(material);

	return true;
}


void GridRenderComponent::VRender(Renderer &renderer)
{
	auto &pRenderingDeviceContext = renderer.GetRenderingDeviceContext();

	// Updating terrain constant buffer with texture scaling(used for tiling texture)
	TerrainCBData terrainCBData(m_TextureScaling);
	pRenderingDeviceContext->GetConstantBuffer(TerrainCBData::NAME)->UpdateBuffer(
		&terrainCBData, pRenderingDeviceContext);

	// Update model constant buffer with information about model such as world matrix etc.
	ModelCBData modelCBData(m_pParentActor._Get()->GetActorComponent<
		TransformComponent>()->GetWorldMatrix());
	pRenderingDeviceContext->GetConstantBuffer(ModelCBData::NAME)->UpdateBuffer(
		&modelCBData, pRenderingDeviceContext);

	// Set mesh and material into rendering pipeline
	m_pMesh->Apply(pRenderingDeviceContext);
	m_pMaterial->Apply(pRenderingDeviceContext);

	// Draw indexed
	pRenderingDeviceContext->DrawIndexed(m_pMesh->GetIndexCount(), 0, 0);
}
bool GridRenderComponent::CreateBuffers(
	const std::shared_ptr<RenderingDevice> &pRenderingDevice)
{
	// Calculating how many patch we need
	UINT numPatchVertexRows = ((m_HeightMapHeight - 1) / m_CellsPerPatch) + 1;
	UINT numPatchVertexColl = ((m_HeightMapWidth - 1) / m_CellsPerPatch) + 1;

	// Calculating total size
	m_TotalWidth = static_cast<float>((m_HeightMapWidth - 1)) * m_CellSpace;
	m_TotalDepth = static_cast<float>((m_HeightMapHeight - 1)) * m_CellSpace;

	float halfWidth = 0.5f * m_TotalWidth;
	float halfDepth = 0.5f * m_TotalDepth;

	// Calculating size of one patch
	float patchWidth = m_TotalWidth / (numPatchVertexColl - 1);
	float patchDepth = m_TotalDepth / (numPatchVertexRows - 1);
	float u = 1.f / (numPatchVertexColl - 1);
	float v = 1.f / (numPatchVertexRows - 1);

	// Creating vertices
	std::vector<PosTexVertex> vertices(numPatchVertexRows * numPatchVertexColl);
	for (UINT i = 0; i < numPatchVertexRows; i++)
	{
		float z = halfDepth - i * patchDepth;
		for (UINT j = 0; j < numPatchVertexColl; j++)
		{
			float x = -halfWidth + j * patchWidth;
			vertices[i * numPatchVertexColl + j].Position = DirectX::XMFLOAT3(x, 0.f, z);
			vertices[i * numPatchVertexColl + j].TexCoord = DirectX::XMFLOAT2(j * u, i * v);
		}
	}

	// Creating indices
	std::vector<UINT> indices;
	indices.reserve((numPatchVertexRows - 1) * (numPatchVertexColl - 1));
	for (UINT i = 0; i < numPatchVertexRows - 1; i++)
	{
		for (UINT j = 0; j < numPatchVertexColl - 1; j++)
		{
			indices.push_back(i * numPatchVertexColl + j);
			indices.push_back(i * numPatchVertexColl + j + 1);
			indices.push_back((i + 1) * numPatchVertexColl + j);
			indices.push_back((i + 1) * numPatchVertexColl + j + 1);
		}
	}
	return m_pMesh->Init(MeshDesc(vertices, indices), pRenderingDevice);
}
void GridRenderComponent::VSaveToXML(XmlNode &configNode)
{
	// Future Work
	EIGHT_ASSERT(false, "GridRenderComponent::VSaveToXML is not fully created");
}