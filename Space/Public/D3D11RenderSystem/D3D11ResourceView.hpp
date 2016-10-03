#ifndef __SPACE_D3D11_SHADERRESOURCE_HPP__
#define __SPACE_D3D11_SHADERRESOURCE_HPP__

#include "D3D11RenderSystem/D3D11Prerequisites.hpp"
#include "D3D11RenderSystem/D3D11Device.hpp"
#include "RenderSystem/ResourceView.hpp"

namespace Space
{
	class SPACE_D3D11_API D3D11DepthStencilView : public DepthStencilView
	{
	public:
		virtual ~D3D11DepthStencilView();

		static D3D11DepthStencilView* Create(D3D11DevicePtr device, DeviceTexture2D* pTexture);
		
		inline ID3D11DepthStencilView* GetDepthStencilView() const
		{
			return m_pView;
		}
	protected:
		D3D11DepthStencilView(D3D11DevicePtr device,
			DeviceTexture2D* pBuffer);
		
		CComPtr<ID3D11DepthStencilView> m_pView;
		D3D11_DEPTH_STENCIL_VIEW_DESC m_Desc;
	};
	typedef std::shared_ptr<D3D11DepthStencilView> D3D11DepthStencilViewPtr;
	
	class SPACE_D3D11_API D3D11ShaderResource : public ShaderResource
	{
	public:
		static D3D11ShaderResource* Create(D3D11DevicePtr device, DeviceTexture2D* pTexture);
		static D3D11ShaderResource* Create(D3D11DevicePtr device, TextureBuffer* pTBuffer);

		CComPtr<ID3D11ShaderResourceView> GetShaderResourceView() const;

		virtual ~D3D11ShaderResource();
	protected:
		D3D11ShaderResource(D3D11DevicePtr device, TextureBuffer* pTBuffer);
		D3D11ShaderResource(D3D11DevicePtr device, DeviceTexture1D* pTexture); 
		D3D11ShaderResource(D3D11DevicePtr device, DeviceTexture2D* pTexture);
		D3D11ShaderResource(D3D11DevicePtr device, DeviceTexture3D* pTexture);

	private:
		CComPtr<ID3D11ShaderResourceView> m_pView = nullptr;

		D3D11_SHADER_RESOURCE_VIEW_DESC m_Desc;
	};
}

#endif