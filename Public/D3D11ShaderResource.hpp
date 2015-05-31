#ifndef __SPACE_D3D11_SHADERRESOURCE_HPP__
#define __SPACE_D3D11_SHADERRESOURCE_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"
#include "D3D11Prerequisites.hpp"
#include "ShaderResource.hpp"

namespace Space
{
	class D3D11ShaderResource : public ShaderResource
	{
	public:
		static D3D11ShaderResource* Create(D3D11Device& device, DeviceTexture2D* pTexture);
		static D3D11ShaderResource* Create(D3D11Device& device, TextureBuffer* pTBuffer);

		CComPtr<ID3D11ShaderResourceView> GetShaderResourceView() const;

		virtual ~D3D11ShaderResource();
	protected:
		D3D11ShaderResource(D3D11Device& device, TextureBuffer* pTBuffer);
		D3D11ShaderResource(D3D11Device& device, DeviceTexture1D* pTexture); 
		D3D11ShaderResource(D3D11Device& device, DeviceTexture2D* pTexture);
		D3D11ShaderResource(D3D11Device& device, DeviceTexture3D* pTexture);

	private:
		CComPtr<ID3D11ShaderResourceView> m_pView = nullptr;

		D3D11_SHADER_RESOURCE_VIEW_DESC m_Desc;
	};
}

#endif