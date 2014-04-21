#include "D3D11RenderSystem.h"
#include "D3D11RenderWindow.h"


namespace space{
	namespace graphic{
	
		D3D11RenderWindow::D3D11RenderWindow(HINSTANCE inst, D3D11Device & device):
			mhInstance(inst),
			mhWnd(nullptr),
			mDevice(device){
			mIsFullScreen = false;
		}

		D3D11RenderWindow::~D3D11RenderWindow(){
			
		}

		void D3D11RenderWindow::Create(const string&name, uint width, uint height, bool fullScreen){

		}

		void D3D11RenderWindow::_Initialize(bool fullScreen){

			HRESULT hr = S_OK;

			UINT createDeviceFlags = 0;
#ifdef _DEBUG
			createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

			D3D_DRIVER_TYPE driverTypes[] =
			{
				D3D_DRIVER_TYPE_HARDWARE,
				D3D_DRIVER_TYPE_WARP,
				D3D_DRIVER_TYPE_REFERENCE,
			};
			UINT numDriverTypes = ARRAYSIZE(driverTypes);

			D3D_FEATURE_LEVEL featureLevels[] =
			{
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0,
			};
			UINT numFeatureLevels = ARRAYSIZE(featureLevels);

			ZeroMemory(&md3dpp, sizeof(md3dpp));
			md3dpp.BufferCount = 1;
			md3dpp.BufferDesc.Width = IRenderTarget::mWidth;
			md3dpp.BufferDesc.Height = IRenderTarget::mHeight;
			md3dpp.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			md3dpp.BufferDesc.RefreshRate.Numerator = 60;
			md3dpp.BufferDesc.RefreshRate.Denominator = 1;
			md3dpp.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			md3dpp.OutputWindow = mhWnd;
			md3dpp.SampleDesc.Count = 1;
			md3dpp.SampleDesc.Quality = 0;
			md3dpp.Windowed = !fullScreen;

			D3D_FEATURE_LEVEL FeatureLevels = D3D_FEATURE_LEVEL_11_0;

			for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
			{
				if (SUCCEEDED(hr))
					break;
			}
			if (FAILED(hr))
				return;

			hr = mDevice->pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&mpBackBuffer);
			if (FAILED(hr))
				return;

			hr = mDevice->CreateRenderTargetView(mpBackBuffer, NULL, &mRenderTargetView);
			mpBackBuffer->Release();
			if (FAILED(hr))
				return;

			// Create depth stencil texture
			ZeroMemory(&descDepth, sizeof(descDepth));
			descDepth.Width = mWidth;
			descDepth.Height = mHeight;
			descDepth.MipLevels = 1;
			descDepth.ArraySize = 1;
			descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			descDepth.SampleDesc.Count = 1;
			descDepth.SampleDesc.Quality = 0;
			descDepth.Usage = D3D11_USAGE_DEFAULT;
			descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			descDepth.CPUAccessFlags = 0;
			descDepth.MiscFlags = 0;
			hr = mDevice->CreateTexture2D(&descDepth, NULL, &d3d->pDepthStencil);
			if (FAILED(hr))
				return;

			// Create the depth stencil view
			D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
			ZeroMemory(&descDSV, sizeof(descDSV));
			descDSV.Format = descDepth.Format;
			descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			descDSV.Texture2D.MipSlice = 0;
			hr = d3d->pd3dDevice->CreateDepthStencilView(d3d->pDepthStencil, &descDSV, &d3d->pDepthStencilView);
			if (FAILED(hr))
				return;

			d3d->pImmediateContext->OMSetRenderTargets(1, &d3d->pRenderTargetView, d3d->pDepthStencilView);

			// Setup the viewport
			D3D11_VIEWPORT vp;
			vp.Width = (FLOAT)mWidth;
			vp.Height = (FLOAT)mHeight;
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			vp.TopLeftX = 0;
			vp.TopLeftY = 0;
			d3d->pImmediateContext->RSSetViewports(1, &vp);
		}


		
		
	}
}