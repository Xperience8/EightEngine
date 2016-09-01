#pragma once

class MemoryPool
{
#if defined(_DEBUG)
private:
	char *m_PoolName;
	size_t m_NumAllocations;
	size_t m_NumDeallocations;
	size_t m_AllocatedMemory;
	size_t m_AllocatedMemoryPeak;
#endif
private:
	const UINT CHUNK_HEADER = sizeof(BYTE*);

	UINT m_NumChunks;
	UINT m_ChunkSize;
	
	BYTE *m_pRawMemory;
	BYTE *m_pHead;
public:
	explicit MemoryPool(char *poolName) : m_pRawMemory(nullptr), m_pHead(nullptr), 
		m_NumChunks(0), m_ChunkSize(0) 
	{ 
#if defined(_DEBUG)
		m_PoolName = poolName;
		m_NumAllocations = 0;
		m_NumDeallocations = 0;
		m_AllocatedMemory = 0;
		m_AllocatedMemoryPeak = 0;
#endif
	}
	~MemoryPool() { }

	bool Init(UINT chunkSize, UINT numChunks);
	void Destroy();

	void *Alloc()
	{
#if defined(_DEBUG)
		if (!m_pHead)
		{
			EIGHT_ERROR("Memory pool is out of memory");
			return nullptr;
		}

		++m_NumAllocations;
		m_AllocatedMemory += (m_ChunkSize + CHUNK_HEADER);
		if (m_AllocatedMemory > m_AllocatedMemoryPeak)
		{
			m_AllocatedMemoryPeak = m_AllocatedMemory;
		}
#endif
		
		BYTE *pChunk = m_pHead;
		m_pHead = reinterpret_cast<BYTE**>(pChunk)[0];
		return pChunk + CHUNK_HEADER;
	}
	
	void Free(void *pMemory)
	{
#if defined(_DEBUG)
		if (!pMemory)
		{
			EIGHT_ERROR("You are trying to release empty chunk");
			return;
		}

		++m_NumDeallocations;
		m_AllocatedMemory -= (m_ChunkSize + CHUNK_HEADER);
#endif

		BYTE *pChunk = reinterpret_cast<BYTE*>(pMemory) - CHUNK_HEADER;
		reinterpret_cast<BYTE**>(pChunk)[0] = m_pHead;
		m_pHead = pChunk;
	}

	void Flush();

#if defined(_DEBUG)
	void PrintStatistics()
	{
		const double KB = 1024.0;
		const double MB = KB * 1024.0;
		const double GB = MB * 1024.0;

		EIGHT_LOG("\n\t\t\t**********Memory allocation statistic for \"" +
			std::string(m_PoolName) + "\" memory pool***************");
		EIGHT_LOG("Pool size: " + std::to_string((m_ChunkSize + CHUNK_HEADER) * m_NumChunks));
		EIGHT_LOG("Chunk size: " + std::to_string(m_ChunkSize + CHUNK_HEADER));
		EIGHT_LOG("Allocations count: " + std::to_string(m_NumAllocations));
		EIGHT_LOG("Deallocations count: " + std::to_string(m_NumDeallocations));

		if (m_AllocatedMemoryPeak > GB)
		{
			EIGHT_LOG("Maximum allocated memory: " +
				std::to_string(static_cast<double>(m_AllocatedMemoryPeak) / GB) + "GB");
		}
		else if (m_AllocatedMemoryPeak > MB)
		{
			EIGHT_LOG("Maximum allocated memory: " +
				std::to_string(static_cast<double>(m_AllocatedMemoryPeak) / MB) + "MB");
		}
		else if (m_AllocatedMemoryPeak > KB)
		{
			EIGHT_LOG("Maximum allocated memory: " +
				std::to_string(static_cast<double>(m_AllocatedMemoryPeak) / KB) + "KB");
		}
		else
		{
			EIGHT_LOG("Maximum allocated memory: " +
				std::to_string(m_AllocatedMemoryPeak) + "B");
		}

		if (m_NumAllocations != m_NumDeallocations)
		{
			EIGHT_ERROR("There are memory leaks\n");
		}
		else {
			EIGHT_LOG("There are not memory leaks\n");
		}
	}
#endif
};