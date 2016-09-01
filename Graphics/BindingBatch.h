#pragma once

class RenderingDeviceContext;
class RenderingPipelineTable;

template <typename Object>
class BindingBatch
{
private:
	const UINT m_StartId;
	std::vector<std::shared_ptr<Object> > m_ObjectsToBind;

	std::function<void(UINT)> m_BindingFunction;

	void GetBindingRange(UINT &startSlot, UINT &lastSlot,
		RenderingPipelineTable &renderingPipeline);
public:
	BindingBatch(UINT startId) : m_StartId(startId) { }
	~BindingBatch() { }

	void AddBindingObject(const std::shared_ptr<Object> &pObjectToBind)
	{
		m_ObjectsToBind.push_back(pObjectToBind);
	}
	void SetBindingFunction(std::function<void(UINT)> &bindingFunction)
	{
		m_BindingFunction = bindingFunction;
	}
	void Bind(const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext);

	UINT GetSize() const
	{
		return static_cast<UINT>(m_ObjectsToBind.size());
	}
};

#include "EightEngine\Graphics\ApiWrapper\RenderingDeviceContext.h"

template <typename Object>
void BindingBatch<Object>::GetBindingRange(UINT &startSlot, UINT &lastSlot,
	RenderingPipelineTable &renderingPipeline)
{
	// Find first object to bind
	startSlot = -1;
	for (UINT i = 0; i < m_ObjectsToBind.size(); i++)
	{
		if (renderingPipeline.TryBindConstantBuffer(reinterpret_cast<UINT>(
			m_ObjectsToBind[i].get()), m_StartId + i))
		{
			startSlot = i;
			break;
		}
	}

	// Find last object to bind
	lastSlot = startSlot + 1;
	for (UINT i = lastSlot; i < m_ObjectsToBind.size(); i++)
	{
		if (renderingPipeline.TryBindConstantBuffer(reinterpret_cast<UINT>(
			m_ObjectsToBind[i].get()), m_StartId + i))
		{
			lastSlot = i + 1;
		}
	}
}
template <typename Object>
void BindingBatch<Object>::Bind(
	const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
{
	if (m_ObjectsToBind.empty())
		return;

	UINT startSlot;
	UINT lastSlot;
	GetBindingRange(startSlot, lastSlot,
		pRenderingDeviceContext->GetRenderingPipelineTable());

	if (startSlot != -1)
	{
		for (UINT i = startSlot; i < lastSlot; i++)
		{
			m_ObjectsToBind[i]->BatchBind(pRenderingDeviceContext);
		}

		// Future work
		m_BindingFunction(startSlot);
	}
}

template <typename Object>
class BindingBatch2
{
private:
	const UINT m_StartId;
	std::vector<Object> m_ObjectsToBind;

	std::function<void(UINT)> m_BindingFunction;

	void GetBindingRange(UINT &startSlot, UINT &lastSlot,
		RenderingPipelineTable &renderingPipeline);
public:
	BindingBatch2(UINT startId) : m_StartId(startId) { }
	~BindingBatch2() { }

	void AddBindingObject(const Object &pObjectToBind)
	{
		m_ObjectsToBind.push_back(pObjectToBind);
	}
	void SetBindingFunction(std::function<void(UINT)> &bindingFunction)
	{
		m_BindingFunction = bindingFunction;
	}
	void Bind(const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext);
};

#include "EightEngine\Graphics\ApiWrapper\RenderingDeviceContext.h"

template <typename Object>
void BindingBatch2<Object>::GetBindingRange(UINT &startSlot, UINT &lastSlot,
	RenderingPipelineTable &renderingPipeline)
{
	// Find first object to bind
	startSlot = -1;
	for (UINT i = 0; i < m_ObjectsToBind.size(); i++)
	{
		if (renderingPipeline.TryBindConstantBuffer(reinterpret_cast<UINT>(
			m_ObjectsToBind[i].Get()), m_StartId + i))
		{
			startSlot = i;
			break;
		}
	}

	// Find last object to bind
	lastSlot = startSlot + 1;
	for (UINT i = lastSlot; i < m_ObjectsToBind.size(); i++)
	{
		if (renderingPipeline.TryBindConstantBuffer(reinterpret_cast<UINT>(
			m_ObjectsToBind[i].Get()), m_StartId + i))
		{
			lastSlot = i + 1;
		}
	}
}
template <typename Object>
void BindingBatch2<Object>::Bind(
	const std::shared_ptr<RenderingDeviceContext> &pRenderingDeviceContext)
{
	if (m_ObjectsToBind.empty())
		return;

	UINT startSlot;
	UINT lastSlot;
	GetBindingRange(startSlot, lastSlot,
		pRenderingDeviceContext->GetRenderingPipelineTable());

	if (startSlot != -1)
	{
		for (UINT i = startSlot; i < lastSlot; i++)
		{
			m_ObjectsToBind[i]->BatchBind(pRenderingDeviceContext);
		}

		// Future work
		m_BindingFunction(startSlot);
	}
}