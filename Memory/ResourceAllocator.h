#pragma once

namespace Allocators
{
	class ResourceAllocator
	{
	private:

	public:

		static inline bool Create()
		{
			EIGHT_LOG("Resource allocator is successfully initialized");
			return true;
		}

		static inline void Destroy()
		{
			EIGHT_LOG("Resource allocator was destroyed");
		}

		static inline void *Alloc(size_t size)
		{
			return malloc(size);
		}
		
		static inline void Dealloc(void *pResourcePtr)
		{
			free(pResourcePtr);
		}
	};
}