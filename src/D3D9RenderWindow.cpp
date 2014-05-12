#include "D3D9RenderWindow.h"
#include "WindowsUtilities.h"

namespace space{
	using namespace graphic;

	D3D9RenderWindow::D3D9RenderWindow(HINSTANCE hInst) :
		mhInstance(hInst){

	}

	D3D9RenderWindow::~D3D9RenderWindow(){

	}

	void D3D9RenderWindow::Create(const String& name,uint width,uint height,bool fullScreen){
		//mhInstance = (HINSTANCE)GetModuleHandle(NULL);

		mWidth = width; mHeight = height;

		WNDCLASSEX windowClass;
		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.style = CS_CLASSDC;
		windowClass.lpfnWndProc = WindowsUtilities::_WndProc;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = mhInstance;
		windowClass.hIcon = NULL;
		windowClass.hCursor = NULL;
		windowClass.hbrBackground = NULL;
		windowClass.lpszMenuName = 0;
		windowClass.lpszClassName = name.c_str();
		windowClass.hIconSm = NULL;
		RegisterClassEx(&windowClass);
		mhWnd = CreateWindow(windowClass.lpszClassName, name.c_str(),
			WS_OVERLAPPEDWINDOW,
			0, 0, mWidth, mHeight, 0, 0, mhInstance, 0);

	}

	void D3D9RenderWindow::_Initialize(bool fullScreen){

	}
	
	void D3D9RenderWindow::Resize(uint width, uint height){

	}

	void D3D9RenderWindow::Reposition(int left, int top){

	}

	void D3D9RenderWindow::ShutDown(){

	}
}