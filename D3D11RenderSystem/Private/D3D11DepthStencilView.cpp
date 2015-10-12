#include "RenderSystem/D3D11Shared.hpp"
#include "RenderSystem/D3D11Device.hpp"
#include "RenderSystem/D3D11DeviceTexture.hpp"
#include "RenderSystem/D3D11DepthStencilView.hpp"

namespace Space
{
	D3D11DepthStencilView::D3D11DepthStencilView(
		D3D11DevicePtr device,
		DeviceTexture2D* pBuffer)
	{
		auto pD3DBuffer = dynamic_cast<D3D11DeviceTexture2D*>(pBuffer);
		if (pD3DBuffer == nullptr)
			throw std::exception("Wrong D3D11DeviceTexture2D Interface.");

		D3D11_TEXTURE2D_DESC texDesc;
		pD3DBuffer->GetD3DTexture2D()->GetDesc(&texDesc);

		ZeroMemory(&m_Desc, sizeof(m_Desc));
		m_Desc.Format = texDesc.Format;
		m_Desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		m_Desc.Texture2D.MipSlice = 0;

		ID3D11DepthStencilView* pView = nullptr;
		HRESULT hr = device->Get()->CreateDepthStencilView(pD3DBuffer->GetD3DTexture2D(), &m_Desc, &pView);
		if (FAILED(hr))
		{
			throw std::exception("ID3D11Device::CreateDepthStencilView failed.");
		}
		m_pView = pView;
	}

	D3D11DepthStencilView* D3D11DepthStencilView::Create(D3D11DevicePtr device, DeviceTexture2D* pBuffer)
	{
		try
		{
			if (pBuffer == nullptr)
				throw std::exception("Null DeviceTexture2D pointer");

			return new D3D11DepthStencilView(device, pBuffer);
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}
	
	D3D11DepthStencilView::~D3D11DepthStencilView()
	{}

}