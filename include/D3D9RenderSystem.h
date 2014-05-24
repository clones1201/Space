#ifndef __SPACE_D3D9_RENDERSYSTEM_H__
#define __SPACE_D3D9_RENDERSYSTEM_H__

#include "RenderSystem.h"
#include "D3D9Prerequisites.h"

namespace space{
	namespace graphic{

		class D3D9Device{
		private:
			IDirect3DDevice9* pD3Ddevice;
		public:
			D3D9Device(IDirect3DDevice9* device);
			D3D9Device();
			~D3D9Device();

			inline IDirect3DDevice9* operator->(){
				return pD3Ddevice;
			}

			bool isNull() const;

			IDirect3DDevice9* Get(){
				return pD3Ddevice;
			}
		};

		class D3D9RenderSystem : virtual public RenderSystem{
		public:
			D3D9RenderSystem(HINSTANCE hInst);
			~D3D9RenderSystem();

			virtual RenderWindow* _createRenderWindow(const string &name, unsigned int width, unsigned int height,
				bool fullScreen);

			virtual void InitRenderSystem();
			//virtual void AttachRenderTarget(IRenderTarget &rt) = 0;
			//virtual void DetachRenderTarget(const string &name) = 0;
			virtual RenderWindow* _Initialize(bool autoCreateWindow);

			virtual void _Render();

			virtual void _BeginScene();

			virtual void _EndScene();

			virtual void ShutDown(void);

			virtual void SetAmbientLight(float r, float g, float b);
			
		private:
			HINSTANCE		mhInstance;
			HWND			mhWnd;
			D3D9Device		mDevice;
			IDirect3D9*		pd3d;
		};

	}
}

#endif