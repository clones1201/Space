#pragma once

#include "D3D11Prerequisites.hpp"
#include "D3D11DeviceResource.hpp"
#include "Window.hpp"

namespace Space
{
	namespace Render
	{

		class SPACE_D3D11_API D3D11RenderTarget
		{
		public:
			typedef typename D3D11RenderSystem RenderSystemType;
			inline ID3D11RenderTargetView* GetRenderTargetView() const
			{
				return (m_pRenderTargetView.p);
			}

			D3D11RenderTarget(D3D11Device* device, D3D11DeviceTexture2D const* pBackBuffer);
		private:
			D3D11_RENDER_TARGET_VIEW_DESC m_RTVDesc;

			CComPtr<ID3D11RenderTargetView> m_pRenderTargetView = nullptr;
			//CComPtr<ID3D11Texture2D> m_pBackBuffer = nullptr; 
		};

		class SPACE_D3D11_API D3D11DepthStencilView : public Uncopyable
		{
		public:
			virtual ~D3D11DepthStencilView();

			inline CComPtr<ID3D11DepthStencilView> GetDepthStencilView() const
			{
				return m_pView;
			}
			D3D11DepthStencilView(D3D11Device* device,
				D3D11DeviceTexture2D const* pBuffer);
		protected:
			CComPtr<ID3D11DepthStencilView> m_pView = nullptr;
			D3D11_DEPTH_STENCIL_VIEW_DESC m_Desc;
		};
		
		class SPACE_D3D11_API D3D11RenderWindow
		{
		public:
			D3D11DeviceTexture2D* GetBackBuffer() const;

			void Present();
			void Resize(int32 width, int32 height);
			void Show();

			D3D11RenderWindow(D3D11Device* device,
				const std::string& name, int32 width, int32 height, bool fullscreen);

			~D3D11RenderWindow();
		private:
			CComPtr<IDXGISwapChain> _swapChain = nullptr;

			D3D11Device* _device = nullptr;
			std::unique_ptr<Win32Window> _window;
		};
	}
}
