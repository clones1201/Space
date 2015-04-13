#include "Log.h"
#include "Utility.h"
#include "Window.hpp"
#include "Win32Window.hpp"

namespace Space
{

	Win32Window::Win32Window(std::wstring const& name, int32 width, int32 height, bool fullscreen)
		:Window(name, width, height, fullscreen)
	{
		m_hInstance = (HINSTANCE)GetModuleHandle(NULL);

		// Register class
		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = MessageRouter;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = m_hInstance;
		wcex.hIcon = LoadIcon(m_hInstance, (LPCTSTR)NULL);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = TEXT("RenderWindow");
		wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)NULL);
		if (!RegisterClassEx(&wcex)){
			throw new std::exception("RenderWindow RegisterClassEx Failed");
		}

		RECT rc = { 0, 0, width, height };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
		m_Width = rc.right - rc.left;
		m_Height = rc.bottom - rc.top;

		m_hWnd = CreateWindowW(L"RenderWindow", L"RenderWindow", WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, m_Width, m_Height, NULL, NULL, m_hInstance,
			reinterpret_cast<LPVOID>(this));
		if (!m_hWnd){
			throw std::exception("RenderWindow CreateWindow Failed");
		}
		m_hDC = GetDC(m_hWnd);
	}

	Win32Window::~Win32Window()
	{

	}

	LRESULT CALLBACK Win32Window::MessageRouter(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		Win32Window* pThis = nullptr;
		CREATESTRUCT* pStruct = nullptr;
		if (message == WM_CREATE)
		{
			pStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
			pThis = reinterpret_cast<Win32Window*>(pStruct->lpCreateParams);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
		}
		else
		{
			pThis = reinterpret_cast<Win32Window*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));

			if (pThis)
			{
				return pThis->WndProc(hWnd, message, wParam, lParam);
			}
			else
			{
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		return 0;
	}

	HWND Win32Window::GetHandle() const
	{
		return m_hWnd;
	}
	HDC Win32Window::GetDeviceContext() const
	{
		return m_hDC;
	}

	LRESULT Win32Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		HDC hdc;

		switch (message)
		{
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}

	void Win32Window::Show()
	{
		::ShowWindow(m_hWnd, true);
	}

}
