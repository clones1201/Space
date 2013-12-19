#include "RenderSystem.h"
#include "WindowController.h"


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
		
		class RenderSystemDirect3D::Direct3DDevice{
		public:
			LPDIRECT3D9				pD3D;
			LPDIRECT3DDEVICE9		pd3dDevice;
		};
		
		void RenderSystemDirect3D::Init(HWND hWnd, uint width,uint height){
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

		RenderSystemDirect3D::RenderSystemDirect3D(HWND hWnd, uint width, uint height):RenderSystem(width,height){
			Init(hWnd, width, height);
			camera = PerspectiveCamera_ptr( new PerspectiveCamera);
		}
		RenderSystemDirect3D::~RenderSystemDirect3D(){
			if (d3d->pd3dDevice != NULL)
				d3d->pd3dDevice->Release();

			if (d3d->pD3D != NULL)
				d3d->pD3D->Release();

			if ( d3d != nullptr) delete d3d;
		}
		void RenderSystemDirect3D::Resize(int width, int height){}
		void RenderSystemDirect3D::LookAt(float eyex, float eyey, float eyez,
			float centrex, float centrey, float centrez,
			float upx, float upy, float upz){

		}
		void RenderSystemDirect3D::Perspective(float fovy, float aspect, float zNear, float zFar){}

		void RenderSystemDirect3D::RotateEye(float x, float y){}

		void RenderSystemDirect3D::RotateLook(float x, float y){}

		void RenderSystemDirect3D::SetView(const PerspectiveCamera &camera){}
		 
		void RenderSystemDirect3D::SetTransform(TransformType type, const Matrix &matWorld){}
		
		void RenderSystemDirect3D::SetColor(const Color &c){}

		void RenderSystemDirect3D::SetMaterial(const Material &m){}

		void RenderSystemDirect3D::SetTexture(Texture* tex){}

		void RenderSystemDirect3D::DrawMesh(const Mesh& mesh){}

		void RenderSystemDirect3D::DrawSolidMesh(const Mesh& mesh){}

		void RenderSystemDirect3D::DrawWiredMesh(const Mesh& mesh){}

		void RenderSystemDirect3D::DrawScene(Scene& scene){}

		void RenderSystemDirect3D::DrawSphere(float r){}
		void RenderSystemDirect3D::DrawCube(float a, float b, float c){}
		void RenderSystemDirect3D::DrawPlane(Vector3 normal){}
	}
}