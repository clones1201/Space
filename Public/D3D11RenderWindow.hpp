#ifndef __SPACE_D3D11_RENDERWINDOW_HPP__
#define __SPACE_D3D11_RENDERWINDOW_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"
#include "RenderWindow.hpp"
#include "D3D11Prerequisites.hpp"
#include "D3D11RenderTarget.hpp"
#include "D3D11Shared.hpp"
#include "D3D11RenderSystem.hpp"

namespace Space
{
	class D3D11RenderWindow : 
		public RenderWindow
	{
	public:
		virtual DeviceTexture2D* GetBackBuffer() throw() = 0;
		virtual bool _Initialize() = 0;

		friend class D3D11RenderSystem;
		friend class D3D11RenderWindow;
	
		static D3D11RenderWindow* Create(D3D11Device& device, const std::string& name, int32 width, int32 height, bool fullscreen) throw();
	protected:
		D3D11RenderWindow(const std::string& name, int32 width, int32 height, bool fullscreen);
	};


}

#endif