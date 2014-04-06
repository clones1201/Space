#ifndef __SPACE_RAYTRACING_H__
#define __SPACE_RAYTRACING_H__

#include "basic.h"
#include "RenderSystem.h"

namespace space{
	namespace graphic{
		namespace raytrace{

			class RenderSystemRayTrace : public RenderSystemOpenGL{
			public:
				RenderSystemRayTrace(HWND hWnd, uint width, uint height);
				~RenderSystemRayTrace();

				void BeginScene();
				void EndScene();

				void Resize(int width, int height);

				void SetView(const PerspectiveCamera &);
				//template <TransformType t>
				void SetTransform(TransformType type, const math::Matrix &matWorld);
				
				void SetColor(const Color& color);
				void SetMaterial(const Material &m);
				void SetTexture(Texture*  tex);

				virtual void DrawMesh(const Mesh& mesh);
				virtual void DrawSolidMesh(const Mesh& mesh);
				virtual void DrawWiredMesh(const Mesh& mesh);
				virtual void DrawScene(Scene&);
				virtual void DrawSphere(float r);
				virtual void DrawCube(float a, float b, float c);
				virtual void DrawPlane(math::Vector3 normal);
			private:
				class RayTracer;

				RayTracer *tracer;
				//Matrix mat[1];
			};

			class Shader{
			public:
				math::Vector3 normal;
				math::Vector3 hitPos;
				math::Vector3 dir;

				Ray ray;
				bool hitAnObject;

				Material material;

				float u, v;
				Texture_ptr tex_ptr;
				Shader() :hitAnObject(false), material(){}
			};

			class Primitive :virtual public Object{
			protected:
				Material_ptr material;
				Texture_ptr texture;
			public:
				Primitive(Material_ptr mptr, Texture_ptr tptr) :material(mptr),texture(tptr){};
				virtual bool Intersect(Ray, float&, Shader&) = 0;
				virtual void CalculateBoundsBox(math::Vector3 &max, math::Vector3 &min) const = 0;
			};
			typedef shared_ptr<Primitive> Primitive_ptr; 
			//typedef shared_ptr<Primitive> Primitive_ptr;

			class BSPNode : public Primitive, public util::BinaryTreeNode< BBox >{
			public:
				//typedef shared_ptr< BSPNode > Ptr;
				/*...*/
				BSPNode() :Primitive(nullptr, nullptr), util::BinaryTreeNode< BBox >(){}
				BSPNode(BBox box) :Primitive(nullptr, nullptr), util::BinaryTreeNode< BBox >(box){

				}

				virtual bool Intersect(Ray ray, float&t, Shader& sd);
			
				virtual void CalculateBoundsBox(math::Vector3 &max, math::Vector3 &min)const{
					max = elem.bmax; min = elem.bmin;
				}
			};

			class BSPLeaf :public Primitive, public util::BinaryTreeLeaf< BBox >{
			private:
				vector<Primitive_ptr> prims;
			public:
				//typedef shared_ptr< BSPLeaf > Ptr;

				BSPLeaf() : Primitive(nullptr, nullptr), util::BinaryTreeLeaf< BBox >(){}
				BSPLeaf(BBox box, const vector<Primitive_ptr>& prims) :Primitive(nullptr, nullptr), util::BinaryTreeLeaf< BBox >(box), prims(prims){}

				virtual bool Intersect(Ray ray, float&t, Shader& sd);

				virtual void CalculateBoundsBox(math::Vector3 &max, math::Vector3 &min)const{
					max = elem.bmax; min = elem.bmin;
				}
			};

			const uint maxDepth = 16;
			BSPNode::Ptr BuildBSPTree(/*out*/vector<Primitive_ptr>& bsp, const vector<Primitive_ptr>& prims, uint depth);

			void CreatePrimitives(vector<Primitive_ptr>& prims, const Mesh& mesh);

			class RenderSystemRayTrace::RayTracer : private Interface{
			private:
				vector<Primitive_ptr> prims;
				vector<Material_ptr> materials;
				vector<Texture_ptr> textures;

				Material_ptr currentMaterial;
				Texture_ptr currentTexture;

				PerspectiveCamera_ptr camera;

				math::Matrix matWorld;

				struct StreamSource{
					void* ptr;
					uint stride;
					uint size;
					StreamSource() :ptr(nullptr), stride(0), size(0){}
				}vertices, normals, texcoords;

				Color Shade(const Shader& sd, const math::Vector3&wi,/*out*/ math::Vector3& wo, bool isInshadow);

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
					vector<Primitive_ptr> bspPrims;
					BSPNode::Ptr root = BuildBSPTree(bspPrims, prims, maxDepth);

					/*trace the ray*/
					float fovy, aspect, zNear, zFar;
					camera->GetPerspective(fovy, aspect, zNear, zFar);
					float t, b, l, r;
					t = 1 * tanf(ToRadian(0.5f * fovy)) + 0.5f;
					b = -t;
					l = -(t - b) * aspect * 0.5f;
					r = -l;
					math::Matrix viewMat = camera->GetModelViewMatrix();
					for (uint y = 0; y < height; y++){
						for (uint x = 0; x < width; x++){
							Ray ray;
							ray.ori = math::Vector3((-1 + 2 * x / float(width)) * aspect, -1 + 2 * y / float(height), -1 * zNear);
							math::Vector3 dist = math::Vector3(l + (r - l) * x / float(width),
								b + (t - b) * y / float(height),
								-1 * zNear - 1);
							ray.dir = dist - ray.ori;

							Color color = Trace(bspPrims, ray, 2);
							image.push_back(T(color));
						}
					}

					prims.clear();
					materials.clear();

					/* make every pixel become the average of 8 pixels round it
					/* not a good way to anti-alias, make the picture blur 
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
				}
				void SetView(const PerspectiveCamera&camera);

				void SetMatrix(const math::Matrix& mat);

				void SetColor(const Color& color);

				void SetTexture(Texture* tex);

				void SetMaterial(const Material& m);
				/* couldn't decide the interface...
				/* seems that Direct3D style have more flexibility,
				/* but OpenGl style may be easier to implement ...*/
				
				void SetVertexPointer(uint size, uint stride, const float* vertices);
				void SetNormalPointer(uint stride, const float* normals);
				void SetTexCoordPointer(uint size, uint stride, const float* texcoords);
				void DrawSphere(float r);
				void DrawPlane(math::Vector3 normal);
				void DrawElements(PrimitiveType type, uint count, uint size, const uint* indices);
			};
		}
	}
}

#endif