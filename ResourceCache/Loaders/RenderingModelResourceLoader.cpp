#include "stdafx.h"
#include "RenderingModelResourceLoader.h"
#include "EightEngine\ResourceCache\ResourceHandle.h"
#include "EightEngine\ResourceCache\ResourceCache.h"
#include "EightEngine\Graphics\Model.h"
#include "EightEngine\Graphics\ApiWrapper\Renderer.h"
#include "EightEngine\Memory\MemoryReader.h"

typedef ResourceHandle<Model> RenderingModelResourceHandle;

///////////////////////////////////////////////////////////////////////////////
//							Completely rewrite								 //
///////////////////////////////////////////////////////////////////////////////
enum TextureType
{
	DIFFUSE_MAP,
	BUMP_MAP,
	AMBIENT_MAP,
	SPECULAR_MAP,
	HEIGHT_MAP,
	EMISSIVE_MAP,
	REFLECTION_MAP,
};

const UINT IDENTIFICATION_CODE = 'G' * '3' * 'D';
const UINT ACTUAL_VERSION = 0x03;

struct RenderingFileHeader
{
	UINT IdentificationCode;
	UINT Version;
	DirectX::BoundingBox FrustumCullingVolume;
	UINT NumMeshes;
	UINT NumMaterials;
	UINT NumSubsets;
};
struct MeshHeader
{
	UINT VertexCount;
	UINT VertexBufferStride;
	UINT IndexCount;
	UINT IndexBufferStride;
};

bool RenderingModelResourceLoader::VCreateResource(BYTE *pRawMemory, UINT MemorySize,
	const std::string &resourceName, std::unique_ptr<ResourceHandleBase> &pResourceHandle)
{
	MemoryReader reader;
	reader.LoadFromMemoryStream(pRawMemory, MemorySize);

	// If file has newer version, or it is in different format
	auto pFileHeader = reader.ReadMemoryArray<RenderingFileHeader>(1);
	if (pFileHeader->IdentificationCode != IDENTIFICATION_CODE ||
		pFileHeader->Version != ACTUAL_VERSION)
	{
#if defined(_DEBUG)
		if (pFileHeader->IdentificationCode != IDENTIFICATION_CODE)
		{
			EIGHT_WARNING("Model resource: " + resourceName + " , there is a file with bad identification code hence it is file with different format");
		}
		else if (pFileHeader->Version < ACTUAL_VERSION)
		{
			EIGHT_WARNING("Model resource: " + resourceName + " has old version. Please update it on the newest version " +
				std::to_string(ACTUAL_VERSION));
		}
		else if (pFileHeader->Version > ACTUAL_VERSION)
		{
			EIGHT_WARNING("Model resource: " + resourceName + " has newer version, please update model resource loader on version" +
				std::to_string(pFileHeader->Version));
		}
#endif
		return false;
	}

	// If resource handle was not requested, create it
	if (!pResourceHandle)
	{
		pResourceHandle = std::make_unique<RenderingModelResourceHandle>(resourceName);
	}
	auto pModel = static_cast<RenderingModelResourceHandle*>(pResourceHandle.get())->GetResource();
	

	pModel->m_FrustumCullingVolume = pFileHeader->FrustumCullingVolume;

	pModel->m_Meshes.resize(pFileHeader->NumMeshes);
	for (UINT i = 0; i < pFileHeader->NumMeshes; i++)
	{
		if (!LoadMesh(pModel->m_Meshes[i], reader))
		{
			EIGHT_WARNING("Can not load mesh with id: " + std::to_string(i));
			return false;
		}
	}

	pModel->m_Materials.resize(pFileHeader->NumMaterials);
	for (UINT i = 0; i < pFileHeader->NumMaterials; i++)
	{
		LoadMaterial(pModel->m_Materials[i], reader);
	}

	pModel->m_Subsets.resize(pFileHeader->NumSubsets);
	UINT memorySize = pFileHeader->NumSubsets * sizeof(SubsetTable);
	CopyMemory(pModel->m_Subsets.data(), reader.ReadMemoryArray<BYTE>(memorySize), memorySize);

	EIGHT_LOG("Rendering model: " + resourceName + " is loaded and initialized");

	pResourceHandle->PostInit();

	return true;
}
bool RenderingModelResourceLoader::LoadMesh(Mesh &renderingMesh, MemoryReader &memoryReader)
{
	auto pMeshHeader = memoryReader.ReadMemoryArray<MeshHeader>(1);

	MeshDesc meshDesc;
	meshDesc.VertexBufferStride = pMeshHeader->VertexBufferStride;
	meshDesc.VertexCount = pMeshHeader->VertexCount;
	meshDesc.IndexBufferStride = pMeshHeader->IndexBufferStride;
	meshDesc.IndexCount = pMeshHeader->IndexCount;

	meshDesc.pVerticesMemory = memoryReader.ReadMemoryArray<BYTE>(
		meshDesc.VertexBufferStride * meshDesc.VertexCount);
	meshDesc.pIndicesMemory = memoryReader.ReadMemoryArray<BYTE>(
		meshDesc.IndexBufferStride * meshDesc.IndexCount);

	return renderingMesh.Init(meshDesc, g_Renderer.GetRenderingDevice());
}
void RenderingModelResourceLoader::LoadMaterial(Material &renderingMaterial,
	MemoryReader &memoryReader)
{
	auto pLightMaterial = memoryReader.ReadMemoryArray<LightMaterial>(1);
	renderingMaterial.SetLightMaterial(*pLightMaterial);

	UINT texturesCount = memoryReader.ReadMemory<UINT>();
	for (UINT j = 0; j < texturesCount; j++)
	{
		// Request texture with specific name
		// TextureType is actually not used
		USHORT textureNameSize = memoryReader.ReadMemory<USHORT>();
		std::string textureName(memoryReader.ReadMemoryArray<char>(
			textureNameSize), textureNameSize);
		TextureType textureType = memoryReader.ReadMemory<TextureType>();

		renderingMaterial.AddTexture(g_ResourceCache.RequestResource<Texture2D>(
			textureName));
	}

	renderingMaterial.Init(g_Renderer.GetRenderingDeviceContext());
}

///////////////////////////////////////////////////////////////////////////////
//							Completely rewrite								 //
///////////////////////////////////////////////////////////////////////////////
