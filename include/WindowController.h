#ifndef __WINDOWCONTROLLER_H__
#define __WINDOWCONTROLLER_H__

#include "RenderSystem.h"

namespace space{
	using namespace graphic;
	//==========================
	// Callback Function
	//==========================
	typedef void(*CallbackFunc)();

	enum _CallbackFuncType{
		DisplayFunc, Total_CallbackFunc_Count
	};
	
	LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	class WindowController{
	private:
		WNDCLASSEX				windowClass;
		HWND					hWnd;
		HINSTANCE				hInstance;
		CallbackFunc			Callback[Total_CallbackFunc_Count];
		RenderSystem*			renderSystem;

		class _MouseController{
			DEFINE_IS_SET(LeftButtonDown);
			DEFINE_IS_SET(RightButtonDown);
			DEFINE_GET_SET(int,MouseXPos);
			DEFINE_GET_SET(int,MouseYPos);
		}MouseController;

		class _KeyboardController{

		}KeyboardController;

		void SwapBuffer(){
			renderSystem->SwapBuffer();
		}
	public:
		WindowController(RenderSystemType type);

		void SetDisplayFunc(void(*callback)(void)){
			Callback[DisplayFunc] = callback;
		}

		void Loop(){
			ShowWindow(hWnd, SW_SHOWDEFAULT);
			UpdateWindow(hWnd);

			MSG msg;
			ZeroMemory(&msg, sizeof(msg));
			while (msg.message != WM_QUIT)
			{
				if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)){
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else{
					Callback[DisplayFunc]();
					SwapBuffer();
				}
			}
			UnregisterClass(L"Space", hInstance);
		}

		RenderSystem* GetRenderSystem(){
			return renderSystem;
		}

		bool HandleWndMessage(HWND _hWnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& result);
		
		HWND GetWndHandler(){	return hWnd;	}
	};
	WindowController* GetWindowController();

	void SetWindowController(WindowController* windowController);
}

#endif