#ifndef __SPACE_RAYTRACING_H__
#define __SPACE_RAYTRACING_H__

#include "basic.h"
#include "RenderSystem.h"
using namespace space;
using namespace space::math;


namespace space{
	namespace graphic{
		namespace raytrace{

			class RenderSystemRayTrace : public RenderSystemOpenGL{
			public:
				RenderSystemRayTrace(HWND hWnd);
				~RenderSystemRayTrace();

				void Resize(int width, int height);

				//template <TransformType t>
				void SetTransform(TransformType type, const Matrix &matWorld);
				void Flush();

				//first, we try some inmidiate command
				//late, we may add handler and vbo management
				virtual void DrawMesh(const Mesh& mesh);
				virtual void DrawSolidMesh(const Mesh& mesh);
				virtual void DrawWiredMesh(const Mesh& mesh);
				virtual void DrawScene(Scene&);
			private:
				class RayTracer;

				RayTracer *tracer;
				Matrix mat[1];

			};
		}
	}
}

#endif