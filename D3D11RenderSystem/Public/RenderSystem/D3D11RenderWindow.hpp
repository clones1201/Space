#ifndef __SPACE_D3D11_RENDERWINDOW_HPP__
#define __SPACE_D3D11_RENDERWINDOW_HPP__

#include "RenderSystem/D3D11Prerequisites.hpp"
#include "RenderSystem/D3D11RenderTarget.hpp"
#include "RenderSystem/D3D11Shared.hpp"

#include "RenderSystem/RenderWindow.hpp"

namespace Space
{
	class SPACE_D3D11_API D3D11RenderWindow :
		public RenderWindow
	{
	public:
		virtual DeviceTexture2D* GetBackBuffer() throw();
		virtual void Present();
		virtual void Resize(int32 width, int32 height);
		virtual bool Initialize();
		
		friend class D3D11RenderSystem;
		friend class D3D11RenderWindow;
	
		static D3D11RenderWindow* Create(D3D11DevicePtr device, const std::string& name, int32 width, int32 height, bool fullscreen) throw();
	protected:
		D3D11RenderWindow(D3D11DevicePtr device, const std::string& name, int32 width, int32 height, bool fullscreen);

	private:
		D3D11DevicePtr mDevice;

		TypeTrait<DeviceTexture2D>::Ptr m_pBackBuffer = nullptr;
		CComPtr<IDXGISwapChain> m_pSwapChain = nullptr;
	};


}

#endif