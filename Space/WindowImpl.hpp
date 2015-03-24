#ifndef __SPACE_WINDOW_IMPL_HPP__
#define __SPACE_WINDOW_IMPL_HPP__

#include "Window.hpp"

namespace Space
{	
	class Window::Impl
	{
	protected:
		std::string name;
		int _width, _height;
		bool _fullscreen;
	public:
		Impl(const std::string& name, int width, int height, bool fullscreen);
		virtual ~Impl();

		virtual void Show();
		virtual int Width() const;
		virtual int Height() const;
		virtual bool IsFullscreen() const;

	};

#if SPACE_PLATFORM == SPACE_WIN32
	class Window::Win32Window : public Window::Impl
	{
	private:
		HINSTANCE hInstance;
		HWND hWnd;
		HDC hDC;

	public:
		Win32Window(const std::string& winName, int width, int height, bool fullscreen);
		~Win32Window();
		static LRESULT CALLBACK MessageRouter(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		HWND GetHandle() const;
		HDC GetDeviceContext() const;

		LRESULT _WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		virtual void Show();
		virtual int Width() const;
		virtual int Height() const;
		virtual bool IsFullscreen() const;
	};
#endif

}

#endif