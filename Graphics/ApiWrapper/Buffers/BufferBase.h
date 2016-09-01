#pragma once

#include "EightEngine\Graphics\ApiWrapper\GraphicsFlags.h"

class BufferBase
{
public:
	// This is used for setting buffer properties like cpu access an mode in which
	// you want to use buffer(like only gpu access etc)
	// Default settings are only reading for gpu and no cpu access
	// Parameters:
	// usageFlag -> usage flag from UsageFlag enum
	// cpuAccessFlag -> cpu access flag from CpuAccessFlag menu
	void SetProperties(UsageFlag usageFlag, CpuAccessFlag cpuAccessFlag)
	{
		m_UsageFlag = usageFlag;
		m_CpuAccessFlag = cpuAccessFlag;
	}
protected:
	UsageFlag m_UsageFlag;
	CpuAccessFlag m_CpuAccessFlag;

	BufferBase() : m_UsageFlag(UsageFlag::USAGE_IMMUTABLE),
		m_CpuAccessFlag(CpuAccessFlag::NO_CPU_ACCESS) { }
	~BufferBase() { }
};