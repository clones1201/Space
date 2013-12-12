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
				RenderSystemRayTrace(HWND hWnd,uint width,uint height);
				~RenderSystemRayTrace();

				void Resize(int width, int height);

				//template <TransformType t>
				void SetTransform(TransformType type, const Matrix &matWorld);
				void Flush();
				 
				void SetColor(const Color& color);
				
				void SetMaterial(const Material &m);

				virtual void DrawMesh(const Mesh& mesh);
				virtual void DrawSolidMesh(const Mesh& mesh);
				virtual void DrawWiredMesh(const Mesh& mesh);
				virtual void DrawScene(Scene&); 
				virtual void DrawSphere(float r);
				virtual void DrawCube(float a, float b, float c);
				virtual void DrawPlane(Vector3 normal);
			private:
				class RayTracer;

				RayTracer *tracer;
				//Matrix mat[1];
			};
		}
	}
}

#endif