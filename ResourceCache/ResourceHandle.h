#pragma once

#include "ResourceHandleBase.h"

template <typename Resource>
class ResourceHandle : public ResourceHandleBase
{
private:
	std::shared_ptr<Resource> m_pResource;
public:
	explicit ResourceHandle(const std::string &resourceName)
		: ResourceHandleBase(resourceName), m_pResource(std::make_shared<Resource>()) { }
	~ResourceHandle() { }

	bool VIsThereOwner() const override { return m_pResource.use_count() > 1; }

	std::shared_ptr<Resource> &GetResource() { return m_pResource; }
};
#include "EightEngine\Graphics\ApiWrapper\Shaders\ShaderBase.h"

template <>
class ResourceHandle<ShaderBase> : public ResourceHandleBase
{
private:
	std::shared_ptr<ShaderBase> m_pResource;
public:
	explicit ResourceHandle(const std::string &resourceName) 
		: ResourceHandleBase(resourceName) { }
	~ResourceHandle() { }

	bool VIsThereOwner() const override { return m_pResource.use_count() > 1; }

	std::shared_ptr<ShaderBase> &GetResource() { return m_pResource; }
};