#ifndef __SPACE_D3D9_RENDERWINDOW_H__
#define __SPACE_D3D9_RENDERWINDOW_H__

#include "D3D9Prerequisites.h"
#include "D3D9RenderSystem.h"
#include "RenderWindow.h"

namespace space{
	
	class D3D9RenderWindow : virtual public RenderWindow {
	private:
		HWND					mhWnd;
		HINSTANCE				mhInstance;
	public:
		D3D9RenderWindow(HINSTANCE hInst);
		~D3D9RenderWindow();

	public:
		virtual void Create(const String& name, uint width, uint height, bool fullScreen);
		virtual void _Initialize(bool fullScreen);

		virtual void SetFullScreen(bool fullScreen, uint width, uint height){}
		virtual bool isFullScreen(void) const{ return mIsFullScreen; }

		virtual void Resize(uint width, uint height);
		virtual void Reposition(int left, int top);

		virtual void ShutDown();
	};
}

#endif