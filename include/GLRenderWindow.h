#ifndef __SPACE_GL_RENDERWINDOW_H__
#define __SPACE_GL_RENDERWINDOW_H__

#include "GLPrerequisites.h"
#include "GLRenderSystem.h"
#include "RenderWindow.h"

namespace space{
	namespace graphic{		

		class GLRenderWindow : virtual public 
			RenderWindow{
		public:
			GLRenderWindow(HINSTANCE hInst,GLDevice& device);
			~GLRenderWindow();

			virtual void Create(const String & name, uint width, uint height, bool fullScreen) = 0;
			virtual void _Initialize(bool fullScreen) = 0;

			virtual void SetFullScreen(bool fullScreen, uint width, uint height){}
			
			virtual void Resize(uint width, uint height) = 0;
			virtual void Reposition(int left, int top) = 0;
			
			virtual void ShutDown();
		private:
			HINSTANCE		mhInstance;
			HWND			mhWnd;

			GLDevice&		mDevice;

			void CreateGLResouse();
		};

	}
}

#endif