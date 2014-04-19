#ifndef __RENDERSYSTEM_H__
#define __RENDERSYSTEM_H__

#include "Prerequisites.h"
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
			PerspectiveCamera_ptr camera;
			uint width, height;
		protected:
			virtual void Init(HWND hWnd, uint width, uint height) = 0;
		public:
			IRenderSystem(uint width, uint height) :width(width), height(height){}
			~IRenderSystem(){}

			virtual void BeginScene() = 0;
			virtual void EndScene() = 0;

			virtual void Resize(int width, int height) = 0;
			virtual void Perspective(float fovy, float aspect, float zNear, float zFar) = 0;
			virtual void LookAt(float eyex, float eyey, float eyez,
				float centrex, float centrey, float centrez,
				float upx, float upy, float upz) = 0;
			virtual void RotateEye(float x, float y) =0;
			virtual void RotateLook(float x, float y) =0;
			virtual void SetView(const PerspectiveCamera &camera) = 0;
			//template <TransformType t>
			virtual void SetTransform(TransformType type, const math::Matrix&) = 0;

			virtual void SetColor(const Color&) = 0;
			virtual void SetMaterial(const Material&) = 0;
			virtual void SetTexture(Texture*) = 0;

			virtual void DrawMesh(const Mesh&) = 0;

			virtual void DrawSolidMesh(const Mesh&) = 0;
			virtual void DrawWiredMesh(const Mesh&) = 0;
			
			virtual void DrawScene( Scene&) = 0;

			virtual void DrawSphere(float r) = 0;
			virtual void DrawCube(float a, float b, float c) = 0;
			virtual void DrawPlane(math::Vector3 normal) = 0;
		};

		//============================
		// RenderSystem
		// Packed Graphic API
		//============================
		typedef shared_ptr<IRenderSystem> IRenderSystem_ptr;
		
	}
}

#endif