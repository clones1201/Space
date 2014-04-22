#include "RenderSystem.h"
#include "D3D9RenderSystem.h"
#include "D3D9RenderWindow.h"

namespace space{
	namespace graphic{

		D3D9RenderSystem::D3D9RenderSystem(HINSTANCE hInst) :
			mhInstance(hInst){
			
			InitRenderSystem();

		}


		D3D9RenderSystem::~D3D9RenderSystem(){
		}

		void D3D9RenderSystem::InitRenderSystem(){
			// Create the D3D object.
			if (NULL == (pd3d = Direct3DCreate9(D3D_SDK_VERSION)))
				return ;

			// Set up the structure used to create the D3DDevice
			D3DPRESENT_PARAMETERS d3dpp;
			ZeroMemory(&d3dpp, sizeof(d3dpp));
			d3dpp.Windowed = TRUE;
			d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
			d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
			d3dpp.EnableAutoDepthStencil = TRUE;
			d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
			// Create the D3DDevice
			IDirect3DDevice9 *device;
			if (FAILED(pd3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mhWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &device)))
			{
				return ;
			}

			mDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

			//Turn on the Zbuffer
			mDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

			return ;
		}

	}
}