#pragma once

#include "D3D11Prerequisites.hpp"
#include "D3D11Device.hpp"
#include "D3D11DeviceResource.hpp"

namespace Space {
	namespace Render{
		class SPACE_D3D11_API D3D11ShaderResource : public Uncopyable
		{
		public:
			friend class ShaderResource;
			inline CComPtr<ID3D11ShaderResourceView> GetShaderResourceView() const
			{
				return m_pView;
			}

			virtual ~D3D11ShaderResource();
		protected:
			D3D11ShaderResource(
				D3D11Device* device, D3D11DeviceBuffer* pTBuffer, size_t elementCount, size_t sizeOfElement);
			D3D11ShaderResource(D3D11Device* device, D3D11DeviceTexture1D* pTexture);
			D3D11ShaderResource(D3D11Device* device, D3D11DeviceTexture2D* pTexture);
			D3D11ShaderResource(D3D11Device* device, D3D11DeviceTexture3D* pTexture);

		private:
			CComPtr<ID3D11ShaderResourceView> m_pView = nullptr;
			D3D11_SHADER_RESOURCE_VIEW_DESC m_Desc;
		};
	}
}