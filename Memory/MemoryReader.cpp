#include "stdafx.h"
#include "MemoryReader.h"

bool MemoryReader::LoadFromFile(const std::string &filename)
{
	if (m_pRawMemory && m_OwnMemory)
	{
		delete[] m_pRawMemory;
	}

	std::ifstream fin(filename.c_str(), std::ios_base::binary);
	if (!fin)
		return false;

	fin.seekg(0, std::ios_base::end);
	m_RawMemorySize = static_cast<size_t>(fin.tellg());
	fin.seekg(0, std::ios_base::beg);

	m_pRawMemory = new BYTE[m_RawMemorySize];
	
	fin.read(reinterpret_cast<char*>(m_pRawMemory), m_RawMemorySize);
	fin.close();

	m_pReadingLocation = m_pRawMemory;
	m_OwnMemory = true;

	return true;
}
void MemoryReader::LoadFromMemoryStream(BYTE *pRawMemory, size_t rawMemorySize, bool ownMemory)
{
	if (m_pRawMemory && m_OwnMemory)
	{
		delete[] m_pRawMemory;
	}
	EIGHT_ASSERT(pRawMemory || rawMemorySize > 0, "You are trying to read empty memory stream");
	
	if (ownMemory)
	{
		m_pRawMemory = new BYTE[rawMemorySize];
		CopyMemory(m_pRawMemory, pRawMemory, rawMemorySize);
	}
	else
	{
		m_pRawMemory = pRawMemory;
	}

	m_OwnMemory = ownMemory;
	m_RawMemorySize = rawMemorySize;
	m_pReadingLocation = m_pRawMemory;
}