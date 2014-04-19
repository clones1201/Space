#include "RenderSystem.h"
#include "WindowController.h"

#include<mmsystem.h>

#include <D3DX11.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )

namespace space{
	namespace graphic{

		class D3D11RenderSystem::Direct3DDevice{
		public:
			D3D_DRIVER_TYPE         driverType = D3D_DRIVER_TYPE_NULL;
			D3D_FEATURE_LEVEL       featureLevel = D3D_FEATURE_LEVEL_11_0;
			ID3D11Device*           pd3dDevice = NULL;
			ID3D11DeviceContext*    pImmediateContext = NULL;
			ID3D11RenderTargetView* pRenderTargetView = NULL;
			IDXGISwapChain*         pSwapChain = NULL;
			ID3D11Texture2D*        pDepthStencil = NULL;
			ID3D11DepthStencilView* pDepthStencilView = NULL;
		};
		
		D3D11RenderSystem::D3D11RenderSystem(HWND hWnd, uint width, uint height):RenderSystem(width,height){
			d3d = new Direct3DDevice;
			camera = PerspectiveCamera_ptr(new PerspectiveCamera); 
			Init(hWnd, width, height);
		}

		D3D11RenderSystem::~D3D11RenderSystem(){
			if (d3d){ 
				if (d3d->pd3dDevice)
					d3d->pd3dDevice->Release();
				if (d3d->pSwapChain)
					d3d->pSwapChain->Release();
				if (d3d->pImmediateContext)
					d3d->pImmediateContext->Release();
				delete d3d; 
			}
		}

		void D3D11RenderSystem::Init(HWND hWnd, uint width, uint height){
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
			
			DXGI_SWAP_CHAIN_DESC sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.BufferCount = 1;
			sd.BufferDesc.Width = width;
			sd.BufferDesc.Height = height;
			sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			sd.BufferDesc.RefreshRate.Numerator = 60;
			sd.BufferDesc.RefreshRate.Denominator = 1;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.OutputWindow = hWnd;
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
			sd.Windowed = TRUE;

			D3D_FEATURE_LEVEL FeatureLevels = D3D_FEATURE_LEVEL_11_0;

			for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
			{
				d3d->driverType = driverTypes[driverTypeIndex];
				hr = D3D11CreateDeviceAndSwapChain(NULL, d3d->driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
					D3D11_SDK_VERSION, &sd, &d3d->pSwapChain, &d3d->pd3dDevice, &FeatureLevels, &d3d->pImmediateContext);
				if (SUCCEEDED(hr))
					break;
			}
			if (FAILED(hr))
				return ;

			// Create a render target view
			ID3D11Texture2D* pBackBuffer = NULL;
			hr = d3d->pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			if (FAILED(hr))
				return ;

			hr = d3d->pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &d3d->pRenderTargetView);
			pBackBuffer->Release();
			if (FAILED(hr))
				return ;

			// Create depth stencil texture
			D3D11_TEXTURE2D_DESC descDepth;
			ZeroMemory(&descDepth, sizeof(descDepth));
			descDepth.Width = width;
			descDepth.Height = height;
			descDepth.MipLevels = 1;
			descDepth.ArraySize = 1;
			descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			descDepth.SampleDesc.Count = 1;
			descDepth.SampleDesc.Quality = 0;
			descDepth.Usage = D3D11_USAGE_DEFAULT;
			descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			descDepth.CPUAccessFlags = 0;
			descDepth.MiscFlags = 0;
			hr = d3d->pd3dDevice->CreateTexture2D(&descDepth, NULL, &d3d->pDepthStencil);
			if (FAILED(hr))
				return ;

			// Create the depth stencil view
			D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
			ZeroMemory(&descDSV, sizeof(descDSV));
			descDSV.Format = descDepth.Format;
			descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			descDSV.Texture2D.MipSlice = 0;
			hr = d3d->pd3dDevice->CreateDepthStencilView(d3d->pDepthStencil, &descDSV, &d3d->pDepthStencilView);
			if (FAILED(hr))
				return ;

			d3d->pImmediateContext->OMSetRenderTargets(1, &d3d->pRenderTargetView, d3d->pDepthStencilView);

			// Setup the viewport
			D3D11_VIEWPORT vp;
			vp.Width = (FLOAT)width;
			vp.Height = (FLOAT)height;
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			vp.TopLeftX = 0;
			vp.TopLeftY = 0;
			d3d->pImmediateContext->RSSetViewports(1, &vp);
		}

		void D3D11RenderSystem::BeginScene(){
			float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
			d3d->pImmediateContext->ClearRenderTargetView(d3d->pRenderTargetView, ClearColor);

			//
			// Clear the depth buffer to 1.0 (max depth)
			//
			d3d->pImmediateContext->ClearDepthStencilView(d3d->pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		}

		void D3D11RenderSystem::EndScene(){
			//
			// Present our back buffer to our front buffer
			//
			d3d->pSwapChain->Present(0, 0);
		}

		void D3D11RenderSystem::Resize(int width, int height){}
		void D3D11RenderSystem::LookAt(float eyex, float eyey, float eyez,
			float centrex, float centrey, float centrez,
			float upx, float upy, float upz){

		}
		void D3D11RenderSystem::Perspective(float fovy, float aspect, float zNear, float zFar){}

		void D3D11RenderSystem::RotateEye(float x, float y){}

		void D3D11RenderSystem::RotateLook(float x, float y){}

		void D3D11RenderSystem::SetView(const PerspectiveCamera &camera){}

		void D3D11RenderSystem::SetTransform(TransformType type, const math::Matrix &matWorld){}

		void D3D11RenderSystem::SetColor(const Color &c){}

		void D3D11RenderSystem::SetMaterial(const Material &m){}

		void D3D11RenderSystem::SetTexture(Texture* tex){}

		void D3D11RenderSystem::DrawMesh(const Mesh& mesh){}

		void D3D11RenderSystem::DrawSolidMesh(const Mesh& mesh){}

		void D3D11RenderSystem::DrawWiredMesh(const Mesh& mesh){}

		void D3D11RenderSystem::DrawScene(Scene& scene){}

		void D3D11RenderSystem::DrawSphere(float r){}
		void D3D11RenderSystem::DrawCube(float a, float b, float c){}
		void D3D11RenderSystem::DrawPlane(math::Vector3 normal){}
	}
}
