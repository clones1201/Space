#include "RenderSystem.h"
#include "D3D9RenderWindow.h"


#include <mmsystem.h>
//#include <d3d9.h>
#include <d3dx9.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )

//===================
//   
//   To-do list:
//	 * Coordinate system
//   * Matrix 

namespace space{
	namespace graphic{
		
		class D3D9RenderSystem::Direct3DDevice{
		public:
			LPDIRECT3D9				pD3D;
			LPDIRECT3DDEVICE9		pd3dDevice;
		};
		
		void D3D9RenderSystem::Init(HWND hWnd, uint width,uint height){
			d3d = new Direct3DDevice;
			// Create the D3D object.
			if (NULL == (d3d->pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
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
			if (FAILED(d3d->pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &d3d->pd3dDevice)))
			{
				return ;
			}

			d3d->pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

			//Turn on the Zbuffer
			d3d->pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

			return ;
		}

		D3D9RenderSystem::D3D9RenderSystem(HWND hWnd, uint width, uint height):RenderSystem(width,height){
			Init(hWnd, width, height);
			camera = PerspectiveCamera_ptr( new PerspectiveCamera);
		}
		D3D9RenderSystem::~D3D9RenderSystem(){
			if (d3d->pd3dDevice != NULL)
				d3d->pd3dDevice->Release();

			if (d3d->pD3D != NULL)
				d3d->pD3D->Release();

			if ( d3d != nullptr) delete d3d;
		}
		void D3D9RenderSystem::Resize(int width, int height){}
		void D3D9RenderSystem::LookAt(float eyex, float eyey, float eyez,
			float centrex, float centrey, float centrez,
			float upx, float upy, float upz){

		}
		void D3D9RenderSystem::Perspective(float fovy, float aspect, float zNear, float zFar){}

		void D3D9RenderSystem::RotateEye(float x, float y){}

		void D3D9RenderSystem::RotateLook(float x, float y){}

		void D3D9RenderSystem::SetView(const PerspectiveCamera &camera){}
		 
		void D3D9RenderSystem::SetTransform(TransformType type, const math::Matrix &matWorld){}
		
		void D3D9RenderSystem::SetColor(const Color &c){}

		void D3D9RenderSystem::SetMaterial(const Material &m){}

		void D3D9RenderSystem::SetTexture(Texture* tex){}

		void D3D9RenderSystem::DrawMesh(const Mesh& mesh){}

		void D3D9RenderSystem::DrawSolidMesh(const Mesh& mesh){}

		void D3D9RenderSystem::DrawWiredMesh(const Mesh& mesh){}

		void D3D9RenderSystem::DrawScene(Scene& scene){}

		void D3D9RenderSystem::DrawSphere(float r){}
		void D3D9RenderSystem::DrawCube(float a, float b, float c){}
		void D3D9RenderSystem::DrawPlane(math::Vector3 normal){}
	}
}