#pragma once

// Purposes:
// Loading memory from file or from another memory stream
// After destruction it automatically releases memory
// Caching actual memory location for reading
// Can traverse memory i.e you can set reading location

class MemoryReader
{
public:
	MemoryReader() : m_pRawMemory(nullptr), m_pReadingLocation(nullptr),
		m_RawMemorySize(0), m_OwnMemory(false) { }
	~MemoryReader()
	{
		if (m_OwnMemory)
		{
			delete[] m_pRawMemory;
		}
	}

	// Load file and create buffer
	// Automatically release buffer if it is not empty
	// Set reading location to files start
	// If it can not open file error is send into log
	bool LoadFromFile(const std::string &filename);

	// Saving pointer to memory and memory size
	// If parameter ownMemory is true, memory is copied into internal structure 
	void LoadFromMemoryStream(BYTE *pRawMemory, size_t rawMemorySize, bool ownMemory = false);

	template <typename Type>
	Type ReadMemory()
	{
		Type *pMemory = reinterpret_cast<Type*>(m_pReadingLocation);
		m_pReadingLocation += sizeof(Type);
		return *pMemory;
	}

	template <typename Type>
	Type *ReadMemoryArray(UINT arraySize)
	{
		Type *pMemory = reinterpret_cast<Type*>(m_pReadingLocation);
		m_pReadingLocation += sizeof(Type) * arraySize;
		return pMemory;
	}

	bool IsEnd() const { return m_pReadingLocation >= m_pRawMemory + m_RawMemorySize; }
	void SetReadingLocation(UINT location)
	{
		EIGHT_ASSERT(location < m_RawMemorySize || m_pRawMemory, "You are trying to read out of memory");

		m_pReadingLocation = m_pRawMemory + location;
	}
	const size_t &GetMemorySize() const { return m_RawMemorySize; }
private:
	BYTE *m_pRawMemory;
	BYTE *m_pReadingLocation;
	size_t m_RawMemorySize;
	bool m_OwnMemory;
};