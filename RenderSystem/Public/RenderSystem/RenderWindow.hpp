#ifndef __SPACE_RENDERSYSTEM_RENDERWINDOW_HPP__
#define __SPACE_RENDERSYSTEM_RENDERWINDOW_HPP__

#include "RenderSystem/Prerequisites.hpp"
#include "RenderSystem/Shared.hpp"

namespace Space
{
	class SPACE_RENDERSYSTEM_API RenderWindow : private Interface
	{
	public:
		virtual bool Initialize() = 0;

		virtual DeviceTexture2D* GetBackBuffer() throw() = 0;
		virtual void Present() = 0;
	protected:
		TypeTrait<Window>::Ptr m_pWindow;
		RenderWindow(const std::string& name, int32 width, int32 height, bool fullscreen) throw();

		std::string m_Name;
		int32 m_Width, m_Height;
		bool m_Fullscreen;
	};
	typedef std::shared_ptr<RenderWindow> RenderWindowPtr;
}

#endif