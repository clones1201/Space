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
				RenderSystemRayTrace(HWND hWnd, uint width, uint height);
				~RenderSystemRayTrace();

				void Resize(int width, int height);

				void SetView(const PerspectiveCamera &);
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

			class Shader{
			public:
				Vector3 normal;
				Vector3 hitPos;
				Vector3 dir;

				Ray ray;
				bool hitAnObject;

				Material material;
				//Color color;

				float u, v;
				uint texId;
				Shader() :hitAnObject(false), material(){}
			};

			class Primitive : public Object{
			protected:
				//Color color;
				Material_ptr material;
			public:
				Primitive(Material_ptr ptr) :material(ptr){};
				virtual bool Hit(Ray, float&, Shader&) = 0;
			};
			typedef unique_ptr<Primitive> Primitive_ptr;  /* how to store it in a vector?*/
			//typedef shared_ptr<Primitive> Primitive_ptr;

			class RenderSystemRayTrace::RayTracer : private Interface{
			private:
				vector<Primitive_ptr> prims;
				vector<Material_ptr> materials;
				Material_ptr currentMaterial;
				PerspectiveCamera_ptr camera;

				Matrix matWorld;

				struct StreamSource{
					void* ptr;
					uint stride;
					uint size;
					StreamSource() :ptr(nullptr), stride(0), size(0){}
				}vertices, normals, texcoords;

				Color Shade(const Shader& sd, const Vector3&wi,/*out*/ Vector3& wo, bool isInshadow);

				Color Trace(const vector<Primitive_ptr> &prims, Ray ray, uint depth);

			public:
				RayTracer() :currentMaterial(new Material){
					currentMaterial->diffuse = gray;
					currentMaterial->ka = 1.0f;
					currentMaterial->kd = 0.6f;
				}
				~RayTracer(){}

				template< typename T >
				void Render(vector<T>& image, uint width, uint height){
					/* construct accelerate structure */

					/*trace the ray*/
					float fovy, aspect, zNear, zFar;
					camera->GetPerspective(fovy, aspect, zNear, zFar);
					float t, b, l, r;
					t = 1 * tanf(ToRadian(0.5f * fovy)) + 0.5f;
					b = -t;
					l = -(t - b) * aspect * 0.5f;
					r = -l;
					//Matrix viewMat = camera->GetModelViewMatrix();
					for (uint y = 0; y < height; y++){
						for (uint x = 0; x < width; x++){
							Ray ray;
							ray.ori = Vector3(-1 + 2 * x / float(width) * aspect, -1 + 2 * y / float(height), -1 * zNear);
							Vector3 dist = Vector3(l + (r - l) * x / float(width),
								b + (t - b) * y / float(height),
								-1 * zNear - 1);
							ray.dir = dist - ray.ori;

							Color color = Trace(prims, ray, 4);
							image.push_back(T(color));
						}
					}

					/* make every pixel become the average of 8 pixels round it
					/* not a good way to anti-artifact, make the picture blur 
					*/
					/*for (uint y = 0; y < height; y++){
						for (uint x = 0; x < width; x++){
							T nw, n, ne, e, se, s, sw, w;

							Vector2 pnw(((x - 1 < 0) ? (x) : (x - 1)), (y + 1 > height) ? (y) : (y + 1)),
								pn(x, (y + 1 > height) ? (y) : (y + 1)),
								pne(((x + 1 > width) ? (x) : (x + 1)), (y + 1 > height) ? (y) : (y + 1)),
								pe(((x + 1 > width) ? (x) : (x + 1)), y),
								pse(((x + 1 > width) ? (x) : (x + 1)), (y - 1 < 0) ? (y) : (y - 1)),
								ps(x, (y - 1 < 0) ? (y) : (y - 1)),
								psw(((x - 1 < 0) ? (x) : (x - 1)), (y - 1 < 0) ? (y) : (y - 1)),
								pw(((x - 1 < 0) ? (x) : (x - 1)), y);
							nw = image[pnw.x + pnw.y * width];
							n = image[pn.x + pn.y * width]; 
							ne = image[pne.x + pne.y * width];
							e = image[pe.x + pe.y * width];
							se = image[pse.x + pse.y * width];
							s = image[ps.x + ps.y * width];
							sw = image[psw.x + psw.y * width];
							w = image[pw.x + pw.y * width];

							T aver = T((nw.r + n.r + ne.r + e.r + se.r + s.r + sw.r + w.r + image[x + y *width].r ) / 8,
								(nw.g + n.g + ne.g + e.g + se.g + s.g + sw.g + w.g + image[x + y *width].g) / 8,
								(nw.b + n.b + ne.b + e.b + se.b + s.b + sw.b + w.b + image[x + y *width].b) / 8,
								1);
							image[x + y *width] = T(aver);
						}
					}*/
					prims.clear();
					materials.clear();
				}
				void SetView(const PerspectiveCamera&camera);

				void SetMatrix(const Matrix& mat);

				void SetColor(const Color& color);

				void SetMaterial(const Material& m);
				/* couldn't decide the interface...
				/* seems that Direct3D style have more flexibility,
				/* but OpenGl style may be easier to implement ...*/

				void SetVertexPointer(uint size, uint stride, const float* vertices);
				void SetNormalPointer(uint stride, const float* normals);
				void SetTexCoordPointer(uint size, uint stride, const float* texcoords);
				void DrawSphere(float r);
				void DrawPlane(Vector3 normal);
				void DrawElements(PrimitiveType type, uint count, uint size, const uint* indices);
			};
		}
	}
}

#endif