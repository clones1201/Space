#ifndef __SPACE_WINDOWSUTILITIES_H__
#define __SPACE_WINDOWSUTILITIES_H__

#include "basic.h"

namespace space{

	class WindowsUtilities{
	public:

#if (SPACE_PLATFORM == SPACE_PLATFORM_WINDOWS)
		static LRESULT WINAPI _WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
#endif
		static void Loop(HINSTANCE hInst,HWND hWnd){
			ShowWindow(hWnd, SW_SHOWDEFAULT);
			UpdateWindow(hWnd);

			MSG msg;
			ZeroMemory(&msg, sizeof(msg));
			while (msg.message != WM_QUIT){
				if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)){
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else{
				}
			}
		}


		static int MessagePump(void);
	
	};
}

#endif