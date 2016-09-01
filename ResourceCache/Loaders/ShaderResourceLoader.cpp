#include "stdafx.h"
#include "ShaderResourceLoader.h"
#include "EightEngine\ResourceCache\ResourceHandle.h"

#include "EightEngine\Graphics\ConstantBufferCache.h"
#include "EightEngine\ResourceCache\ResourceCache.h"
#include "EightEngine\Graphics\ApiWrapper\Shaders\VertexShader.h"
#include "EightEngine\Graphics\ApiWrapper\Shaders\HullShader.h"
#include "EightEngine\Graphics\ApiWrapper\Shaders\DomainShader.h"
#include "EightEngine\Graphics\ApiWrapper\Shaders\GeometryShader.h"
#include "EightEngine\Graphics\ApiWrapper\Shaders\PixelShader.h"
#include "EightEngine\Graphics\ApiWrapper\Shaders\ComputeShader.h"
#include "EightEngine\Graphics\ApiWrapper\States\SamplerState.h"
#include "EightEngine\Graphics\ApiWrapper\InputLayout.h"

#include "EightEngine\Graphics\ApiWrapper\RenderingDevice.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDeviceContext.h"
#include "EightEngine\Graphics\RenderingStatesCache.h"
#include "EightEngine\Graphics\ApiWrapper\Renderer.h"


typedef ResourceHandle<ShaderBase> ShaderResourceHandle;

ShaderResourceLoader::ShaderResourceLoader(
	const std::shared_ptr<RenderingStatesCache> &pRenderingStatesCache,
	const std::string &shaderConfigName) : m_pRenderingStatesCache(pRenderingStatesCache), m_pShaderConfig(g_ResourceCache.RequestResource<XmlDocument>(shaderConfigName))
{
	CreateInputLayoutsDesc();
}

bool ShaderResourceLoader::VCreateResource(BYTE *pRawMemory, UINT MemorySize,
	const std::string &Name, std::unique_ptr<ResourceHandleBase> &pResource)
{
	// Getting shader config node
	std::string shaderName = Name.substr(Name.find("Shaders/") + 8);
	shaderName = shaderName.substr(0, shaderName.find_last_of("."));
	auto shaderConfigNode = m_pShaderConfig->GetRootNode().GetChildrenNode(shaderName);
	if (!shaderConfigNode.Exist())
	{
		EIGHT_WARNING("Loaded shader is not in config file!");
		return true;
	}

	// If resource handle was not requested
	if (!pResource)
	{
		pResource = std::make_unique<ShaderResourceHandle>(Name);
	}

	// Start creating and configuring shader
	auto &pShader = static_cast<ShaderResourceHandle*>(pResource.get())->GetResource();
	
	CreateEmptyShader(static_cast<ShaderType>(std::stoi(
		shaderConfigNode.GetAttribute().GetValue())), pShader);
	if (!ConfigureShader(pRawMemory, MemorySize, pShader, shaderConfigNode))
	{
		EIGHT_ERROR("Configuring shader failed");
		return false;
	}

	EIGHT_LOG("Shader: " + Name + " is loaded and initialized");

	//pResource->SetInitializationStatus(true);
	pResource->PostInit();

	return true;
}
bool ShaderResourceLoader::ConfigureShader(const BYTE *pRawMemory, UINT memorySize,
	std::shared_ptr<ShaderBase> &pShader, const XmlNode &configNode)
{
	auto &pRenderingDevice = g_Renderer.GetRenderingDevice();
	auto &pRenderingDeviceContext = g_Renderer.GetRenderingDeviceContext();

	if (!pShader->VCreate(pRawMemory, memorySize, pRenderingDevice,
		pRenderingDeviceContext))
	{
		return false;
	}

	// Start configuring shader
	auto shaderParameterNode = configNode.GetChildrenNode();
	while (shaderParameterNode.Exist())
	{
		std::string parameterName = shaderParameterNode.GetName();
		if (parameterName == "ConstantBuffers")
		{
			auto cbNode = shaderParameterNode.GetChildrenNode();
			while (cbNode.Exist())
			{
				pShader->AddConstantBuffer(pRenderingDeviceContext->GetConstantBuffer(
					cbNode.GetValue()));
				// BindingSlot;
				cbNode.GoToNextNode();

				cbNode.GoToNextNode();
			}
		}
		else if (parameterName == "SamplerStates")
		{
			auto ssNode = shaderParameterNode.GetChildrenNode();
			while (ssNode.Exist())
			{
				pShader->AddSamplerState(m_pRenderingStatesCache->GetSamplerState(
					ssNode.GetValue()));
				// BindingSlot;
				ssNode.GoToNextNode();

				ssNode.GoToNextNode();
			}
		}
		else if (parameterName == "InputLayout")
		{
			auto pInputLayout = g_ResourceCache.RequestResource<InputLayout>(
				shaderParameterNode.GetValue());
			//auto pInputLayout = m_pResourceCache._Get()->RequestResource<InputLayout>(
			//	shaderParameterNode.GetValue());

			if (!pInputLayout->IsInitialized())
			{
				if (!pInputLayout->Create(pRawMemory, memorySize, pRenderingDevice))
				{
					return false;
				}
			}
		}
		shaderParameterNode.GoToNextNode();
	}

	return true;
}
void ShaderResourceLoader::CreateEmptyShader(ShaderType shaderType, 
	std::shared_ptr<ShaderBase> &pShader)
{
	switch (shaderType)
	{
	case ShaderType::VERTEX_SHADER:
		pShader = std::make_shared<VertexShader>();
		break;
	case ShaderType::HULL_SHADER:
		pShader = std::make_shared<HullShader>();
		break;
	case ShaderType::DOMAIN_SHADER:
		pShader = std::make_shared<DomainShader>();
		break;
	case ShaderType::GEOMETRY_SHADER:
		pShader = std::make_shared<GeometryShader>();
		break;
	case ShaderType::PIXEL_SHADER:
		pShader = std::make_shared<PixelShader>();
		break;
	case ShaderType::COMPUTE_SHADER:
		pShader = std::make_shared<ComputeShader>();
		break;
	default:
		EIGHT_ASSERT(false, "Unsupported shader type!");
		break;
	}
}

void ShaderResourceLoader::CreateInputLayoutsDesc()
{
	// Create input layout descriptions

	//auto pInputLayout = m_pResourceCache._Get()->RequestResource<InputLayout>(
	//	"PosTanNormTexIL");
	auto pInputLayout = g_ResourceCache.RequestResource<InputLayout>("PosTanNormTexIL");
	pInputLayout->AddInputElement("POSITION", 0, FormatFlag::FORMAT_R32G32B32_FLOAT, 0,
		0, InputClassification::INPUT_PER_VERTEX_DATA, 0);
	pInputLayout->AddInputElement("TANGENT", 0, FormatFlag::FORMAT_R32G32B32A32_FLOAT, 0,
		12, InputClassification::INPUT_PER_VERTEX_DATA, 0);
	pInputLayout->AddInputElement("NORMAL", 0, FormatFlag::FORMAT_R32G32B32_FLOAT, 0,
		28, InputClassification::INPUT_PER_VERTEX_DATA, 0);
	pInputLayout->AddInputElement("TEXCOORD", 0, FormatFlag::FORMAT_R32G32_FLOAT, 0,
		40, InputClassification::INPUT_PER_VERTEX_DATA, 0);

	//pInputLayout = m_pResourceCache._Get()->RequestResource<InputLayout>(
	pInputLayout = g_ResourceCache.RequestResource<InputLayout>("PosTexIL");
	pInputLayout->AddInputElement("POSITION", 0, FormatFlag::FORMAT_R32G32B32_FLOAT, 0,
		0, InputClassification::INPUT_PER_VERTEX_DATA, 0);
	pInputLayout->AddInputElement("TEX_COORD", 0, FormatFlag::FORMAT_R32G32_FLOAT, 0,
		12, InputClassification::INPUT_PER_VERTEX_DATA, 0);

	//pInputLayout = m_pResourceCache._Get()->RequestResource<InputLayout>(
	pInputLayout = g_ResourceCache.RequestResource<InputLayout>("PosIL");
	pInputLayout->AddInputElement("POSITION", 0, FormatFlag::FORMAT_R32G32B32_FLOAT, 0,
		0, InputClassification::INPUT_PER_VERTEX_DATA, 0);

	//pInputLayout = m_pResourceCache._Get()->RequestResource<InputLayout>(
	pInputLayout = g_ResourceCache.RequestResource<InputLayout>("PosColTexIL");
	pInputLayout->AddInputElement("POSITION", 0, FormatFlag::FORMAT_R32G32B32_FLOAT, 0,
		0, InputClassification::INPUT_PER_VERTEX_DATA, 0);
	pInputLayout->AddInputElement("COLOR", 0, FormatFlag::FORMAT_R32G32B32A32_FLOAT, 0,
		12, InputClassification::INPUT_PER_VERTEX_DATA, 0);
	pInputLayout->AddInputElement("TEX_COORD", 0, FormatFlag::FORMAT_R32G32_FLOAT, 0,
		28, InputClassification::INPUT_PER_VERTEX_DATA, 0);

	//pInputLayout = m_pResourceCache._Get()->RequestResource<InputLayout>(
	pInputLayout = g_ResourceCache.RequestResource<InputLayout>("PosColIL");
	pInputLayout->AddInputElement("POSITION", 0, FormatFlag::FORMAT_R32G32B32_FLOAT, 0,
		0, InputClassification::INPUT_PER_VERTEX_DATA, 0);
	pInputLayout->AddInputElement("COLOR", 0, FormatFlag::FORMAT_R32G32B32A32_FLOAT, 0,
		12, InputClassification::INPUT_PER_VERTEX_DATA, 0);

	pInputLayout = g_ResourceCache.RequestResource<InputLayout>("PosNormIL");
	pInputLayout->AddInputElement("POSITION", 0, FormatFlag::FORMAT_R32G32B32_FLOAT, 0,
		0, InputClassification::INPUT_PER_VERTEX_DATA, 0);
	pInputLayout->AddInputElement("NORMAL", 0, FormatFlag::FORMAT_R32G32B32_FLOAT, 0,
		12, InputClassification::INPUT_PER_VERTEX_DATA, 0);
}