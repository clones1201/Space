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
		D3D11Device& mDevice;

		D3D11_RENDER_TARGET_VIEW_DESC m_RTVDesc;

		CComPtr<ID3D11RenderTargetView> m_pRenderTargetView = nullptr;
		CComPtr<ID3D11Texture2D> m_pBackBuffer = nullptr; 

	public:
		D3D11RenderTargetImpl(D3D11Device& device, DeviceTexture2D* pBackBuffer)
			:D3D11RenderTarget(pBackBuffer->GetWidth(),pBackBuffer->GetHeight()),mDevice(device)
		{
			auto pD3DDeviceTexture2D = dynamic_cast<D3D11DeviceTexture2D*>(pBackBuffer);

			if (pD3DDeviceTexture2D == nullptr)
				throw std::exception("Wrong DeviceTexture2D pointer type");

			m_pBackBuffer = pD3DDeviceTexture2D->GetD3DTexture2D();

			D3D11_TEXTURE2D_DESC texDesc;
			m_pBackBuffer->GetDesc(&texDesc);
			
			if (texDesc.ArraySize != 1)
			{
				throw std::exception("Texture2D array size incorrect.");
			}
			
			ZeroMemory(&m_RTVDesc, sizeof(m_RTVDesc));
			m_RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			m_RTVDesc.Texture2D.MipSlice = 1;
			m_RTVDesc.Format = texDesc.Format;

			ID3D11RenderTargetView* pRenderTargetView = nullptr;
			HRESULT hr = mDevice->CreateRenderTargetView(m_pBackBuffer, &m_RTVDesc, &pRenderTargetView);
			if (FAILED(hr))
			{
				throw std::exception("CreateRenderTargetView failed.");
			}
			m_pRenderTargetView = pRenderTargetView;
		}

		virtual bool Activate(DepthStencilView* pDepthStencil)
		{
			ID3D11DepthStencilView* pDSV = nullptr;
			if (pDepthStencil != nullptr)
			{
				auto pD3DDS = dynamic_cast<D3D11DepthStencilView*>(pDepthStencil);
				if (pD3DDS != nullptr)
				{
					pDSV = pD3DDS->GetDepthStencilView();
				}
				else
				{
					throw std::exception("Incorrect Depth Stencil Interface");
				}

				if (pDSV == nullptr)
				{
					Log("Missing Depth Stencil");
				}
			}		

			ID3D11RenderTargetView* targets[] = 
			{
				m_pRenderTargetView
			};
			uint32 targetCount = ARRAYSIZE(targets);

			mDevice.GetImmediateContext()->OMSetRenderTargets(targetCount,targets,pDSV);
		}
		
		virtual void Clear(float clearColor[4])
		{
			if (m_pRenderTargetView != nullptr)
				mDevice.GetImmediateContext()->ClearRenderTargetView(m_pRenderTargetView, clearColor);
		}

		virtual void Deactivate()
		{
			mDevice.GetImmediateContext()->OMSetRenderTargets(0, nullptr, nullptr);
		}

		virtual ID3D11RenderTargetView* GetRenderTargetView() const
		{
			return (m_pRenderTargetView.p);
		}

	};

	D3D11RenderTarget* D3D11RenderTarget::Create(D3D11Device& device,DeviceTexture2D* pBackBuffer)
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

	D3D11RenderTarget::D3D11RenderTarget(int32 width,int32 height)
		:RenderTarget(width,height)
	{}
}