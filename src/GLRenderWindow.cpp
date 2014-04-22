#include "GLRenderSystem.h"
#include "GLRenderWindow.h"

namespace space{
	namespace graphic{

		GLRenderWindow::GLRenderWindow(HINSTANCE hInst, GLDevice &device) :
			mhInstance(hInst), mDevice(device){

		}

		GLRenderWindow::~GLRenderWindow(){

		}


		// modified from "fghFillPFD" in *freeglut*
		static void spFillPFD(PIXELFORMATDESCRIPTOR *ppfd, HDC hdc)
		{
			ppfd->nSize = sizeof(PIXELFORMATDESCRIPTOR);
			ppfd->nVersion = 1;
			ppfd->dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

			ppfd->iPixelType = PFD_TYPE_RGBA;
			ppfd->cColorBits = 24;
			ppfd->cRedBits = 8;
			ppfd->cGreenBits = 8;
			ppfd->cBlueBits = 8;
			ppfd->cAlphaBits = 8;

			ppfd->cAccumBits = 1;
			ppfd->cAccumRedBits = 0;
			ppfd->cAccumGreenBits = 0;
			ppfd->cAccumBlueBits = 0;
			ppfd->cAccumAlphaBits = 0;

			/* Hmmm, or 32/0 instead of 24/8? */
			ppfd->cDepthBits = 24;
			ppfd->cStencilBits = 8;

			ppfd->iLayerType = PFD_MAIN_PLANE;
			ppfd->bReserved = 0;
			ppfd->dwLayerMask = 0;
			ppfd->dwVisibleMask = 0;
			ppfd->dwDamageMask = 0;

			ppfd->cColorBits = (BYTE)GetDeviceCaps(hdc, BITSPIXEL);
		}

		void GLRenderWindow::CreateGLResouse(){
			mDevice.mhDC = GetDC(mhWnd);
			PIXELFORMATDESCRIPTOR pfd;
			memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
			spFillPFD(&pfd, mDevice.mhDC);

			int nPixelFormat = ChoosePixelFormat(mDevice.mhDC, &pfd);

			if (nPixelFormat == 0) return ;

			BOOL bResult = SetPixelFormat(mDevice.mhDC, nPixelFormat, &pfd);

			if (!bResult) return ;

			HGLRC tempContext = wglCreateContext(mDevice.mhDC);
			wglMakeCurrent(mDevice.mhDC, tempContext);

			GLenum err = glewInit();
			if (GLEW_OK != err){
				MessageBox(mhWnd, L"GLEW is not initialized!", L"GLEW", 0);
			}

			int attribs[] =	{
				WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
				WGL_CONTEXT_MINOR_VERSION_ARB, 1,
				WGL_CONTEXT_FLAGS_ARB, 0,
				0
			};

			if (wglewIsSupported("WGL_ARB_create_context") == 1){
				mDevice.mhGLRC = wglCreateContextAttribsARB(mDevice.mhDC, 0, attribs);
				wglMakeCurrent(NULL, NULL);
				wglDeleteContext(tempContext);
				wglMakeCurrent(mDevice.mhDC, mDevice.mhGLRC);
			}
			else{       //It's not possible to make a GL 3.x context. Use the old style context (GL 2.1 and before)
				mDevice.mhGLRC = tempContext;
			}

			if (!mDevice.mhGLRC){
				MessageBox(mhWnd, L"wglCreateContext() failed", L"Error", MB_ICONERROR | MB_OK);
				exit(1);
			}

			if (!wglMakeCurrent(mDevice.mhDC, mDevice.mhGLRC)){
				MessageBox(mhWnd, L"wglMakeCurrent() failed", L"Error", MB_ICONERROR | MB_OK);
				exit(1);
			}
			
			glViewport(0, 0, mWidth, mHeight);
			glClearColor(0, 0, 1, 1);
		}

		void GLRenderWindow::Create(const String & name, uint width, uint height, bool fullScreen){}
		
		void GLRenderWindow::_Initialize(bool fullScreen){}

		void GLRenderWindow::Resize(uint width, uint height){}

		void GLRenderWindow::Reposition(int left, int top){}
	}
}