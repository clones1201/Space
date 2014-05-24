#include "WindowsUtilities.h"
#include "RenderWindow.h"

namespace space{
	using namespace graphic;

	WindowsUtilities::Windows WindowsUtilities::windows;

	int WindowsUtilities::MessagePump(){
#if (SPACE_PLATFORM == SPACE_PLATFORM_WINDOWS)
		MSG msg;
		ZeroMemory(&msg, sizeof(msg));
		while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
#endif
		return 0;
	}

	void WindowsUtilities::AddRenderWindow(RenderWindow* win){
		windows.push_back(win);
	}

}