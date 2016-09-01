#include "stdafx.h"
#include "EightEngine\Graphics\DirectX11\D3D11BackBuffer.h"
#include "EightEngine\Xml\XmlNode.h"

bool D3D11BackBuffer::Resize(UINT resX, UINT resY,
	const std::shared_ptr<RenderingDevice> &pRenderingDevice)
{
	ThrowIfFailed(m_pSwapChain->ResizeBuffers(1, resX, resY, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

	return m_RenderTarget.Create(m_pSwapChain, pRenderingDevice);
}

bool D3D11BackBuffer::Init(const XmlNode &configNode, HWND windowHandle,
	const std::shared_ptr<RenderingDevice> &pRenderingDevice)
{
	if (!CreateSwapChain(configNode, windowHandle, pRenderingDevice))
	{
		EIGHT_ERROR("Creating swap chain failed");
		return false;
	}

	if (!m_RenderTarget.Create(m_pSwapChain, pRenderingDevice))
	{
		EIGHT_ERROR("Creating default back buffer failed");
		return false;
	}

	return true;
}

bool D3D11BackBuffer::CreateSwapChain(const XmlNode &configNode, HWND windowHandle,
	const std::shared_ptr<RenderingDevice> &pRenderingDevice)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.Width = 0;
	swapChainDesc.BufferDesc.Height = 0;

	auto refreshRateNode = configNode.GetChildrenNode("RefreshRate");

	swapChainDesc.BufferDesc.RefreshRate.Numerator = std::stoi(refreshRateNode.GetAttribute("numerator").GetValue());
	swapChainDesc.BufferDesc.RefreshRate.Denominator = std::stoi(refreshRateNode.GetAttribute("denominator").GetValue());
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapChainDesc.OutputWindow = windowHandle;

	auto antialiasingNode = configNode.GetChildrenNode("Antialliasing");

	swapChainDesc.SampleDesc.Count = std::stoi(antialiasingNode.GetAttribute("count").GetValue());
	swapChainDesc.SampleDesc.Quality = std::stoi(antialiasingNode.GetAttribute("quality").GetValue());

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	auto fullscreenNode = configNode.GetChildrenNode("Fullscreen");
	swapChainDesc.Windowed = !std::stoi(fullscreenNode.GetAttribute().GetValue());

	auto vsyncNode = configNode.GetChildrenNode("VSYNC");
	m_VsyncLevel = std::stoi(vsyncNode.GetAttribute().GetValue());

	Microsoft::WRL::ComPtr<IDXGIDevice> pDevice;
	ThrowIfFailed(pRenderingDevice->GetDevice3D().As(&pDevice));
	Microsoft::WRL::ComPtr<IDXGIAdapter> pAdapter;
	ThrowIfFailed(pDevice->GetAdapter(&pAdapter));
	Microsoft::WRL::ComPtr<IDXGIFactory> pFactory;
	ThrowIfFailed(pAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(pFactory.GetAddressOf())));

	ThrowIfFailed(pFactory->CreateSwapChain(pRenderingDevice->GetDevice3D().Get(), &swapChainDesc, &m_pSwapChain));

	return true;
}