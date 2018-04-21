#include "D3D11RenderSystem/D3D11RenderTarget.hpp"

namespace Space
{
	namespace Render
	{
		D3D11RenderTarget::D3D11RenderTarget(
			D3D11Device* device, D3D11DeviceTexture2D const* pBackBuffer)
		{
			if (pBackBuffer == nullptr)
				throw std::exception("Null DeviceTexture2D pointer");

			CComPtr<ID3D11Texture2D> pBuffer = pBackBuffer->GetRawTexture2D();

			D3D11_TEXTURE2D_DESC texDesc;
			pBuffer->GetDesc(&texDesc);

			if (texDesc.ArraySize != 1)
			{
				throw std::exception("Texture2D array size incorrect.");
			}

			ZeroMemory(&m_RTVDesc, sizeof(m_RTVDesc));
			m_RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			m_RTVDesc.Texture2D.MipSlice = 0;
			m_RTVDesc.Format = texDesc.Format;

			ID3D11RenderTargetView* pRenderTargetView = nullptr;
			HRESULT hr = device->Get()->CreateRenderTargetView(pBuffer, &m_RTVDesc, &pRenderTargetView);
			if (FAILED(hr))
			{
				throw std::exception("CreateRenderTargetView failed.");
			}
			m_pRenderTargetView = pRenderTargetView;
		}

		//DepthStencilView* DepthStencilView::Create(DeviceTexture2D* pTexture)
		//{
		//	auto format = pTexture->GetFormat();
		//	switch (format)
		//	{
		//	default:
		//		return nullptr;
		//	case DataFormat::D24_UNORM_S8_UINT:
		//	case DataFormat::D32_FLOAT:
		//		break;
		//	}
		//	auto device = static_cast<D3D11Device*>(RenderSystem::GetInstance()->GetDevice());
		//	return new D3D11DepthStencilView(device, pTexture);
		//}

		D3D11DepthStencilView::D3D11DepthStencilView(
			D3D11Device* device,
			D3D11DeviceTexture2D const* pBuffer)
		{
			auto pD3DTexture = pBuffer->GetRawTexture2D();
			D3D11_TEXTURE2D_DESC texDesc;
			pD3DTexture->GetDesc(&texDesc);

			ZeroMemory(&m_Desc, sizeof(m_Desc));
			m_Desc.Format = texDesc.Format;
			m_Desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			m_Desc.Texture2D.MipSlice = 0;

			ID3D11DepthStencilView* pView = nullptr;
			HRESULT hr = device->Get()->CreateDepthStencilView(pD3DTexture, &m_Desc, &pView);
			if (FAILED(hr))
			{
				throw std::exception("ID3D11Device::CreateDepthStencilView failed.");
			}
			m_pView = pView;
		}

		D3D11DepthStencilView::~D3D11DepthStencilView()
		{}

		D3D11RenderWindow::D3D11RenderWindow(
			D3D11Device* device, 
			const std::string& name, int32 width, int32 height, bool fullscreen)
			:_device(device)
		{
			_window = std::make_unique<Win32Window>(name, width, height, fullscreen);
			DXGI_SWAP_CHAIN_DESC sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.BufferCount = 1;
			sd.BufferDesc.Width = _window->Width();
			sd.BufferDesc.Height = _window->Height();
			sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
			sd.BufferDesc.RefreshRate.Numerator = 60;
			sd.BufferDesc.RefreshRate.Denominator = 1;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.OutputWindow = _window->GetHandle();
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
			sd.Windowed = !fullscreen;

			IDXGISwapChain* pSwapChain = nullptr;
			HRESULT hr = device->GetDXGIFactory()
				->CreateSwapChain(device->Get(), &sd, &(pSwapChain));
			if (FAILED(hr)) {
				//DebugLog("RenderWindow Initialize Failed");
				throw std::exception("RenderWindow Initialize Failed");
			}
			_swapChain = pSwapChain;
		}

		D3D11RenderWindow::~D3D11RenderWindow()
		{
		}

		D3D11DeviceTexture2D* D3D11RenderWindow::GetBackBuffer() const
		{
			ID3D11Texture2D* pBackBuffer = nullptr;
			HRESULT hr = _swapChain
				->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&(pBackBuffer));
			if (FAILED(hr))
			{
				throw std::exception("Get Back Buffer Failed");
			}
			return new D3D11DeviceTexture2D(_device, pBackBuffer);
		}
		
		void D3D11RenderWindow::Present()
		{
			if (_swapChain != nullptr)
			{
				_swapChain->Present(0, 0);
			}
		}

		void D3D11RenderWindow::Resize(int32 width, int32 height)
		{
			_swapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
		}

		void D3D11RenderWindow::Show()
		{
			_window->Show();
		}
	}
}