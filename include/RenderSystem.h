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
			virtual void Init(HWND hWnd, uint width, uint height) = 0;
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
			virtual void Flush() = 0;

			virtual void SetColor(const Color&) = 0;
			virtual void SetMaterial(const Material&) = 0;
			virtual void SetTexture(Texture&) = 0;

			virtual void DrawMesh(const Mesh&) = 0;

			virtual void DrawSolidMesh(const Mesh&) = 0;
			virtual void DrawWiredMesh(const Mesh&) = 0;
			
			virtual void DrawScene( Scene&) = 0;

			virtual void DrawSphere(float r) = 0;
			virtual void DrawCube(float a, float b, float c) = 0;
			virtual void DrawPlane(Vector3 normal) = 0;
		};

		//============================
		// RenderSystem
		// Packed Graphic API
		//============================

		class RenderSystem : public Object, public IRenderSystem{
		protected:
			PerspectiveCamera_ptr camera;
			uint width, height;
		public:
			RenderSystem(uint _width, uint _height) :width(_width), height(_height){
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
			void Init(HWND hWnd, uint width, uint height);
		public:
			RenderSystemOpenGL(HWND hWnd,uint width,uint height);
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
			virtual void Flush();

			virtual void SetTexture(Texture&);
			virtual void SetColor(const Color&);
			virtual void SetMaterial(const Material&);
			//first, we try some inmidiate command
			//late, we may add handler and vbo management
			virtual void DrawMesh(const Mesh& mesh);
			virtual void DrawSolidMesh(const Mesh& mesh);
			virtual void DrawWiredMesh(const Mesh& mesh);
			virtual void DrawScene(Scene&); 
			virtual void DrawSphere(float r);
			virtual void DrawCube(float a, float b, float c); 
			virtual void DrawPlane(Vector3 normal);
		};


		class RenderSystemDirect3D : public RenderSystem{
		private:
			class Direct3DDevice;
			Direct3DDevice* d3d;

			void Init(HWND hWnd, uint width, uint height);
		public:
			RenderSystemDirect3D(HWND hWnd, uint width, uint height);
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
			void Flush(){}

			virtual void SetColor(const Color&);
			virtual void SetMaterial(const Material&);
			virtual void SetTexture(Texture&);

			//first, we try some inmidiate command
			//late, we will add handler and vbo management
			virtual void DrawMesh(const Mesh& mesh);
			virtual void DrawSolidMesh(const Mesh& mesh);
			virtual void DrawWiredMesh(const Mesh& mesh);
			virtual void DrawScene(  Scene&);
			virtual void DrawSphere(float r);
			virtual void DrawCube(float a, float b, float c);
			virtual void DrawPlane(Vector3 normal);
		};
	}
}

#endif