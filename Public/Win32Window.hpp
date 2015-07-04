#ifndef __SPACE_WIN32WINDOW_HPP__
#define __SPACE_WIN32WINDOW_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"
#include "Window.hpp"

namespace Space
{
#if SPACE_PLATFORM == SPACE_WIN32
	class Win32Window : public Window
	{
	public:
		virtual void Show();

		inline HWND GetHandle() const
		{
			return m_hWnd;
		}
		inline HDC GetDeviceContext() const
		{
			return m_hDC;
		}

		~Win32Window();
	private:
		Win32Window(std::wstring const &name,
			int32 width, int32 height, bool fullscreen);

		LRESULT CALLBACK WndProc(
			HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK MessageRouter(
			HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		
		HINSTANCE m_hInstance;
		HWND m_hWnd;
		HDC m_hDC;
		
		friend class Window;
	};
	
	typedef std::shared_ptr<Win32Window> Win32WindowPtr;
#endif
}

#endif