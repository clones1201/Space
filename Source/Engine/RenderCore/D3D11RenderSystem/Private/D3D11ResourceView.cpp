#include "D3D11Shared.hpp"
#include "D3D11Device.hpp"
#include "D3D11DeviceResource.hpp"
#include "D3D11ResourceView.hpp"

namespace Space
{
namespace Render 
{
		
	D3D11ShaderResource::D3D11ShaderResource(
		D3D11Device* device, 
		D3D11DeviceBuffer* pTBuffer, 
		size_t elementCount, size_t sizeOfElement)
	{
		D3D11_BUFFER_DESC bufDesc;
		pTBuffer->GetRawPtr()->GetDesc(&bufDesc);
		m_Desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		m_Desc.Buffer.FirstElement = 0;
		m_Desc.Buffer.NumElements = (UINT)elementCount;
		m_Desc.Buffer.ElementWidth = (UINT)sizeOfElement;
		m_Desc.Buffer.ElementOffset = 0;

		ID3D11ShaderResourceView* pView;
		HRESULT hr = device->Get()->CreateShaderResourceView(pTBuffer->GetRawPtr(), &m_Desc, &pView);
		if (FAILED(hr))
		{
			throw std::exception("CreateShaderResourceView failed.");
		}
		m_pView = pView;
	}

	D3D11ShaderResource::D3D11ShaderResource(
		D3D11Device* device, 
		D3D11DeviceTexture1D* pTexture)
	{
		D3D11_TEXTURE1D_DESC texDesc;
		pTexture->GetD3DTexture1D()->GetDesc(&texDesc);
		m_Desc.Format = texDesc.Format;
		m_Desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
		m_Desc.Texture1D.MipLevels = texDesc.MipLevels;
		m_Desc.Texture1D.MostDetailedMip = 0;

		ID3D11ShaderResourceView* pView;
		HRESULT hr = device->Get()->CreateShaderResourceView(pTexture->GetD3DTexture1D(), &m_Desc, &pView);
		if (FAILED(hr))
		{
			throw std::exception("CreateShaderResourceView failed.");
		}
		m_pView = pView;
	}

	D3D11ShaderResource::D3D11ShaderResource(
		D3D11Device* device, 
		D3D11DeviceTexture2D* pTexture)
	{
		auto pD3DTexture = pTexture->GetRawTexture2D();
		D3D11_TEXTURE2D_DESC texDesc;
		pD3DTexture->GetDesc(&texDesc);
		m_Desc.Format = texDesc.Format;
		m_Desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		m_Desc.Texture2D.MipLevels = texDesc.MipLevels;
		m_Desc.Texture2D.MostDetailedMip = 0;

		ID3D11ShaderResourceView* pView;
		HRESULT hr = device->Get()->CreateShaderResourceView(pD3DTexture, &m_Desc, &pView);
		if (FAILED(hr))
		{
			throw std::exception("CreateShaderResourceView failed.");
		}
		m_pView = pView;
	}

	D3D11ShaderResource::D3D11ShaderResource(
		D3D11Device* device, 
		D3D11DeviceTexture3D* pTexture)
	{
		D3D11_TEXTURE3D_DESC texDesc;
		pTexture->GetD3DTexture3D()->GetDesc(&texDesc);
		m_Desc.Format = texDesc.Format;
		m_Desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
		m_Desc.Texture1D.MipLevels = texDesc.MipLevels;
		m_Desc.Texture1D.MostDetailedMip = 0;

		ID3D11ShaderResourceView* pView;
		HRESULT hr = device->Get()->CreateShaderResourceView(pTexture->GetD3DTexture3D(), &m_Desc, &pView);
		if (FAILED(hr))
		{
			throw std::exception("CreateShaderResourceView failed.");
		}
		m_pView = pView;
	}
	
	D3D11ShaderResource::~D3D11ShaderResource()
	{}

}
}