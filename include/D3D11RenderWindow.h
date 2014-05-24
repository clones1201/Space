#ifndef __SPACE_D3D11_RENDERWINDOW_H__
#define __SPACE_D3D11_RENDERWINDOW_H__

#include "D3D11Prerequisites.h"
#include "RenderWindow.h"

namespace space{
	namespace graphic{	

		class D3D11RenderWindow : public virtual RenderWindow{

		public:
			D3D11RenderWindow(HINSTANCE,D3D11Device &);
			~D3D11RenderWindow();

			virtual void Create(const String& name, uint width, uint height, bool fullScreen);
			virtual void _Initialize(bool fullScreen);

			virtual void SetFullScreen(bool fullScreen, uint width, uint height);
			virtual bool isFullScreen(void) const{ return mIsFullScreen; }

			virtual void Resize(uint width, uint height);
			virtual void Reposition(int left, int top);

			virtual void ShutDown();

			virtual void ClearRenderTargetView();
		private:
			HINSTANCE mhInstance;
			HWND mhWnd;
			D3D11Device &mDevice; //same one within D3D11RenderSystem

			DXGI_SWAP_CHAIN_DESC md3dpp;
			D3D11_TEXTURE2D_DESC descDepth;

			IDXGISwapChain*				mpSwapChain;
			ID3D11RenderTargetView*		mpRenderTargetView;
			ID3D11DepthStencilView*		mpDepthStencilView;
			ID3D11Texture2D*			mpDepthStencil;
			//IDXGIFactory*				mpDXGIFactory;

			bool isSwapChain;

			void CreateD3DResourse();
		};
	}
}

#endif