#include "WindowController.h"

#include "RayTracing.h"

namespace space{
	using namespace graphic;
	using namespace graphic::raytrace;

	WindowController* windowController = 0;

	WindowController* GetWindowController(){
		return windowController;
	}

	void SetWindowController(WindowController* _windowController){
		windowController = _windowController;
	}

	LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
		WindowController* controller = GetWindowController();
		if ( controller ){
			LRESULT result;
			if ( controller->HandleWndMessage(hWnd,msg, wParam, lParam, result)){
				return result;
			}
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	WindowController::WindowController(RenderSystemType type,uint width,uint height){
		hInstance = (HINSTANCE)GetModuleHandle(NULL);
		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.style = type == RS_D3D ? CS_CLASSDC : CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc = MsgProc;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = hInstance;
		windowClass.hIcon = type == RS_D3D ? NULL : LoadIcon(0, IDI_APPLICATION);
		windowClass.hCursor = type == RS_D3D ? NULL : LoadCursor(0, IDC_ARROW);
		windowClass.hbrBackground = type == RS_D3D ? NULL : (HBRUSH)GetStockObject(BLACK_BRUSH);
		windowClass.lpszMenuName = 0;
		windowClass.lpszClassName = L"Space";
		windowClass.hIconSm = NULL;
		RegisterClassEx(&windowClass);
		hWnd = CreateWindowW(windowClass.lpszClassName, L"Space",
			WS_OVERLAPPEDWINDOW,
			0, 0, width, height, 0, 0, hInstance, 0);

		switch (type){
		case RS_D3D:
			//renderSystem = RenderSystem_ptr(new RenderSystemDirect3D9(hWnd,width,height));
			renderSystem = IRenderSystem_ptr(new D3D11RenderSystem(hWnd, width, height));
			break;
		case RS_OGL:
			renderSystem = IRenderSystem_ptr(new GLRenderSystem(hWnd, width, height));
			break;
		case RS_RT:
			renderSystem = IRenderSystem_ptr(new RenderSystemRayTrace(hWnd, width, height));
			break;
		}
	}
	void WindowController::SetWindowsTitle(const wstring& title){
		
		SetWindowText(hWnd, title.c_str());
	}

	bool WindowController::HandleWndMessage(HWND _hWnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& result){
		switch (msg)
		{
		case WM_CREATE:
			return true;
			break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			MouseController.SetMouseXPos(LOWORD(lParam));
			MouseController.SetMouseYPos(HIWORD(lParam));
			switch (msg){
			case WM_LBUTTONDOWN:
				MouseController.SetLeftButtonDownTrue();
				break;
			case WM_LBUTTONUP:
				MouseController.SetLeftButtonDownFalse();
				break;
			case WM_RBUTTONDOWN:
				MouseController.SetRightButtonDownTrue();
				break;
			case WM_RBUTTONUP:
				MouseController.SetRightButtonDownFalse();
				break;
			}
			break;

		case WM_SIZE:
			renderSystem->Resize(LOWORD(lParam),HIWORD(lParam));
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		default:
			result = DefWindowProc(hWnd, msg, wParam, lParam);
		}
		return true;
	}
}