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

		virtual ID3D11ShaderResourceView* GetShaderResourceView() const = 0;

		virtual ~D3D11ShaderResource();
	protected:
		D3D11ShaderResource();
	};
}

#endif