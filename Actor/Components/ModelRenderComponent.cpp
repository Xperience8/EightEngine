#include "stdafx.h"
#include "ModelRenderComponent.h"
#include "TransformComponent.h"
#include "EightEngine\Actor\ActorFactory.h"
#include "EightEngine\Graphics\ApiWrapper\Renderer.h"
#include "EightEngine\Graphics\Model.h"
#include "EightEngine\ResourceCache\ResourceCache.h"
#include "PhysicsComponent.h"

bool ModelRenderComponent::VInit(const XmlNode &configNode)
{
	m_ModelName = configNode.GetChildrenNode("GeometryData").GetValue();

	//m_pModel = g_ResourceCache.RequestResource<Model>(m_ModelName);
	m_pModel = g_ResourceCache.RequestResource<Model>(m_ModelName);

	if (!m_pModel->HasTextures())
	{
		m_RenderingType = RenderingType::TransparentActor;
	}
	// Future work
	// Really dangerous if Model doesn't exist in init function call
	// Delete this after optimizing
	m_ModelVolume = m_pModel->GetFrustumCullingVolume();

	return true;
}
bool ModelRenderComponent::VPostInit()
{
	auto pPhysicsComponent = m_pParentActor._Get()->GetActorComponent<PhysicsComponent>();
	if (pPhysicsComponent)
	{
		std::vector<DirectX::XMFLOAT3> vertices;
		std::vector<USHORT> indices;
		pPhysicsComponent->GetPhysicsModel(vertices, indices);

		MeshDesc desc(vertices, indices);
		m_pDebugModel = std::make_shared<DebugModel>();
		if (!m_pDebugModel->Init(desc, g_Renderer.GetRenderingDevice()))
		{
			EIGHT_ERROR("Creating debug rendering model failed");
			return false;
		}
	}

	return true;
}
void ModelRenderComponent::VRender(Renderer &renderer)
{
	auto &pRenderingDeviceContext = renderer.GetRenderingDeviceContext();

	ModelCBData modelCBData(m_pParentActor._Get()->GetActorComponent<
		TransformComponent>()->GetWorldMatrix());
	pRenderingDeviceContext->GetConstantBuffer(ModelCBData::NAME)->UpdateBuffer(
		&modelCBData, pRenderingDeviceContext);

	m_pModel->Render(pRenderingDeviceContext);
}
void ModelRenderComponent::VRenderDebug(Renderer &renderer)
{
	auto &pRenderingDeviceContext = renderer.GetRenderingDeviceContext();

	if (m_pDebugModel)
	{
		ModelCBData modelCBData(m_pParentActor._Get()->GetActorComponent <
			TransformComponent > ()->GetWorldMatrix());
		pRenderingDeviceContext->GetConstantBuffer(ModelCBData::NAME)->UpdateBuffer(
			&modelCBData, pRenderingDeviceContext);

		m_pDebugModel->Render(pRenderingDeviceContext);
	}
}

void ModelRenderComponent::VSaveToXML(XmlNode &configNode)
{
	XmlNode componentNode = configNode.CreateChildNode("ModelRenderComponent");
	componentNode.CreateAttribute("type", "RenderComponent");

	componentNode.CreateChildNode("GeometryData", m_ModelName);
}
#if defined(EIGHT_EDITOR)
void ModelRenderComponent::ModifyModel(const XmlNode &configNode)
{
	m_pModel->Modify(configNode);
}
#endif