#include "Log.h"
#include "Utility.h"
#include "D3D11Device.hpp"
#include "D3D11ShaderResource.hpp"

namespace Space
{
	class D3D11ShaderResourceImpl : public D3D11ShaderResource
	{
	private:
		CComPtr<ID3D11ShaderResourceView> m_pView = nullptr;

		D3D11_RESOURCE_DIMENSION m_Dim;
		D3D11_SHADER_RESOURCE_VIEW_DESC m_Desc;
	public:
		D3D11ShaderResourceImpl(D3D11Device& device, ID3D11Resource* pResource)
		{
			if (pResource == nullptr)
			{
				throw std::exception("Null Resource Pointer");
			}
			HRESULT hr = S_OK;
			pResource->GetType(&m_Dim);
			
			ID3D11ShaderResourceView* pView = nullptr;
			ZeroMemory(&m_Desc, sizeof(m_Desc));
			switch (m_Dim)
			{
			case D3D11_RESOURCE_DIMENSION_BUFFER:
			{
				CComPtr<ID3D11Buffer> pBuffer = nullptr;
				hr = pResource->QueryInterface(__uuidof(ID3D11Buffer),(void**)&pBuffer);
				if (FAILED(hr))
				{
					throw std::exception("ID3D11Resource::QueryInterface failed");
				}
				D3D11_BUFFER_DESC bufDesc;
				pBuffer->GetDesc(&bufDesc);
				//m_Desc.Format = bufDesc.
				m_Desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;

			}break;
			case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
			{
				CComPtr<ID3D11Texture1D> pTexture1D = nullptr;
				hr = pResource->QueryInterface(__uuidof(ID3D11Texture1D), (void**)&pTexture1D);
				if (FAILED(hr))
				{
					throw std::exception("ID3D11Resource::QueryInterface failed");
				}
				D3D11_TEXTURE1D_DESC texDesc;
				pTexture1D->GetDesc(&texDesc);
				m_Desc.Format = texDesc.Format;
				m_Desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
				m_Desc.Texture1D.MipLevels = texDesc.MipLevels;
				m_Desc.Texture1D.MostDetailedMip = 0;
				
				hr = device->CreateShaderResourceView(pResource, &m_Desc, &pView);
				if (FAILED(hr))
				{
					throw std::exception("CreateShaderResourceView failed.");
				}				
			}break;
			case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
			{
				CComPtr<ID3D11Texture2D> pTexture2D = nullptr;
				hr = pResource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&pTexture2D);
				if (FAILED(hr))
				{
					throw std::exception("ID3D11Resource::QueryInterface failed");
				}
				D3D11_TEXTURE2D_DESC texDesc;
				pTexture2D->GetDesc(&texDesc);
				m_Desc.Format = texDesc.Format;
				m_Desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				m_Desc.Texture1D.MipLevels = texDesc.MipLevels;
				m_Desc.Texture1D.MostDetailedMip = 0;

				hr = device->CreateShaderResourceView(pResource, &m_Desc, &pView);
				if (FAILED(hr))
				{
					throw std::exception("CreateShaderResourceView failed.");
				}
			}break;
			case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
			{
				CComPtr<ID3D11Texture3D> pTexture3D = nullptr;
				hr = pResource->QueryInterface(__uuidof(ID3D11Texture3D), (void**)&pTexture3D);
				if (FAILED(hr))
				{
					throw std::exception("ID3D11Resource::QueryInterface failed");
				}
				D3D11_TEXTURE3D_DESC texDesc;
				pTexture3D->GetDesc(&texDesc);
				m_Desc.Format = texDesc.Format;
				m_Desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
				m_Desc.Texture1D.MipLevels = texDesc.MipLevels;
				m_Desc.Texture1D.MostDetailedMip = 0;

				hr = device->CreateShaderResourceView(pResource, &m_Desc, &pView);
				if (FAILED(hr))
				{
					throw std::exception("CreateShaderResourceView failed.");
				}
			}break;
			case D3D11_RESOURCE_DIMENSION_UNKNOWN:
			default:
				throw std::exception("Wrong Resource Dimension.");
			}

			if (pView != nullptr)
			{
				m_pView = pView;
			}else{
				throw std::exception("ShaderResourceView failed");
			}
		}

		ID3D11ShaderResourceView* GetShaderResourceView() const
		{
			return (m_pView.p);
		}
	};

	D3D11ShaderResource* D3D11ShaderResource::Create(D3D11Device& device, ID3D11Resource* pResource)
	{
		try
		{
			return new D3D11ShaderResourceImpl(device, pResource);
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}

	D3D11ShaderResource::D3D11ShaderResource()
	{}

	D3D11ShaderResource::~D3D11ShaderResource()
	{} 
}