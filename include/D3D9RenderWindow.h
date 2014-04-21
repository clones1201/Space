#ifndef __WINDOWCONTROLLER_H__
#define __WINDOWCONTROLLER_H__

#include "RenderSystem.h"
#include "RenderWindow.h"

namespace space{
	
	class D3D9RenderWindow : virtual public IRenderWindow {
	private:
		HWND					mhWnd;
		HINSTANCE				mhInstance;
	public:
		D3D9RenderWindow(HINSTANCE hInst);
		~D3D9RenderWindow();

	public:
		virtual void Create(const string& name, uint width, uint height, bool fullScreen);
		virtual void _Initialize(bool fullScreen);

		virtual void SetFullScreen(bool fullScreen, uint width, uint height){}
		virtual bool isFullScreen(void) const{ return mIsFullScreen; }

		virtual void Resize(uint width, uint height);
		virtual void Reposition(int left, int top);
	};
}

#endif