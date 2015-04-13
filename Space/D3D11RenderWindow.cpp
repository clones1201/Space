#include "Log.h"
#include "Window.hpp"
#include "Win32Window.hpp"
#include "D3D11Device.hpp"
#include "D3D11DeviceTexture.hpp"
#include "D3D11RenderTarget.hpp"
#include "D3D11RenderWindow.hpp"

namespace Space
{
	class D3D11RenderWindowImpl : public D3D11RenderWindow
	{
	private:
		D3D11Device& mDevice;
		
		TypeTrait<DeviceTexture2D>::Ptr m_pBackBuffer = nullptr;
		CComPtr<IDXGISwapChain> m_pSwapChain = nullptr;
	public:
		D3D11RenderWindowImpl(D3D11Device& device,const std::string& name,int32 width,int32 height,bool fullscreen)
			:mDevice(device),D3D11RenderWindow(name, width, height, fullscreen) 
		{
		} 

		DeviceTexture2D* GetBackBuffer()
		{
			return m_pBackBuffer.get();
		}

		virtual bool _Initialize()
		{
			auto window = dynamic_cast<Win32Window*>(m_pWindow.get());
			if (window == nullptr)
			{
				throw std::exception("Window type error.");
			}

			DXGI_SWAP_CHAIN_DESC sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.BufferCount = 1;
			sd.BufferDesc.Width = window->Width();
			sd.BufferDesc.Height = window->Height();
			sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
			sd.BufferDesc.RefreshRate.Numerator = 60;
			sd.BufferDesc.RefreshRate.Denominator = 1;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.OutputWindow = window->GetHandle();
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
			sd.Windowed = RenderWindow::m_Fullscreen ? TRUE : FALSE;

			IDXGISwapChain* pSwapChain = nullptr;
			HRESULT hr = mDevice.GetDXGIFactory()->CreateSwapChain(mDevice.Get(), &sd, &(pSwapChain));
			if (FAILED(hr)){
				DebugLog("RenderWindow Initialize Failed");
				return false;
			}
			m_pSwapChain = pSwapChain;

			ID3D11Texture2D* pBackBuffer = nullptr;
			hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&(pBackBuffer));
			if (FAILED(hr))
			{
				Log("Get Back Buffer Failed\n");
				return false;
			}
			m_pBackBuffer.reset(D3D11DeviceTexture2D::Create(mDevice, pBackBuffer));
			return true;
		}

		virtual void Resize(int32 width, int32 height)
		{ 
			m_pSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH); 

			ID3D11Texture2D* pBackBuffer = nullptr;
			HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&(pBackBuffer));
			if (FAILED(hr))
			{
				Log("Get Back Buffer Failed\n");
				return ;
			}
			m_pBackBuffer.reset(D3D11DeviceTexture2D::Create(mDevice, pBackBuffer));
		}
	};

	D3D11RenderWindow* D3D11RenderWindow::Create(D3D11Device& mDevice, const std::string& name, int32 width, int32 height, bool fullscreen)
	{
		try
		{
			return new D3D11RenderWindowImpl(mDevice, name, width, height, fullscreen);
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}

	D3D11RenderWindow::D3D11RenderWindow(const std::string& name, int32 width, int32 height, bool fullscreen)
		:RenderWindow(name, width, height,fullscreen)
	{
	}
}
