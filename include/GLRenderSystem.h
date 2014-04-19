#ifndef __SPACE_GL_RENDERSYSTEM_H__
#define __SPACE_GL_RENDERSYSTEM_H__

#include "RenderSystem.h"

namespace space{
	namespace graphic{
		class GLRenderSystem : public IRenderSystem {
		private:
			HDC						hDC;
			HGLRC					hGLRC;
			HPALETTE				hPalette;
			void Init(HWND hWnd, uint width, uint height);
		public:
			GLRenderSystem(HWND hWnd, uint width, uint height);
			~GLRenderSystem();

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

			virtual void SetTexture(Texture*);
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
			virtual void DrawPlane(math::Vector3 normal);
		};
	}
}

#endif