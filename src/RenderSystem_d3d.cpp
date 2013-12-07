#include "RenderSystem.h"
#include "WindowController.h"
//===================
//   
//   To-do list:
//	 * Coordinate system
//   * Matrix 

namespace space{
	namespace graphic{
		 
		
		void RenderSystemDirect3D::Init(HWND hWnd){
			// Create the D3D object.
			if (NULL == (pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
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
			if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &pd3dDevice)))
			{
				return ;
			}

			pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

			//Turn on the Zbuffer
			pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

			return ;
		}

		RenderSystemDirect3D::RenderSystemDirect3D(HWND hWnd){
			Init(hWnd);
			camera = new PerspectiveCamera;
		}
		RenderSystemDirect3D::~RenderSystemDirect3D(){
			if (pd3dDevice != NULL)
				pd3dDevice->Release();

			if (pD3D != NULL)
				pD3D->Release();
		}
		void RenderSystemDirect3D::Resize(int width, int height){}
		void RenderSystemDirect3D::LookAt(float eyex, float eyey, float eyez,
			float centrex, float centrey, float centrez,
			float upx, float upy, float upz){

		}
		void RenderSystemDirect3D::Perspective(float fovy, float aspect, float zNear, float zFar){

		}
		void RenderSystemDirect3D::RotateEye(float x, float y){

		}
		void RenderSystemDirect3D::RotateLook(float x, float y){

		}
		void RenderSystemDirect3D::SetView(const PerspectiveCamera &camera){

		}
		 
		void RenderSystemDirect3D::SetTransform(TransformType type, const Matrix &matWorld){

		}

		//first, we try some inmidiate command
		//late, we will add handler and vbo management
		void RenderSystemDirect3D::DrawMesh(const Mesh& mesh){
			
		}

		void RenderSystemDirect3D::DrawSolidMesh(const Mesh& mesh){

		}
		void RenderSystemDirect3D::DrawWiredMesh(const Mesh& mesh){

		}

		void RenderSystemDirect3D::DrawScene(Scene& scene){
		}
	}
}