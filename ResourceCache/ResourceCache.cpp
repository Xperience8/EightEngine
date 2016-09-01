#include "stdafx.h"
#include "ResourceCache.h"
#include "EightEngine\Memory\MemoryReader.h"

ResourceCache g_ResourceCache;

void ResourceCache::StartUp()
{
	// Basic loaders which are needed for basic functionality

	// RawDataResourceLoader is used when requested loader doesn't exist, 
	// or we want to store just plain binary(raw) data
	m_ResourceLoadersMap.emplace(RawDataResourceLoader::s_SIGNATURE,
		std::make_unique<RawDataResourceLoader>());

	// XmlResourceLoader is used for loading config files and for configuring 
	// engine we have to load config files and work with them 
	m_ResourceLoadersMap.emplace(XmlResourceLoader::s_SIGNATURE,
		std::make_unique<XmlResourceLoader>());
}
void ResourceCache::ShutDown()
{
	m_ResourceHandlersMap.clear();
	m_ResourceLoadersMap.clear();
}
bool ResourceCache::PreloadResources(const std::string &filename, bool reload)
{
	// Creating memory loader and loading file
	MemoryReader buffer;
	if (!buffer.LoadFromFile(filename))
		return false;

	// How many resources are in file
	UINT numResources = buffer.ReadMemory<UINT>();

	for (UINT i = 0; i < numResources; i++)
	{
		// Save resource name, loader signature and buffer 
		WORD nameSize = buffer.ReadMemory<WORD>();
		std::string resourceName(buffer.ReadMemoryArray<char>(nameSize), nameSize);

		UINT loaderSignature = buffer.ReadMemory<UINT>();
		UINT bufferSize = buffer.ReadMemory<UINT>();

		if (!LoadResource(buffer.ReadMemoryArray<BYTE>(bufferSize), bufferSize, 
			loaderSignature, resourceName, reload))
		{
			return false;
		}
	}
	return true;
}
bool ResourceCache::LoadResource(const std::string &filename, 
	UINT loaderSignature, bool reload)
{
	// Creating memory loader and loading file
	MemoryReader buffer;
	if (!buffer.LoadFromFile(filename))
	{
		return false;
	}

	return LoadResource(buffer.ReadMemoryArray<BYTE>(static_cast<UINT>(buffer.GetMemorySize())), 
		static_cast<UINT>(buffer.GetMemorySize()), loaderSignature, filename, reload);
}
bool ResourceCache::LoadResource(BYTE *pMemory, UINT memorySize, 
	UINT loaderSignature, const std::string &resourceName, bool reload)
{
	// If smart pointer to resource doesn't exist, 
	// it means that this resource has not been requested yet
	// If resource is not initialized,
	// it means that this resource has been allready requested
	// and now it came into memory so it is ready for initialization
	// If resource has been initialized, nothing more happens
	auto &pResource = m_ResourceHandlersMap[resourceName];
	if (!pResource || (!pResource->IsInitialized() || reload))
	{
		IResourceLoader *pLoader = nullptr;

		auto &pLoaderIt = m_ResourceLoadersMap.find(loaderSignature);
		if (pLoaderIt == m_ResourceLoadersMap.end())
		{
			EIGHT_WARNING("Requested loader doesn't exist, therefore RawDataResourceLoader is used");
			pLoader = m_ResourceLoadersMap[RawDataResourceLoader::s_SIGNATURE].get();
		}
		else {
			pLoader = pLoaderIt->second.get();
		}

		if (!pLoader->VCreateResource(pMemory, memorySize, resourceName, pResource))
		{
			EIGHT_ERROR("Creating resource: " + resourceName + " failed");
			pResource.reset();
			return false;
		}
	}
	return true;
}

bool ResourceCache::LoadResourceEditor(const std::string &resourceName)
{
	std::string extension = resourceName.substr(resourceName.find_last_of('.') + 1);
	UINT loaderSignature = RawDataResourceLoader::s_SIGNATURE;
	if (extension == "dds")
	{
		loaderSignature = TextureResourceLoader::s_SIGNATURE;
	}
	else if (extension == "xml")
	{
		loaderSignature = XmlResourceLoader::s_SIGNATURE;
	}
	else if (extension == "g3d")
	{
		loaderSignature = RenderingModelResourceLoader::s_SIGNATURE;
	}
	else if (extension == "p3d")
	{
		loaderSignature = PhysicsModelResourceLoader::s_SIGNATURE;
	}
	else if (extension == "wav")
	{
		loaderSignature = AudioLoader::s_SIGNATURE;
	}
	else if (extension == "f")
	{
		loaderSignature = FontLoader::s_SIGNATURE;
	}
	else if (extension == "cso")
	{
		loaderSignature = ShaderResourceLoader::s_SIGNATURE;
	}
	else
	{
		return false;
	}

	MemoryReader buffer;
	if (!buffer.LoadFromFile("Assets/" + resourceName))
	{
		return false;
	}

	if (!LoadResource(buffer.ReadMemoryArray<BYTE>(static_cast<UINT>(buffer.GetMemorySize())),
		static_cast<UINT>(buffer.GetMemorySize()), loaderSignature, resourceName))
	{
		return false;
	}

	return true;
}
///////////////////////////////////////////////////////////////////////////////
//							Completely rewrite								 //
///////////////////////////////////////////////////////////////////////////////




