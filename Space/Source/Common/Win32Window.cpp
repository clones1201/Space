#include "Common/Log.h"
#include "Common/Utility.hpp"
#include "Common/Window.hpp"

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
		WNDCLASSEX wc =
		{
			sizeof(WNDCLASSEX), CS_CLASSDC, Win32Window::MessageRouter, 0L, 0L,
			GetModuleHandle(NULL), NULL, NULL, !fullscreen ? NULL : (HBRUSH)COLOR_WINDOW ,NULL,
			TEXT("D3D Tutorial"), NULL
		};
		RegisterClassEx(&wc);
		m_hInstance = wc.hInstance;

		DWORD wndStyle = ! fullscreen
			? WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
			: WS_EX_TOPMOST | WS_POPUP;
		int topX = !fullscreen ? 0 : 0;
		int topY = !fullscreen ? 0 : 0;
		
		if (!fullscreen) {
			RECT rc = { 0, 0, width, height };
			AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
			m_Width = rc.right - rc.left;
			m_Height = rc.bottom - rc.top;
		}
		m_hWnd = CreateWindow(wc.lpszClassName, TEXT("RenderWindow"), wndStyle
			, topX, topY, m_Width, m_Height,
			NULL, NULL, m_hInstance, this);
		
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
		case WM_MOUSEMOVE:
		{
			auto x = GET_X_LPARAM(lParam);
			auto y = GET_Y_LPARAM(lParam);	
			
			m_EventDispatcher->RaiseMouseMoveEvent(
				MouseEventArgs{
				x,y,0,
				(MK_LBUTTON == (MK_LBUTTON & wParam)),
				(MK_RBUTTON == (MK_RBUTTON & wParam)),
				(MK_MBUTTON == (MK_MBUTTON & wParam))}
				);
		}	
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
