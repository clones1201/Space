#ifndef __SPACE_RENDERWINDOW_HPP__
#define __SPACE_RENDERWINDOW_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"

namespace Space
{
	class RenderWindow : virtual public Interface
	{
	protected:
		TypeTrait<Window>::Ptr m_pWindow;
		RenderWindow(const std::string& name, int width, int height, bool fullscreen) throw();

		std::string m_strName;
		int m_iWidth, m_iHeight;
		bool m_bFullscreen;
	public:
		virtual bool _Initialize() = 0;
		virtual RenderTarget* CreateRenderTarget() throw() = 0;
	};
}

#endif