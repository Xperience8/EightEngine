#pragma once

#include "EightEngine\Memory\MemoryPool.h"
#include "EightEngine\Memory\SharedPtr.h"

namespace Allocators
{
	class EventAllocator
	{
	private:
		static MemoryPool s_MemoryPool;
	public:
		// Create memory pools
		static inline bool Create()
		{
			if (!s_MemoryPool.Init(60, 12800))
			{
				EIGHT_ERROR("Initializing memory pool failed");
				return false;
			}

			EIGHT_LOG("Event allocator is successfully initialized");
			return true;
		}

		// Release memory pools and print debug memory statistics(such as usage etc.)
		static inline void Destroy()
		{
			s_MemoryPool.Destroy();
			EIGHT_LOG("Event allocator was destroyed");
		}

		static inline void *Alloc(size_t size)
		{
			EIGHT_ASSERT(size <= 60, "Requested chunk " + std::to_string(size) + " is too big");
			return s_MemoryPool.Alloc();
		}
		static inline void Dealloc(void *pEventPtr)
		{
			s_MemoryPool.Free(pEventPtr);
		}
	};
}

template <typename _EventType>
using EventPtr = SharedPtr<_EventType, Allocators::EventAllocator>;