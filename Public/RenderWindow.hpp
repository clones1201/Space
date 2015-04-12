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
		RenderWindow(const std::string& name, int32 width, int32 height, bool fullscreen) throw();

		std::string m_strName;
		int32 m_iWidth, m_iHeight;
		bool m_bFullscreen;
	public:
		virtual bool _Initialize() = 0;

		virtual DeviceTexture2D* GetBackBuffer() throw() = 0; 
	};
}

#endif