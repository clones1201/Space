#ifndef __SPACE_WINDOWSUTILITIES_H__
#define __SPACE_WINDOWSUTILITIES_H__

#include "basic.h"
#include "Prerequisites.h"

namespace space{

	class WindowsUtilities{
	public:

#if (SPACE_PLATFORM == SPACE_PLATFORM_WINDOWS)
		static LRESULT WINAPI _WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
#endif
		static int MessagePump(void);

		static void AddRenderWindow(graphic::RenderWindow* win);
	private:
		typedef std::vector<graphic::RenderWindow*> Windows;
		static Windows windows;
	};
}

#endif