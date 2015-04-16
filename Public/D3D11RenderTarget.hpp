#ifndef __SPACE_D3D11_RENDERTARGET_HPP__
#define __SPACE_D3D11_RENDERTARGET_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"
#include "RenderTarget.hpp"
#include "D3D11Prerequisites.hpp"
#include "D3D11RenderSystem.hpp"

namespace Space
{
	class D3D11RenderTarget : public RenderTarget
	{
	public:
		virtual ID3D11RenderTargetView* GetRenderTargetView() const throw() = 0;
		
		static D3D11RenderTarget* Create(D3D11Device& mDevice, DeviceTexture2D* pBackBuffer);
	protected:
	};

}

#endif