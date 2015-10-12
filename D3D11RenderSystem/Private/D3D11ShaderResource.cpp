#include "Common/Log.h"
#include "Common/Utility.hpp"
#include "RenderSystem/D3D11Device.hpp"
#include "RenderSystem/D3D11ShaderResource.hpp"
#include "RenderSystem/D3D11DeviceBuffer.hpp"
#include "RenderSystem/D3D11DeviceTexture.hpp"

namespace Space
{
	D3D11ShaderResource::D3D11ShaderResource(
		D3D11DevicePtr device, TextureBuffer* pTBuffer)
	{
		assert(nullptr != dynamic_cast<D3D11DeviceBuffer*>(pTBuffer->GetBuffer()));

		CComPtr<ID3D11Buffer> pBuffer(static_cast<D3D11DeviceBuffer*>(pTBuffer->GetBuffer())->GetRawBuffer());

		D3D11_BUFFER_DESC bufDesc;
		pBuffer->GetDesc(&bufDesc);
		m_Desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		m_Desc.Buffer.FirstElement = 0;
		m_Desc.Buffer.NumElements = pTBuffer->GetElementCount();
		m_Desc.Buffer.ElementWidth = pTBuffer->GetSizeOfElement();
		m_Desc.Buffer.ElementOffset = 0;

		ID3D11ShaderResourceView* pView;
		HRESULT hr = device->Get()->CreateShaderResourceView(pBuffer, &m_Desc, &pView);
		if (FAILED(hr))
		{
			throw std::exception("CreateShaderResourceView failed.");
		}
		m_pView = pView;
	}

	D3D11ShaderResource::D3D11ShaderResource(
		D3D11DevicePtr device, DeviceTexture1D* pTexture)
	{
		assert(nullptr != dynamic_cast<D3D11DeviceTexture1D*>(pTexture));

		CComPtr<ID3D11Texture1D> pTexture1D(static_cast<D3D11DeviceTexture1D*>(pTexture)->GetD3DTexture1D());

		D3D11_TEXTURE1D_DESC texDesc;
		pTexture1D->GetDesc(&texDesc);
		m_Desc.Format = texDesc.Format;
		m_Desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
		m_Desc.Texture1D.MipLevels = texDesc.MipLevels;
		m_Desc.Texture1D.MostDetailedMip = 0;

		ID3D11ShaderResourceView* pView;
		HRESULT hr = device->Get()->CreateShaderResourceView(pTexture1D, &m_Desc, &pView);
		if (FAILED(hr))
		{
			throw std::exception("CreateShaderResourceView failed.");
		}
		m_pView = pView;
	}

	D3D11ShaderResource::D3D11ShaderResource(D3D11DevicePtr device, DeviceTexture2D* pTexture)
	{
		assert(nullptr != dynamic_cast<D3D11DeviceTexture2D*>(pTexture));

		CComPtr<ID3D11Texture2D> pTexture2D(static_cast<D3D11DeviceTexture2D*>(pTexture)->GetD3DTexture2D());

		D3D11_TEXTURE2D_DESC texDesc;
		pTexture2D->GetDesc(&texDesc);
		m_Desc.Format = texDesc.Format;
		m_Desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		m_Desc.Texture2D.MipLevels = texDesc.MipLevels;
		m_Desc.Texture2D.MostDetailedMip = 0;

		ID3D11ShaderResourceView* pView;
		HRESULT hr = device->Get()->CreateShaderResourceView(pTexture2D, &m_Desc, &pView);
		if (FAILED(hr))
		{
			throw std::exception("CreateShaderResourceView failed.");
		}
		m_pView = pView;
	}

	D3D11ShaderResource::D3D11ShaderResource(
		D3D11DevicePtr device, DeviceTexture3D* pTexture)
	{
		assert(nullptr != dynamic_cast<D3D11DeviceTexture3D*>(pTexture));

		CComPtr<ID3D11Texture3D> pTexture3D(static_cast<D3D11DeviceTexture3D*>(pTexture)->GetD3DTexture3D());

		D3D11_TEXTURE3D_DESC texDesc;
		pTexture3D->GetDesc(&texDesc);
		m_Desc.Format = texDesc.Format;
		m_Desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
		m_Desc.Texture1D.MipLevels = texDesc.MipLevels;
		m_Desc.Texture1D.MostDetailedMip = 0;

		ID3D11ShaderResourceView* pView;
		HRESULT hr = device->Get()->CreateShaderResourceView(pTexture3D, &m_Desc, &pView);
		if (FAILED(hr))
		{
			throw std::exception("CreateShaderResourceView failed.");
		}
		m_pView = pView;
	}


	CComPtr<ID3D11ShaderResourceView> D3D11ShaderResource::GetShaderResourceView() const
	{
		return (m_pView.p);
	}

	D3D11ShaderResource* D3D11ShaderResource::Create(D3D11DevicePtr device, DeviceTexture2D* pTexture)
	{
		TRY_CATCH_LOG(return new D3D11ShaderResource(device, pTexture), return nullptr);
	}

	D3D11ShaderResource* D3D11ShaderResource::Create(D3D11DevicePtr device, TextureBuffer* pTBuffer)
	{
		TRY_CATCH_LOG(return new D3D11ShaderResource(device, pTBuffer), return nullptr);
	}

	D3D11ShaderResource::~D3D11ShaderResource()
	{}
}