#include "Common/Log.h"
#include "Common/Utility.hpp"

#include "Common/Core.hpp"
#include "Common/Window.hpp"
#include "Common/Win32Window.hpp"

namespace Space
{
	Window::Window(const std::wstring& name, int32 width, int32 height, bool fullscreen)
		:m_Name(name), m_Width(width), m_Height(height), m_Fullscreen(fullscreen)
	{
		m_EventDispatcher = Core::GetInstance()->GetEventDispatcher();
	}

	Window::~Window()
	{
	}

	Window* Window::Create(const std::string& name, int32 width, int32 height, bool fullscreen)
	{
		return Create(str2wstr(name), width, height, fullscreen);
	}
	Window* Window::Create(const std::wstring& name, int32 width, int32 height, bool fullscreen)
	{
		try
		{
#if SPACE_PLATFORM == SPACE_WIN32
			return new Win32Window(name, width, height, fullscreen);
#else
			return nullptr;
#endif
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}


}