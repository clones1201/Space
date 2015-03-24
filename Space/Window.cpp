#include "Log.h"
#include "WindowImpl.hpp"

namespace Space
{
	Window::Window(const std::string& winName, int width, int height, bool fullscreen)
		:
#if SPACE_PLATFORM == SPACE_WIN32
		impl(new Window::Win32Window(winName, width, height, fullscreen))
#endif
	{
	}

	Window::~Window()
	{
	}

	void Window::Show()
	{
		impl->Show();
	}
	
	int Window::Width() const
	{
		return impl->Width();
	}
	int Window::Height() const
	{
		return impl->Height();
	}
	bool Window::IsFullscreen() const
	{
		return impl->IsFullscreen();
	}
	
	int Window::Impl::Width() const
	{
		return _width;
	}
	int Window::Impl::Height() const
	{
		return _height;
	}
	bool Window::Impl::IsFullscreen() const
	{
		return _fullscreen;
	}
	
	LRESULT CALLBACK Window::Win32Window::MessageRouter(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		Window::Win32Window* pThis = nullptr;
		CREATESTRUCT* pStruct = nullptr;
		if (message == WM_CREATE)
		{
			pStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
			pThis = reinterpret_cast<Window::Win32Window*>(pStruct->lpCreateParams);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
		}
		else
		{
			pThis = reinterpret_cast<Window::Win32Window*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));

			if (pThis)
			{
				return pThis->_WndProc(hWnd, message, wParam, lParam);
			}
			else
			{
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		return 0;
	}

	Window::Win32Window::Win32Window(const std::string& winName,int width,int height,bool fullscreen)
		:Window::Impl(winName,width,height,fullscreen)
	{
		hInstance = (HINSTANCE)GetModuleHandle(NULL);

		// Register class
		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = MessageRouter;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)NULL);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = TEXT("RenderWindow");
		wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)NULL);
		if (!RegisterClassEx(&wcex)){
			throw new std::exception("RenderWindow RegisterClassEx Failed");
		}

		RECT rc = { 0, 0, width, height };
		//AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
		_width = rc.right - rc.left;
		_height = rc.bottom - rc.top;
		
		hWnd = CreateWindow(TEXT("RenderWindow"), TEXT("RenderWindow"), WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, _width, _height, NULL, NULL, hInstance,
			reinterpret_cast<LPVOID>(this));
		if (!hWnd){
			throw std::exception("RenderWindow CreateWindow Failed");
		}
		hDC = GetDC(hWnd);
	}
	Window* Window::Create(const std::string& winName, int width, int height, bool fullscreen) throw()
	{
		try
		{
			return new Window(winName, width, height, fullscreen);
		}
		catch (std::exception e)
		{
			Log("%s\n", e.what());
			return nullptr;
		}
	}
		
	LRESULT Window::Win32Window::_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		HDC hdc;

		switch (msg)
		{
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
		return 0;
	}

	
	HWND Window::Win32Window::GetHandle() const
	{
		return hWnd;
	}

	HDC Window::Win32Window::GetDeviceContext() const
	{
		return hDC;
	}

}