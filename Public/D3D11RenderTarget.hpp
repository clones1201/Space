#ifndef __SPACE_D3D11_RENDERTARGET_HPP__
#define __SPACE_D3D11_RENDERTARGET_HPP__

#include "Prerequisites.hpp"
#include "RenderTarget.hpp"
#include "D3D11Prerequisites.hpp"
#include "D3D11RenderSystem.hpp"

namespace Space
{
	class D3D11RenderTarget : public RenderTarget
	{
	public:
		virtual bool Activate(DepthStencilView* pDepthStencil) = 0;
		virtual void Clear(float clearColor[4]) = 0;
		virtual void Deactivate() = 0;
		
		virtual ID3D11RenderTargetView* GetRenderTargetView() const throw() = 0;
		
		static D3D11RenderTarget* Create(D3D11Device& mDevice, DeviceTexture2D* pBackBuffer);
	protected:
		D3D11RenderTarget(int32 width, int32 height);
	};

}

#endif