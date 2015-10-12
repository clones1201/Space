#ifndef __SPACE_D3D11_RENDERTARGET_HPP__
#define __SPACE_D3D11_RENDERTARGET_HPP__

#include "RenderSystem/RenderTarget.hpp"
#include "RenderSystem/D3D11Prerequisites.hpp"
#include "RenderSystem/D3D11RenderSystem.hpp"

namespace Space
{
	class SPACE_D3D11_API D3D11RenderTarget : public RenderTarget
	{
	public:
		inline ID3D11RenderTargetView* GetRenderTargetView() const
		{
			return (m_pRenderTargetView.p);
		}
		
		static D3D11RenderTarget* Create(D3D11DevicePtr device, DeviceTexture2D* pBackBuffer);
	protected:
		D3D11RenderTarget(D3D11DevicePtr device, DeviceTexture2D* pBackBuffer);

		D3D11DevicePtr mDevice;

		D3D11_RENDER_TARGET_VIEW_DESC m_RTVDesc;

		CComPtr<ID3D11RenderTargetView> m_pRenderTargetView = nullptr;
		//CComPtr<ID3D11Texture2D> m_pBackBuffer = nullptr; 
	};

}

#endif