#include "Log.h"
#include "D3D11Prerequisites.hpp"
#include "D3D11Device.hpp"
#include "D3D11DeviceTexture.hpp"
#include "D3D11RenderTarget.hpp"
#include "D3D11RenderWindow.hpp"
#include "D3D11DepthStencilView.hpp"

namespace Space
{	
	class D3D11RenderTargetImpl : public D3D11RenderTarget
	{
	private:
		D3D11DevicePtr mDevice;

		D3D11_RENDER_TARGET_VIEW_DESC m_RTVDesc;

		CComPtr<ID3D11RenderTargetView> m_pRenderTargetView = nullptr;
		//CComPtr<ID3D11Texture2D> m_pBackBuffer = nullptr; 

	public:
		D3D11RenderTargetImpl(D3D11DevicePtr device, DeviceTexture2D* pBackBuffer)
			:mDevice(device)
		{
			auto pD3DDeviceTexture2D = dynamic_cast<D3D11DeviceTexture2D*>(pBackBuffer);

			if (pD3DDeviceTexture2D == nullptr)
				throw std::exception("Wrong DeviceTexture2D pointer type");

			CComPtr<ID3D11Texture2D> pBuffer = pD3DDeviceTexture2D->GetD3DTexture2D();

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
			HRESULT hr = mDevice->Get()->CreateRenderTargetView(pBuffer, &m_RTVDesc, &pRenderTargetView);
			if (FAILED(hr))
			{
				throw std::exception("CreateRenderTargetView failed.");
			}
			m_pRenderTargetView = pRenderTargetView;
		}
 
		virtual ID3D11RenderTargetView* GetRenderTargetView() const
		{
			return (m_pRenderTargetView.p);
		}

	};

	D3D11RenderTarget* D3D11RenderTarget::Create(D3D11DevicePtr device,DeviceTexture2D* pBackBuffer)
	{
		try
		{
			if (pBackBuffer == nullptr)
				throw std::exception("Null DeviceTexture2D pointer");

			return new D3D11RenderTargetImpl(device, pBackBuffer);
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}

}