#ifndef __SPACE_D3D11_DEPTHSTENCIL_HPP__
#define __SPACE_D3D11_DEPTHSTENCIL_HPP__

#include "RenderSystem/DepthStencilView.hpp"
#include "RenderSystem/D3D11Prerequisites.hpp"

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
}

#endif