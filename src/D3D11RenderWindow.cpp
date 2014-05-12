#include "D3D11RenderSystem.h"
#include "D3D11RenderWindow.h"
#include "WindowsUtilities.h"

namespace space{
	namespace graphic{
	
		D3D11RenderWindow::D3D11RenderWindow(HINSTANCE inst, D3D11Device & device):
			mhInstance(inst),
			mhWnd(nullptr),
			mDevice(device),
			mpSwapChain(nullptr),
			mpRenderTargetView(nullptr),
			mpDepthStencil(nullptr),
			mpDepthStencilView(nullptr){

			mIsFullScreen = false;
			isSwapChain = false;

		}

		D3D11RenderWindow::~D3D11RenderWindow(){
			
		}

		void D3D11RenderWindow::Create(const String & name, uint width, uint height, bool fullScreen){
			mIsFullScreen = fullScreen;
			RenderTarget::name = name;
			mWidth = width; mHeight = height;

			WNDCLASSEX windowClass;
			windowClass.cbSize = sizeof(WNDCLASSEX);
			windowClass.style = CS_CLASSDC;
			windowClass.lpfnWndProc = WindowsUtilities::_WndProc;
			windowClass.cbClsExtra = 0;
			windowClass.cbWndExtra = 0;
			windowClass.hInstance = mhInstance;
			windowClass.hIcon = LoadIcon(0, IDI_APPLICATION);
			windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
			windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
			windowClass.lpszMenuName = 0;
			windowClass.lpszClassName = name.c_str();
			windowClass.hIconSm = NULL;
			RegisterClassEx(&windowClass);
			mhWnd = CreateWindow(windowClass.lpszClassName, name.c_str(),
				WS_OVERLAPPEDWINDOW,
				0, 0, mWidth, mHeight, nullptr, nullptr, mhInstance, this);
			
			RECT rc; 
			GetWindowRect(mhWnd, &rc);
			mTop = rc.top;
			mLeft = rc.left;
			// width and height represent interior drawable area
			GetClientRect(mhWnd, &rc);
			mWidth = rc.right;
			mHeight = rc.bottom;

			CreateD3DResourse();

		}

		void D3D11RenderWindow::CreateD3DResourse(){

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
			md3dpp.BufferDesc.Width = mWidth;
			md3dpp.BufferDesc.Height = mHeight;
			md3dpp.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			md3dpp.BufferDesc.RefreshRate.Numerator = 60;
			md3dpp.BufferDesc.RefreshRate.Denominator = 1;
			md3dpp.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			md3dpp.OutputWindow = mhWnd;
			md3dpp.SampleDesc.Count = 1;
			md3dpp.SampleDesc.Quality = 0;
			md3dpp.Windowed = !mIsFullScreen;

			D3D_FEATURE_LEVEL FeatureLevels = D3D_FEATURE_LEVEL_11_0;
			if (isSwapChain){
				for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
				{
					IDXGIDevice * pDXGIDevice;
					hr = mDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice);
					if (SUCCEEDED(hr))
						break;
					IDXGIAdapter * pDXGIAdapter;
					hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter);
					if (SUCCEEDED(hr))
						break;
					IDXGIFactory * pIDXGIFactory;
					hr = pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&pIDXGIFactory);
					if (SUCCEEDED(hr))
						break;

					hr = pIDXGIFactory->CreateSwapChain(mDevice.Get(), &md3dpp, &mpSwapChain);
					if (SUCCEEDED(hr))
						break;
				}
				if (FAILED(hr))
					return;
			}
			ID3D11Texture2D *mBackBuffer;
			if (isSwapChain){
				hr = mpSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&mBackBuffer);
				if (FAILED(hr))
					return;

				hr = mDevice->CreateRenderTargetView(mBackBuffer, NULL, &mpRenderTargetView);
				mBackBuffer->Release();
				if (FAILED(hr))
					return;
			}
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
			hr = mDevice->CreateTexture2D(&descDepth, NULL, &mpDepthStencil);
			if (FAILED(hr))
				return;

			// Create the depth stencil view
			D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
			ZeroMemory(&descDSV, sizeof(descDSV));
			descDSV.Format = descDepth.Format;
			descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			descDSV.Texture2D.MipSlice = 0;
			hr = mDevice->CreateDepthStencilView(mpDepthStencil, &descDSV, &mpDepthStencilView);
			if (FAILED(hr))
				return;

			mDevice.GetImmediateContext()->OMSetRenderTargets(1, &mpRenderTargetView, mpDepthStencilView);

			// Setup the viewport
			D3D11_VIEWPORT vp;
			vp.Width = (FLOAT)mWidth;
			vp.Height = (FLOAT)mHeight;
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			vp.TopLeftX = 0;
			vp.TopLeftY = 0;
			mDevice.GetImmediateContext()->RSSetViewports(1, &vp);
		}

		void D3D11RenderWindow::_Initialize(bool fullScreen){
		}

		void D3D11RenderWindow::SetFullScreen(bool fullScreen, uint width, uint height){
		}
		
		void D3D11RenderWindow::Resize(uint width, uint height){
		}
		void D3D11RenderWindow::Reposition(int left, int top){
		}
		
		void D3D11RenderWindow::ShutDown(){
			UnregisterClass(NULL, mhInstance);
		}
	}
}