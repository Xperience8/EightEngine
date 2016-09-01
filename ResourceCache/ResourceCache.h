#pragma once

#include "Loaders\RawDataResourceLoader.h"
#include "Loaders\XmlResourceLoader.h"
#include "Loaders\TextureResourceLoader.h"
#include "Loaders\HeightMapLoader.h"
#include "Loaders\ShaderResourceLoader.h"
#include "Loaders\RenderingModelResourceLoader.h"
#include "Loaders\PhysicsModelResourceLoader.h"
#include "Loaders\AudioLoader.h"
#include "Loaders\FontLoader.h"

#include "ResourceHandle.h"

class ResourceCache
{
public:
	ResourceCache() { }
	~ResourceCache() { }

	void StartUp();
	void ShutDown();

	// Add loader into hash table
	// Ownership of loader is taken by resource cache 
	// and resource cache is now responsible for destroying this loader
	void AddLoader(std::unique_ptr<IResourceLoader> &pLoader, UINT loaderSignature)
	{
		EIGHT_LOG("Loader with signature: " + std::to_string(loaderSignature) +
			" was added into resource cache");
		m_ResourceLoadersMap.emplace(loaderSignature, std::move(pLoader));
	}
	
	// Remove loader from hash table
	void RemoveLoader(UINT loaderSignature)
	{
		EIGHT_LOG("Loader with signature: " + std::to_string(loaderSignature) +
			" was removed from resource cache");
		m_ResourceLoadersMap.erase(loaderSignature);
	}

	// Trying to find resource in hash map
	// If Resource exist smart pointer is returned
	// If it doesn't exist, new resource is created 
	// with ResourceType type and resource is inserted into hash map
	// and smart pointer to new resource is returned,
	// new resource isn't fully initialized only default constructor is applied
	// It is initialized in LoadResource call
	template <typename ResourceType>
	std::shared_ptr<ResourceType> &RequestResource(const std::string &resourceName)
	{
		// Find resource
		auto resourceHandleIt = m_ResourceHandlersMap.find(resourceName);

		if (resourceHandleIt == m_ResourceHandlersMap.end())
		{			
#if defined(EIGHT_EDITOR)
			if (LoadResourceEditor(resourceName))
			{
				return static_cast<ResourceHandle<ResourceType> *>(
					m_ResourceHandlersMap.find(resourceName)->second.get())->GetResource();
			}
#endif
			// If it doesn't exist, create it, insert it into map and return pointer to it
			// Resource is not initialized, only placeholder is created
			auto pResourceHandle = std::make_unique<ResourceHandle<ResourceType> >(resourceName);
			auto &pResource = pResourceHandle->GetResource();
			m_ResourceHandlersMap.emplace(resourceName, std::move(pResourceHandle));
			return pResource;
		}
		else {
			// If it exists return pointer to it
			return static_cast<ResourceHandle<ResourceType> *>(
				resourceHandleIt->second.get())->GetResource();
		}
	}
private:
	std::unordered_map<UINT, std::unique_ptr<IResourceLoader> > m_ResourceLoadersMap;
	std::unordered_map<std::string, std::unique_ptr<ResourceHandleBase> > m_ResourceHandlersMap;
public:
	bool LoadResource(const std::string &filename, UINT loaderSignature, bool reload = false);
	bool LoadResource(BYTE *pMemory, UINT memorySize, UINT loaderSignature, 
		const std::string &filename, bool reload = false);
	bool PreloadResources(const std::string &filename, bool reload = false);

	bool LoadResourceEditor(const std::string &filename);

	void RemoveResource(const std::string &resourceName)
	{
		auto resourceHandleIt = m_ResourceHandlersMap.find(resourceName);

		if (resourceHandleIt != m_ResourceHandlersMap.end())
		{
			m_ResourceHandlersMap.erase(resourceHandleIt);
		}
	}

	std::string GetAssetsList() const
	{
		std::string assetsList;

		for (auto &resource : m_ResourceHandlersMap)
		{
			if (resource.second && resource.second->VIsThereOwner())
			{
				assetsList += resource.first + "$";
			}
		}
		return assetsList;
	}

	void ClearResources()
	{
		m_ResourceHandlersMap.clear();
	}
};

extern ResourceCache g_ResourceCache;