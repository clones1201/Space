
#include "D3D11Prerequisites.h"
#include "D3D11RenderSystem.h"
#include "D3D11RenderWindow.h"

namespace space{
	namespace graphic{

		class Direct3DDevice{
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

		D3D11Device::D3D11Device(ID3D11Device* device):
			pD3D11Device(device){
			if (pD3D11Device){
				pD3D11Device->GetImmediateContext(&pImmediateContext);
			}
		}

		D3D11Device::D3D11Device() :pD3D11Device(nullptr), pImmediateContext(nullptr){
		}

		D3D11Device::~D3D11Device(){
			//since it may be copied to other place, you can't release any thing here
		}

		bool D3D11Device::isNull() const {
			return (pD3D11Device == nullptr);
		}
		
		D3D11RenderSystem::D3D11RenderSystem(HINSTANCE hInst):
			mhInstance(hInst){

			InitRenderSystem();			
		}

		D3D11RenderSystem::~D3D11RenderSystem(){

			mDevice.Release();
		}	

		//virtual void AttachRenderTarget(IRenderTarget &rt) = 0;
		//virtual void DetachRenderTarget(const string &name) = 0;

		void D3D11RenderSystem::InitRenderSystem(){
			HRESULT hr;

			ID3D11Device * device;
			hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, 0, 
				D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_SINGLETHREADED, NULL, 0, D3D11_SDK_VERSION, &device, 0, 0);

			if (FAILED(hr))
			{
				return;
			}

			mDevice = D3D11Device(device);
		}

		RenderWindow* D3D11RenderSystem::_createRenderWindow(const String &name, uint width, uint height, bool fullScreen){

			RenderWindow * win = new D3D11RenderWindow(mhInstance,mDevice);

			win->Create(name,width,height,fullScreen);

			mActiveRenderTarget = win; //only one render target
			
			return win;
		}

		RenderWindow* D3D11RenderSystem::_Initialize(bool autoCreateWindow){
			
			if (!mDevice.isNull()){
				mDevice.Release();
			}

			HRESULT hr;

			ID3D11Device * device;
			hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, 0,
				D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_SINGLETHREADED, NULL, 0, D3D11_SDK_VERSION, &device, 0, 0);

			if (FAILED(hr))
			{
				return nullptr;
			}

			mDevice = D3D11Device(device);
			RenderWindow* autoWin = nullptr;
			if (autoCreateWindow){
				bool fullScreen = false;
				autoWin = new D3D11RenderWindow(mhInstance, mDevice);
				
			}
			return autoWin;
		}

		void D3D11RenderSystem::_Render(){

		}

		void D3D11RenderSystem::_BeginScene(){

		}

		void D3D11RenderSystem::_EndScene(){

		}

		void D3D11RenderSystem::ShutDown(void){

		}

		void D3D11RenderSystem::SetAmbientLight(float r, float g, float b){

		}

	}
}
