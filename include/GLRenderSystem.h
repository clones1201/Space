#ifndef __SPACE_GL_RENDERSYSTEM_H__
#define __SPACE_GL_RENDERSYSTEM_H__

#include "GLPrerequisites.h"
#include "RenderSystem.h"

namespace space{
	namespace graphic{

		class GLDevice{
		public:
			HDC						mhDC;
			HGLRC					mhGLRC;
			HPALETTE				mhPalette;
		};

		class GLRenderSystem : public IRenderSystem {
		public:
			GLRenderSystem();
			~GLRenderSystem();

			virtual IRenderWindow* _createRenderWindow(const string &name, unsigned int width, unsigned int height,
				bool fullScreen) = 0;

			//virtual void AttachRenderTarget(IRenderTarget &rt) = 0;
			//virtual void DetachRenderTarget(const string &name) = 0;

			virtual void InitRenderSystem();

			virtual IRenderWindow* _Initialize(bool autoCreateWindow){ IRenderWindow* autoWin = nullptr; return autoWin; }

			virtual void _Render() = 0;

			virtual void _BeginScene() = 0;

			virtual void _EndScene() = 0;

			virtual void ShutDown(void) = 0;

			virtual void SetAmbientLight(float r, float g, float b) = 0;
		private:

		};
	}
}

#endif