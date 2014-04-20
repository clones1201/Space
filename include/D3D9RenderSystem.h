#ifndef __SPACE_D3D9_RENDERSYSTEM_H__
#define __SPACE_D3D9_RENDERSYSTEM_H__

#include "RenderSystem.h"

namespace space{
	namespace graphic{

		class D3D9RenderSystem : virtual public IRenderSystem{
		private:
			class Direct3DDevice;
			Direct3DDevice* d3d;

			void Init(HWND hWnd, uint width, uint height);
		public:
			D3D9RenderSystem(HWND hWnd, uint width, uint height);
			~D3D9RenderSystem();

			void BeginScene();
			void EndScene();

			void Resize(int width, int height);
			void LookAt(float eyex, float eyey, float eyez,
				float centrex, float centrey, float centrez,
				float upx, float upy, float upz);
			void Perspective(float fovy, float aspect, float zNear, float zFar);
			void RotateEye(float x, float y);
			void RotateLook(float x, float y);
			void SetView(const PerspectiveCamera &camera);

			//template <TransformType t>
			void SetTransform(TransformType type, const math::Matrix &matWorld);

			virtual void SetColor(const Color&);
			virtual void SetMaterial(const Material&);
			virtual void SetTexture(Texture*);

			//first, we try some inmidiate command
			//late, we will add handler and vbo management
			virtual void DrawMesh(const Mesh& mesh);
			virtual void DrawSolidMesh(const Mesh& mesh);
			virtual void DrawWiredMesh(const Mesh& mesh);
			virtual void DrawScene(Scene&);
			virtual void DrawSphere(float r);
			virtual void DrawCube(float a, float b, float c);
			virtual void DrawPlane(math::Vector3 normal);
		};

	}
}

#endif