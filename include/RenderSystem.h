#ifndef __RENDERSYSTEM_H__
#define __RENDERSYSTEM_H__

#include "global.h"
#include "basic.h"
#include "Entity.h"

namespace space{
	namespace graphic{

		typedef enum _RenderSystemType{
			RS_OGL = 0, RS_D3D = 1, RS_RT = 2
		}RenderSystemType;

		typedef enum _TransformType{
			SP_VIEW
		} TransformType;

		class IRenderSystem : public  Interface{
		protected:
			virtual void Init(HWND hWnd) = 0;
		public:
			virtual void Resize(int width, int height) = 0;
			virtual void Perspective(float fovy, float aspect, float zNear, float zFar) = 0;
			virtual void LookAt(float eyex, float eyey, float eyez,
				float centrex, float centrey, float centrez,
				float upx, float upy, float upz) = 0;
			virtual void RotateEye(float x, float y) =0;
			virtual void RotateLook(float x, float y) =0;
			virtual void SetView(const PerspectiveCamera &camera) = 0;
			//template <TransformType t>
			virtual void SetTransform(TransformType type, const Matrix&) = 0;
			virtual void SwapBuffer() = 0;

			virtual void DrawMesh(const Mesh&) = 0;

			virtual void DrawSolidMesh(const Mesh&) = 0;
			virtual void DrawWiredMesh(const Mesh&) = 0;
			virtual void DrawScene( Scene&) = 0;
		};

		//============================
		// RenderSystem
		// Packed Graphic API
		//============================

		class RenderSystem : public Object, public IRenderSystem{
		protected:
			PerspectiveCamera_ptr camera;
		public:
			RenderSystem(){
			}
			~RenderSystem(){
			}
		};
		typedef shared_ptr<RenderSystem> RenderSystem_ptr;

		class RenderSystemOpenGL : public RenderSystem {
		private:
			HDC						hDC;
			HGLRC					hGLRC;
			HPALETTE				hPalette;
			void Init(HWND hWnd);
		public:
			RenderSystemOpenGL(HWND hWnd);
			~RenderSystemOpenGL();

			void Resize(int width, int height);
			
			void LookAt(float eyex, float eyey, float eyez,
				float centrex, float centrey, float centrez,
				float upx, float upy, float upz);
			void Perspective(float fovy, float aspect, float zNear, float zFar);
			void RotateEye(float x, float y);
			void RotateLook(float x, float y);
			void SetView(const PerspectiveCamera &camera);


			//template <TransformType t>
			void SetTransform(TransformType type, const Matrix &matWorld);
			void SwapBuffer();

			//first, we try some inmidiate command
			//late, we may add handler and vbo management
			virtual void DrawMesh(const Mesh& mesh);
			virtual void DrawSolidMesh(const Mesh& mesh);
			virtual void DrawWiredMesh(const Mesh& mesh);
			virtual void DrawScene(  Scene&);
		};


		class RenderSystemDirect3D : public RenderSystem{
		private:
			LPDIRECT3D9				pD3D;
			LPDIRECT3DDEVICE9		pd3dDevice;

			void Init(HWND hWnd);
		public:
			RenderSystemDirect3D(HWND hWnd);
			~RenderSystemDirect3D();

			void Resize( int width, int height);
			void LookAt(float eyex, float eyey, float eyez,
				float centrex, float centrey, float centrez,
				float upx, float upy, float upz);
			void Perspective(float fovy, float aspect, float zNear, float zFar);
			void RotateEye(float x, float y);
			void RotateLook(float x, float y);
			void SetView(const PerspectiveCamera &camera);

			//template <TransformType t>
			void SetTransform(TransformType type, const Matrix &matWorld);

			void SwapBuffer(){}

			//first, we try some inmidiate command
			//late, we will add handler and vbo management
			virtual void DrawMesh(const Mesh& mesh);
			virtual void DrawSolidMesh(const Mesh& mesh);
			virtual void DrawWiredMesh(const Mesh& mesh);
			virtual void DrawScene(  Scene&);
		};
	}
}

#endif