#include "stdafx.h"
#include "ConstantBuffer.h"
#include "EightEngine\Graphics\ApiWrapper\RenderingDevice.h"

bool ConstantBuffer::Create(UINT bufferSize,
	const std::shared_ptr<RenderingDevice> &pRenderingDevice)
{
	EIGHT_ASSERT(bufferSize, "You are trying to create constant buffer with size 0");

	m_BufferSize = bufferSize;

	// If data are not 16 bytes aligned, align them
	UINT toAlign = bufferSize % 16;
	if (toAlign)
		bufferSize += (16 - toAlign);

	return CreateApiSpecific(bufferSize, pRenderingDevice);
}