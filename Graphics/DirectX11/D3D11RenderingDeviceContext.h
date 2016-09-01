#pragma once

class RenderingPipelineTable
{
private:
	const UINT EMPTY_SLOT = 0;
	std::vector<UINT> m_ShadersMap;
	
	std::vector<UINT> m_ConstantBuffersMap;
	std::vector<UINT> m_SamplerStatesMap;
	std::vector<UINT> m_ShaderResourceViewsMap;

	UINT m_RasterizerStateId;
	UINT m_BlendStateId;
	UINT m_DepthStencilStateId;
	UINT m_InputLayoutId;
	UINT m_PrimitiveTopologyId;

	UINT m_VertexBufferId;
	UINT m_IndexBufferId;
	
	bool TryBind(UINT newAddress, UINT &bindedAddress)
	{
		if (newAddress == bindedAddress)
		{
			return false;
		}
		bindedAddress = newAddress;
		return true;
	}
public:
	RenderingPipelineTable();
	~RenderingPipelineTable() { }

	bool TryBindVertexBuffer(UINT address, UINT slot = 0)
	{
		return TryBind(address, m_VertexBufferId);
	}
	bool TryBindIndexBuffer(UINT address)
	{
		return TryBind(address, m_IndexBufferId);
	}
	bool TryBindShader(UINT address, UINT slot)
	{
		return TryBind(address, m_ShadersMap[slot]);
	}
	bool TryBindShaderResourceView(UINT address, UINT slot)
	{
		return TryBind(address, m_ShaderResourceViewsMap[slot]);
	}
	bool TryBindRasterizerState(UINT address)
	{
		return TryBind(address, m_RasterizerStateId);
	}
	bool TryBindBlendState(UINT address)
	{
		return TryBind(address, m_BlendStateId);
	}
	bool TryBindDepthStencilState(UINT address)
	{
		return TryBind(address, m_DepthStencilStateId);
	}
	bool TryBindInputLayout(UINT address)
	{
		return TryBind(address, m_InputLayoutId);
	}
	bool TryBindPrimitiveTopology(UINT address)
	{
		return TryBind(address, m_PrimitiveTopologyId);
	}
	bool TryBindConstantBuffer(UINT address, UINT slot)
	{
		return TryBind(address, m_ConstantBuffersMap[slot]);
	}
	bool TryBindSamplerState(UINT address, UINT slot)
	{
		return TryBind(address, m_SamplerStatesMap[slot]);
	}
};

class D3D11RenderingDeviceContext
{
private:
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext3D;

	ID3D11Buffer *m_ConstantBufferBatchBind[15];
	UINT m_BatchSizeCB = 0;

	ID3D11SamplerState *m_SamplerStateBatchBind[15];
	UINT m_BatchSizeSS = 0;

	ID3D11ShaderResourceView *m_ShaderResourceViewBatchBind[15];
	UINT m_BatchSizeSRV = 0;
public:
	D3D11RenderingDeviceContext(Microsoft::WRL::ComPtr<ID3D11DeviceContext> &pContext3D)
		: m_pContext3D(pContext3D) { }
	~D3D11RenderingDeviceContext() { }

	void Draw(UINT vertexCount, UINT startVertexLocation)
	{
		m_pContext3D->Draw(vertexCount, startVertexLocation);
	}
	void DrawIndexed(UINT indexCount, UINT startIndexLocation, UINT baseVertexLocation)
	{
		m_pContext3D->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
	}


	Microsoft::WRL::ComPtr<ID3D11DeviceContext> &GetContext3D() { return m_pContext3D; }


	void BindConstantBufferInBatch(ID3D11Buffer *pConstantBuffer)
	{
		m_ConstantBufferBatchBind[m_BatchSizeCB++] = pConstantBuffer;
	}

	void BatchBindConstantBufferVS(UINT startSlot)
	{
		m_pContext3D->VSSetConstantBuffers(startSlot, m_BatchSizeCB, m_ConstantBufferBatchBind);
		m_BatchSizeCB = 0;
	}
	void BatchBindConstantBufferHS(UINT startSlot)
	{
		m_pContext3D->HSSetConstantBuffers(startSlot, m_BatchSizeCB, m_ConstantBufferBatchBind);
		m_BatchSizeCB = 0;
	}
	void BatchBindConstantBufferDS(UINT startSlot)
	{
		m_pContext3D->DSSetConstantBuffers(startSlot, m_BatchSizeCB, m_ConstantBufferBatchBind);
		m_BatchSizeCB = 0;
	}
	void BatchBindConstantBufferGS(UINT startSlot)
	{
		m_pContext3D->GSSetConstantBuffers(startSlot, m_BatchSizeCB, m_ConstantBufferBatchBind);
		m_BatchSizeCB = 0;
	}
	void BatchBindConstantBufferPS(UINT startSlot)
	{
		m_pContext3D->PSSetConstantBuffers(startSlot, m_BatchSizeCB, m_ConstantBufferBatchBind);
		m_BatchSizeCB = 0;
	}
	void BatchBindConstantBufferCS(UINT startSlot)
	{
		m_pContext3D->CSSetConstantBuffers(startSlot, m_BatchSizeCB, m_ConstantBufferBatchBind);
		m_BatchSizeCB = 0;
	}

	void BindSamplerStateInBatch(ID3D11SamplerState *pSamplerState)
	{
		m_SamplerStateBatchBind[m_BatchSizeSS++] = pSamplerState;
	}
	void BatchBindSamplerStateVS(UINT startSlot)
	{
		m_pContext3D->VSSetSamplers(startSlot, m_BatchSizeSS, m_SamplerStateBatchBind);
		m_BatchSizeSS = 0;
	}
	void BatchBindSamplerStateHS(UINT startSlot)
	{
		m_pContext3D->HSSetSamplers(startSlot, m_BatchSizeSS, m_SamplerStateBatchBind);
		m_BatchSizeSS = 0;
	}
	void BatchBindSamplerStateDS(UINT startSlot)
	{
		m_pContext3D->DSSetSamplers(startSlot, m_BatchSizeSS, m_SamplerStateBatchBind);
		m_BatchSizeSS = 0;
	}
	void BatchBindSamplerStateGS(UINT startSlot)
	{
		m_pContext3D->GSSetSamplers(startSlot, m_BatchSizeSS, m_SamplerStateBatchBind);
		m_BatchSizeSS = 0;
	}
	void BatchBindSamplerStatePS(UINT startSlot)
	{
		m_pContext3D->PSSetSamplers(startSlot, m_BatchSizeSS, m_SamplerStateBatchBind);
		m_BatchSizeSS = 0;
	}
	void BatchBindSamplerStateCS(UINT startSlot)
	{
		m_pContext3D->CSSetSamplers(startSlot, m_BatchSizeSS, m_SamplerStateBatchBind);
		m_BatchSizeSS = 0;
	}

	void BindShaderResourceViewInBatch(ID3D11ShaderResourceView *pShaderResourceView)
	{
		m_ShaderResourceViewBatchBind[m_BatchSizeSRV++] = pShaderResourceView;
	}
	void BatchBindShaderResourceViewVS(UINT startSlot)
	{
		m_pContext3D->VSSetShaderResources(startSlot, m_BatchSizeSRV, 
			m_ShaderResourceViewBatchBind);
		m_BatchSizeSRV = 0;
	}
	void BatchBindShaderResourceViewHS(UINT startSlot)
	{
		m_pContext3D->HSSetShaderResources(startSlot, m_BatchSizeSRV,
			m_ShaderResourceViewBatchBind);
		m_BatchSizeSRV = 0;
	}
	void BatchBindShaderResourceViewDS(UINT startSlot)
	{
		m_pContext3D->DSSetShaderResources(startSlot, m_BatchSizeSRV, 
			m_ShaderResourceViewBatchBind);
		m_BatchSizeSRV = 0;
	}
	void BatchBindShaderResourceViewGS(UINT startSlot)
	{
		m_pContext3D->GSSetShaderResources(startSlot, m_BatchSizeSRV, 
			m_ShaderResourceViewBatchBind);
		m_BatchSizeSRV = 0;
	}
	void BatchBindShaderResourceViewPS(UINT startSlot)
	{
		m_pContext3D->PSSetShaderResources(startSlot, m_BatchSizeSRV, 
			m_ShaderResourceViewBatchBind);
		m_BatchSizeSRV = 0;
	}
	void BatchBindShaderResourceViewCS(UINT startSlot)
	{
		m_pContext3D->CSSetShaderResources(startSlot, m_BatchSizeSRV, 
			m_ShaderResourceViewBatchBind);
		m_BatchSizeSRV = 0;
	}
};



