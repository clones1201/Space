#include "Log.h"
#include "Utility.hpp"
#include "Window.hpp"
#include "Win32Window.hpp"

namespace Space
{
	Window::Window(const std::wstring& name, int32 width, int32 height, bool fullscreen)
		:m_Name(name), m_Width(width), m_Height(height), m_Fullscreen(fullscreen)
	{
	}

	Window::~Window()
	{
	}
	 
	int32 Window::Width() const
	{
		return m_Width;
	}
	int32 Window::Height() const
	{
		return m_Height;
	}
	bool Window::IsFullscreen() const
	{
		return m_Fullscreen;
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