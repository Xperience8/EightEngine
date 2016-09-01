#include "stdafx.h"
#include "SkyRenderComponent.h"
#include "TransformComponent.h"
#include "EightEngine\Graphics\ApiWrapper\Renderer.h"
#include "EightEngine\Graphics\Mesh.h"
#include "EightEngine\Graphics\Material.h"
#include "EightEngine\ResourceCache\ResourceCache.h"

bool SkyRenderComponent::VInit(const XmlNode &configNode)
{
	m_TextureName = configNode.GetChildrenNode("Texture").GetValue();

	m_pMaterial = std::make_shared<Material>();
	m_pMaterial->Init(g_Renderer.GetRenderingDeviceContext());
	m_pMaterial->AddTexture(g_ResourceCache.RequestResource<Texture2D>(
		m_TextureName));

	return BuildBuffers(5000, 30, 30, g_Renderer.GetRenderingDevice());
}
void SkyRenderComponent::VRender(Renderer &renderer)
{
	auto &pRenderingDeviceContext = renderer.GetRenderingDeviceContext();

	// Update constant buffer with model information
	ModelCBData modelCBData(m_pParentActor._Get()->GetActorComponent<
		TransformComponent>()->GetWorldMatrix());
	pRenderingDeviceContext->GetConstantBuffer(ModelCBData::NAME)->UpdateBuffer(
		&modelCBData, pRenderingDeviceContext);

	m_pMesh->Apply(pRenderingDeviceContext);
	m_pMaterial->Apply(pRenderingDeviceContext);

	pRenderingDeviceContext->DrawIndexed(m_pMesh->GetIndexCount(), 0, 0);
}
bool SkyRenderComponent::BuildBuffers(float radius, UINT stackCount, 
	UINT sliceCount, const std::shared_ptr<RenderingDevice> &pRenderingDevice)
{
	// Generate vertex and index data and create mesh
	std::vector<PosVertex> vertices;
	GenerateVertexBufferData(radius, stackCount, sliceCount, vertices);
	std::vector<USHORT> indices;
	GenerateIndexBufferData(static_cast<UINT>(vertices.size()) - 1, stackCount, 
		sliceCount, indices);

	m_pMesh = std::make_shared<Mesh>();
	return m_pMesh->Init(MeshDesc(vertices, indices), pRenderingDevice);
}
void SkyRenderComponent::GenerateVertexBufferData(float radius, 
	UINT stackCount, UINT sliceCount, std::vector<PosVertex> &vertices)
{
	// Generate vertices for sphere
	// Vertices are compound from position only

	vertices.push_back(PosVertex(0.f, radius, 0.f));

	float phiStep = DirectX::XM_PI / static_cast<float>(stackCount);
	float thetaStep = DirectX::XM_2PI / static_cast<float>(sliceCount);

	for (UINT i = 1; i < stackCount; i++)
	{
		float phi = phiStep * i;
		for (UINT j = 0; j <= sliceCount; j++)
		{
			float theta = thetaStep * j;

			vertices.push_back(PosVertex(
				radius * sinf(phi) * cosf(theta), 
				radius * cosf(phi), 
				radius * sinf(phi) * sinf(theta))
				);
		}
	}

	vertices.push_back(PosVertex(0.f, -radius, 0.f));
}
void SkyRenderComponent::GenerateIndexBufferData(UINT lastVertexID, 
	UINT stackCount, UINT sliceCount, std::vector<USHORT> &indices)
{
	// Generate index buffer for sphere

	// Compute indices for top stack
	for (UINT i = 1; i <= sliceCount; i++)
	{
		indices.push_back(0);
		indices.push_back(i + 1);
		indices.push_back(i);
	}

	USHORT baseIndex = 1;
	USHORT ringVertexCount = static_cast<USHORT>(sliceCount + 1);
	for (UINT i = 0; i < stackCount - 2; ++i)
	{
		for (UINT j = 0; j < sliceCount; j++)
		{
			indices.push_back(baseIndex + i * ringVertexCount + j);
			indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

			indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
			indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
		}
	}

	baseIndex = lastVertexID - ringVertexCount;

	// Compute indices for bottom stack
	for (UINT i = 0; i < sliceCount; i++)
	{
		indices.push_back(lastVertexID);
		indices.push_back(baseIndex + i);
		indices.push_back(baseIndex + i + 1);
	}
}
void SkyRenderComponent::VSaveToXML(XmlNode &configNode)
{
	XmlNode componentNode = configNode.CreateChildNode("SkyRenderComponent");
	componentNode.CreateAttribute("type", "RenderComponent");

	componentNode.CreateChildNode("Texture", m_TextureName);
}