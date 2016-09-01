#pragma once

#include "EightEngine\Graphics\ApiWrapper\Buffers\ConstantBuffer.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDeviceContext.h"
#include "BindingBatch.h"
#include "EightEngine\Graphics\ApiWrapper\Texture2D.h"
#include "EightEngine\Xml\XmlNode.h"
#include "EightEngine\Graphics\ApiWrapper\Shaders\ShaderBase.h"

class Material
{
private:
	BindingBatch<Texture2D> m_TextureBatch;
	LightMaterial m_LightMaterial;
public:
	Material() : m_TextureBatch(static_cast<UINT>(ShaderType::PIXEL_SHADER) * 15) { }
	~Material() { }

	// Link binding function with textures
	void Init(const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
	{
		// Setting binding function for textures
		std::function<void(UINT)> bindingFunction = std::bind(
			&RenderingDeviceContext::BatchBindShaderResourceViewPS,
			pRenderingDeviceContext.get(), std::placeholders::_1);

		m_TextureBatch.SetBindingFunction(bindingFunction);
	}

	// Adding new texture to the texture batch(textures are binded together in batch)
	void AddTexture(std::shared_ptr<Texture2D> &pTexture)
	{
		m_TextureBatch.AddBindingObject(pTexture);
	}

	// Setting light information about surface
	void SetLightMaterial(const LightMaterial &lightMaterial) 
	{ 
		m_LightMaterial = lightMaterial; 
	}
	
	const LightMaterial &GetLightMaterial() const
	{
		return m_LightMaterial;
	}

	// Sending material data to gpu and binding textures
	void Apply(const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
	{
		MaterialCBData cbData(m_LightMaterial);
		pRenderingDeviceContext->GetConstantBuffer(MaterialCBData::NAME)->UpdateBuffer(
			&cbData, pRenderingDeviceContext);

		m_TextureBatch.Bind(pRenderingDeviceContext);
	}

	bool HasTextures() const
	{
		return m_TextureBatch.GetSize() > 0;
	}

#if defined(EIGHT_EDITOR)
	void SaveToXml(XmlNode &node)
	{
		auto ambientNode = node.CreateChildNode("Ambient");
		ambientNode.CreateAttribute("r", std::to_string(m_LightMaterial.Ambient.x));
		ambientNode.CreateAttribute("g", std::to_string(m_LightMaterial.Ambient.y));
		ambientNode.CreateAttribute("b", std::to_string(m_LightMaterial.Ambient.z));
		ambientNode.CreateAttribute("a", std::to_string(m_LightMaterial.Ambient.w));

		auto diffuseNode = node.CreateChildNode("Diffuse");
		diffuseNode.CreateAttribute("r", std::to_string(m_LightMaterial.Diffuse.x));
		diffuseNode.CreateAttribute("g", std::to_string(m_LightMaterial.Diffuse.y));
		diffuseNode.CreateAttribute("b", std::to_string(m_LightMaterial.Diffuse.z));
		diffuseNode.CreateAttribute("a", std::to_string(m_LightMaterial.Diffuse.w));

		auto specularNode = node.CreateChildNode("Specular");
		specularNode.CreateAttribute("r", std::to_string(m_LightMaterial.Specular.x));
		specularNode.CreateAttribute("g", std::to_string(m_LightMaterial.Specular.y));
		specularNode.CreateAttribute("b", std::to_string(m_LightMaterial.Specular.z));
		specularNode.CreateAttribute("a", std::to_string(m_LightMaterial.Specular.w));

		auto emissiveNode = node.CreateChildNode("Emissive");
		emissiveNode.CreateAttribute("r", std::to_string(m_LightMaterial.Emissive.x));
		emissiveNode.CreateAttribute("g", std::to_string(m_LightMaterial.Emissive.y));
		emissiveNode.CreateAttribute("b", std::to_string(m_LightMaterial.Emissive.z));
		emissiveNode.CreateAttribute("a", std::to_string(m_LightMaterial.Emissive.w));

		auto reflectionNode = node.CreateChildNode("Reflection");
		reflectionNode.CreateAttribute("r", std::to_string(m_LightMaterial.Reflection.x));
		reflectionNode.CreateAttribute("g", std::to_string(m_LightMaterial.Reflection.y));
		reflectionNode.CreateAttribute("b", std::to_string(m_LightMaterial.Reflection.z));
		reflectionNode.CreateAttribute("a", std::to_string(m_LightMaterial.Reflection.w));

		auto transparentNode = node.CreateChildNode("Transparent");
		transparentNode.CreateAttribute("r", std::to_string(m_LightMaterial.Transparent.x));
		transparentNode.CreateAttribute("g", std::to_string(m_LightMaterial.Transparent.y));
		transparentNode.CreateAttribute("b", std::to_string(m_LightMaterial.Transparent.z));
		transparentNode.CreateAttribute("a", std::to_string(m_LightMaterial.Transparent.w));
	}
	void Modify(const XmlNode &configNode)
	{
		auto ambientNode = configNode.GetChildrenNode("Ambient");
		m_LightMaterial.Ambient.x = std::stof(ambientNode.GetAttribute("r").GetValue());
		m_LightMaterial.Ambient.y = std::stof(ambientNode.GetAttribute("g").GetValue());
		m_LightMaterial.Ambient.z = std::stof(ambientNode.GetAttribute("b").GetValue());
		m_LightMaterial.Ambient.w = std::stof(ambientNode.GetAttribute("a").GetValue());
	
		auto diffuseNode = configNode.GetChildrenNode("Diffuse");
		m_LightMaterial.Diffuse.x = std::stof(diffuseNode.GetAttribute("r").GetValue());
		m_LightMaterial.Diffuse.y = std::stof(diffuseNode.GetAttribute("g").GetValue());
		m_LightMaterial.Diffuse.z = std::stof(diffuseNode.GetAttribute("b").GetValue());
		m_LightMaterial.Diffuse.w = std::stof(diffuseNode.GetAttribute("a").GetValue());

		auto specularNode = configNode.GetChildrenNode("Specular");
		m_LightMaterial.Specular.x = std::stof(specularNode.GetAttribute("r").GetValue());
		m_LightMaterial.Specular.y = std::stof(specularNode.GetAttribute("g").GetValue());
		m_LightMaterial.Specular.z = std::stof(specularNode.GetAttribute("b").GetValue());
		m_LightMaterial.Specular.w = std::stof(specularNode.GetAttribute("a").GetValue());

		auto emissiveNode = configNode.GetChildrenNode("Emissive");
		m_LightMaterial.Emissive.x = std::stof(emissiveNode.GetAttribute("r").GetValue());
		m_LightMaterial.Emissive.y = std::stof(emissiveNode.GetAttribute("g").GetValue());
		m_LightMaterial.Emissive.z = std::stof(emissiveNode.GetAttribute("b").GetValue());
		m_LightMaterial.Emissive.w = std::stof(emissiveNode.GetAttribute("a").GetValue());

		auto reflectionNode = configNode.GetChildrenNode("Reflection");
		m_LightMaterial.Reflection.x = std::stof(reflectionNode.GetAttribute("r").GetValue());
		m_LightMaterial.Reflection.y = std::stof(reflectionNode.GetAttribute("g").GetValue());
		m_LightMaterial.Reflection.z = std::stof(reflectionNode.GetAttribute("b").GetValue());
		m_LightMaterial.Reflection.w = std::stof(reflectionNode.GetAttribute("a").GetValue());

		auto transparentNode = configNode.GetChildrenNode("Transparent");
		m_LightMaterial.Transparent.x = std::stof(transparentNode.GetAttribute("r").GetValue());
		m_LightMaterial.Transparent.y = std::stof(transparentNode.GetAttribute("g").GetValue());
		m_LightMaterial.Transparent.z = std::stof(transparentNode.GetAttribute("b").GetValue());
		m_LightMaterial.Transparent.w = std::stof(transparentNode.GetAttribute("a").GetValue());
	}
#endif

};
