#pragma once

#if defined(_DEBUG)

inline std::string ConvertAddressToString(UINT address)
{
#if defined(_WIN64)
	char buffer[(64 / 4) + 2]; // 4 bits to one hexa number
	sprintf_s(buffer, "%016X", address);
#elif defined(WIN32)
	char buffer[(32 / 4) + 2]; // 4 bits to one hexa number
	sprintf_s(buffer, "%08X", address);
#endif

	return std::string("0x") + std::string(buffer);
}

class MemoryManager
{
private:
	__declspec(align(16))
	struct AllocHeader
	{
		size_t Signature;
		size_t Size;
		size_t NumAllocation;
		AllocHeader *pNextAlloc;
		AllocHeader *pPrevAlloc;
	};

	const size_t SIGNATURE = 0;

	size_t m_NumAllocations;
	size_t m_NumDeallocations;
	size_t m_AllocatedMemory;
	size_t m_AllocatedMemoryPeak;

	AllocHeader *m_pHeadAllocation;
public:
	MemoryManager() : m_NumAllocations(0), m_NumDeallocations(0), m_AllocatedMemory(0),
		m_AllocatedMemoryPeak(0), m_pHeadAllocation(nullptr) { }
	~MemoryManager() { }

	void *AllocMemory(size_t size)
	{
		BYTE *pMemory = reinterpret_cast<BYTE*>(malloc(size + sizeof(AllocHeader)));
		if (!pMemory)
		{
			EIGHT_ERROR("Memory allocation failed");
			return false;
		}

		// Cache information about allocation
		++m_NumAllocations;

		// In release build won't be allocation header
		// and we want to know how much memory needs application in release build
		m_AllocatedMemory += size;

		if (m_AllocatedMemory > m_AllocatedMemoryPeak)
		{
			m_AllocatedMemoryPeak = m_AllocatedMemory;
		}

		// Fill allocation header
		AllocHeader *pAllocHeader = reinterpret_cast<AllocHeader*>(pMemory);
		pAllocHeader->Signature = SIGNATURE;
		pAllocHeader->Size = size;
		pAllocHeader->NumAllocation = m_NumAllocations;
		pAllocHeader->pPrevAlloc = m_pHeadAllocation;
		pAllocHeader->pNextAlloc = nullptr;

		if (m_pHeadAllocation != nullptr)
		{
			m_pHeadAllocation->pNextAlloc = pAllocHeader;
		}
		m_pHeadAllocation = pAllocHeader;

		// Skip allocation header
		return pMemory + sizeof(AllocHeader);
	}
	void DeallocMemory(void *pMemory)
	{
		if (pMemory)
		{
			AllocHeader *pAllocHeader = reinterpret_cast<AllocHeader*>(
				reinterpret_cast<BYTE*>(pMemory)-sizeof(AllocHeader));

			if (pAllocHeader->Signature != SIGNATURE)
			{
				free(pMemory);
				return;
			}

			if (m_pHeadAllocation == pAllocHeader)
			{
				m_pHeadAllocation = m_pHeadAllocation->pPrevAlloc;
			}

			AllocHeader *pPrevAlloc = pAllocHeader->pPrevAlloc;
			AllocHeader *pNextAlloc = pAllocHeader->pNextAlloc;

			if (pPrevAlloc)
			{
				pPrevAlloc->pNextAlloc = pNextAlloc;
			}
			if (pNextAlloc)
			{
				pNextAlloc->pPrevAlloc = pPrevAlloc;
			}

			// Cache information about deallocation
			++m_NumDeallocations;

			// In release build won't be allocation header
			// and we want to know how much memory needs application in release build
			m_AllocatedMemory -= pAllocHeader->Size;

			free(pAllocHeader);
}
	}
	void PrintMemoryLeaks()
	{
		if (m_pHeadAllocation)
		{
			for (AllocHeader *pAllocation = m_pHeadAllocation; pAllocation;
				pAllocation = pAllocation->pPrevAlloc)
			{
				std::string message = "There is memory leak";
				message += " Address: " + ConvertAddressToString(reinterpret_cast<UINT>(
					pAllocation + sizeof(AllocHeader)));
				message += " Size: " + std::to_string(pAllocation->Size);
				message += " Number of memory allocation: " +
					std::to_string(pAllocation->NumAllocation);

				EIGHT_LOG(message);
			}
		}
	}

	void PrintStatistics()
	{
		const double KB = 1024.0;
		const double MB = KB * 1024.0;
		const double GB = MB * 1024.0;

		EIGHT_LOG("\n\t\t\t**********Memory allocation statistic***************");
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

		if (m_NumAllocations != m_NumDeallocations || m_pHeadAllocation)
		{
			PrintMemoryLeaks();
		}
		else {
			EIGHT_LOG("There are not memory leaks\n");
		}
	}
};

#else
class MemoryManager
{
public:
	MemoryManager() { }
	~MemoryManager() { }

	void *AllocMemory(size_t size)
	{
		return malloc(size);
	}
	void DeallocMemory(void *pMemory)
	{
		if (pMemory)
		{
			free(pMemory);
		}
	}
	void PrintStatistics() { }
};
#endif

extern _declspec(selectany) MemoryManager s_MemoryManager;

inline void *operator new(size_t size){ return s_MemoryManager.AllocMemory(size); }
inline void *operator new[](size_t size) { return s_MemoryManager.AllocMemory(size); }
inline void operator delete(void *pMemory) { s_MemoryManager.DeallocMemory(pMemory); }
inline void operator delete[](void *pMemory) { s_MemoryManager.DeallocMemory(pMemory); }
