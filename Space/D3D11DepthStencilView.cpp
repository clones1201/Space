#include "Log.h"
#include "D3D11DepthStencilView.hpp"
#include "D3D11Device.hpp"

namespace Space
{
	class D3D11DepthStencilViewImpl : public D3D11DepthStencilView
	{
	private:
		CComPtr<ID3D11DepthStencilView> m_pView;
		D3D11_DEPTH_STENCIL_VIEW_DESC m_Desc;
	public:
		D3D11DepthStencilViewImpl(
			D3D11Device& device,
			ID3D11Texture2D* pBuffer)
		{
			D3D11_TEXTURE2D_DESC texDesc;
			pBuffer->GetDesc(&texDesc);
			
			ZeroMemory(&m_Desc, sizeof(m_Desc));
			m_Desc.Format = texDesc.Format;
			m_Desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			m_Desc.Texture2D.MipSlice = 0;
			
			ID3D11DepthStencilView* pView = nullptr;
			HRESULT hr = device->CreateDepthStencilView(pBuffer, &m_Desc, &pView);
			if (FAILED(hr))
			{
				throw std::exception("ID3D11Device::CreateDepthStencilView failed.");
			}
			m_pView = pView;
		}

		virtual ID3D11DepthStencilView* GetDepthStencilView() const
		{
			return (m_pView.p);
		}
	};


	D3D11DepthStencilView* D3D11DepthStencilView::Create(D3D11Device& device,ID3D11Texture2D* pBuffer)
	{
		try
		{
			return new D3D11DepthStencilViewImpl(device,pBuffer);
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}

	D3D11DepthStencilView::D3D11DepthStencilView()
	{

	}

}