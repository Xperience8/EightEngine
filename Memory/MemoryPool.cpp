#include "stdafx.h"
#include "MemoryPool.h"

bool MemoryPool::Init(UINT chunkSize, UINT numChunks)
{
	m_NumChunks = numChunks;
	m_ChunkSize = chunkSize;

	UINT blockSize = m_ChunkSize + CHUNK_HEADER;
	UINT memoryArraySize = m_NumChunks * blockSize;

	m_pRawMemory = reinterpret_cast<BYTE*>(malloc(memoryArraySize));
	if (!m_pRawMemory)
	{
		EIGHT_ERROR("Allocating memory pool failed");
		return false;
	}

	Flush();

	return true;
}
void MemoryPool::Destroy()
{
	if (m_pRawMemory)
	{
		free(m_pRawMemory);
		m_pRawMemory = nullptr;
		m_pHead = nullptr;
		m_NumChunks = 0;
		m_ChunkSize = 0;

#if defined(_DEBUG)
		m_NumAllocations = 0;
		m_NumDeallocations = 0;
		m_AllocatedMemory = 0;
		m_AllocatedMemoryPeak = 0;
		
		PrintStatistics();
#endif
	}
}
void MemoryPool::Flush()
{
	UINT blockSize = m_ChunkSize + CHUNK_HEADER;
	UINT memoryArraySize = m_NumChunks * blockSize;

	BYTE *pCurrChunk = m_pRawMemory;
	BYTE *pLastChunk = m_pRawMemory + memoryArraySize - blockSize;
	while (pCurrChunk < pLastChunk)
	{
		BYTE *pNextChunk = pCurrChunk + blockSize;
		reinterpret_cast<BYTE**>(pCurrChunk)[0] = pNextChunk;
		pCurrChunk += blockSize;
	}
	reinterpret_cast<BYTE**>(pLastChunk)[0] = nullptr;

	m_pHead = m_pRawMemory;
}