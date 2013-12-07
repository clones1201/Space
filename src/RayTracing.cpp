#include "RayTracing.h"

namespace space{
	namespace graphic{
		namespace raytrace{
			const float kEpsilon = 0.001;
			
			class Shader{
			public:
				Vector3 normal;
				Vector3 hitPos;
				Vector3 dir;

				Ray ray;
				bool hitAnObject;
				
				//Material material;

				Color color;

				float u, v;
				uint texId;
				Shader():hitAnObject(false),color(blue){}
			};

			class Primitive : public Object{
			protected:
				Color color;
			public:
				Primitive(Color color) :color(color){};
				virtual bool Hit(Ray ,float&, Shader&) = 0;
			};

			typedef shared_ptr<Primitive> Primitive_ptr;
			class Sphere : public Primitive{
			private:
				float r;
				Vector3 centre;
			public:
				Sphere(float r = 1, Vector3 centre = Vector3(0, 0, 0),Color color = Color(1,0,0,1)):
					Primitive(color),r(r),centre(centre){}
				bool Hit(Ray ray,float&tmin, Shader& sd){
					float t;
					Vector3 temp = ray.ori - centre;
					float a = Vec3Dot( ray.dir , ray.dir);
					float b = 2.0 * Vec3Dot(temp , ray.dir);
					float c = Vec3Dot(temp, temp) - r * r;
					float disc = b * b - 4 * a * c;

					if (disc < 0.0)
						return false;
					else{
						float e = sqrt(disc);
						float denom = 2.0 * a;
						t = (-b - e) / denom;

						if (t > kEpsilon){
							tmin = t;
							sd.normal = Vec3Normalize((temp + t * ray.dir) / r);
							sd.hitPos = ray.ori + t * ray.dir;
							return true;
						}

						t = (-b + e) / denom;

						if (t > kEpsilon){
							tmin = t;
							sd.normal = Vec3Normalize((temp + t * ray.dir) / r);
							sd.hitPos = ray.ori + t * ray.dir;
							return true;
						}
					}
					return false;
				}
			};

			class Plane : public Primitive{
			private:
				Vector3 normal;
				Vector3 pos;
			public:
				Plane(Vector3 normal = Vector3(0, 1, 0), Vector3 pos = Vector3(0, 0, 0), Color color = Color(1, 0, 0, 1)) :
					Primitive(color),normal(normal), pos(pos){}
				bool Hit(Ray, float &t, Shader&);
			};

			class Triangle : public Primitive{
			private:
				graphic::Triangle tri;
			public:
				Triangle(const graphic::Triangle &_tri,Color color = Color(1, 0, 0, 1)):
				Primitive(color),tri(_tri){}
				bool Hit(Ray ray, float&t, Shader& sd){
					if (tri.Intersect(ray, t)){
						sd.hitAnObject = true;
						sd.hitPos = ray.ori + t * ray.dir;
						sd.normal = (tri.n0 + tri.n1 + tri.n2) / 3;
						sd.color = color;
					}
					return false;
				}
			};

			class RenderSystemRayTrace::RayTracer{
			private:
				vector<Primitive_ptr> prims;
				PerspectiveCamera* camera;
				
				Matrix matWorld;

				struct StreamSource{
					void* ptr;
					uint stride;
					uint size;
					StreamSource() :ptr(nullptr), stride(0), size(0){}
				}vertices, normals,texcoords;

				Color Trace(const vector<Primitive_ptr> &prims, Ray ray, uint depth){
					float t,tmin = INFINITY;
					Shader sd;
					/*for (uint i = 0; i < prims.size(); i++){
						Shader sdt;
						if (prims[i]->Hit(ray, t, sdt)){
							if (t < tmin){
								sd = sdt;
								tmin = t;
							}
						}
					}*/
					graphic::Triangle tri;
					tri.v0 = Vector3(1, 0, 0);
					tri.v1 = Vector3(-1, 0, 0);
					tri.v2 = Vector3(0, 1, 0);
					Triangle(tri).Hit(ray, t, sd);
					if ( sd.hitAnObject )
						return sd.color;
					else return blue;
				}
			public:
				RayTracer(){}
				~RayTracer(){
					if (camera != nullptr) delete camera;
				}

				template< typename T > 
				void Render(vector<T>& image, uint w, uint h){
					/* construct accelerate structure */

					/*trace the ray*/
					float fovy, aspect, zNear, zFar;
					camera->GetPerspective(fovy, aspect, zNear, zFar);
					float t, b, l, r;
					t = 1 * tanf(ToRadian(0.5f * fovy)) + 0.5f;
					b = -t;
					l = - (t - b) / aspect * 0.5f;
					r = -l;
					Matrix viewMat = camera->GetModelViewMatrix();
					for (uint y = 0; y < h; y++){
						for (uint x = 0; x < w; x++){
							Ray ray;
							ray.ori = Vector3(-1 + 2 * x / float(w), -1 + 2 * y / float(h), -1 * zNear);
							Vector3 dist = Vector3(b + (t - b) * x / float(w),
								l + (r - l) * y / float(h),
								-1 * zNear - 1);
							ray.ori = Vec3Transform(MatrixInverse(viewMat), Vector4(ray.ori));
							dist = Vec3Transform(MatrixInverse(viewMat), Vector4(dist));
							ray.dir = dist - ray.ori;

							Color color = Trace(prims, ray, 6);
							image.push_back(T(color));
						}
					}

					prims.clear();
				}
				void SetView(const PerspectiveCamera&camera){
					PerspectiveCamera *temp = new PerspectiveCamera(camera);
					if (temp != nullptr){
						delete RayTracer::camera;
						RayTracer::camera = temp;
					}
				}

				/* couldn't decide the interface... 
				/* seems that Direct3D style have more flexibility,
				/* but OpenGl style may be easier to implement ...*/

				void SetVertexPointer(uint size, uint stride,const float* vertices){
					RayTracer::vertices.ptr = (float*)vertices;
					RayTracer::vertices.size = size;
					RayTracer::vertices.stride = stride;
				}
				void SetNormalPointer(uint stride,const float* normals){
					RayTracer::normals.ptr = (float*) normals;
					RayTracer::normals.size = 3;
					RayTracer::normals.stride = stride;
				}
				void SetTexCoordPointer(uint size, uint stride,const float* texcoords){
					RayTracer::texcoords.ptr = (float*)texcoords;
					RayTracer::texcoords.size = size;
					RayTracer::texcoords.stride = stride;
				}
				void DrawElements(PrimitiveType type, uint count, uint size,const uint* indices){
					// do vertex process here

					switch (type){
					case SP_TRIANGLES:

						for (uint i = 0; i < count; i += 3){
							graphic::Triangle tri;

							tri.v0 = Vec3Transform(matWorld, Vector4(((float*)vertices.ptr)[i * vertices.stride], ((float*)vertices.ptr)[i * vertices.stride + 1], ((float*)vertices.ptr)[i * vertices.stride + 2], 1));
							tri.v1 = Vec3Transform(matWorld, Vector4(((float*)vertices.ptr)[(i + 2) * vertices.stride], ((float*)vertices.ptr)[(i + 1) * vertices.stride + 1], ((float*)vertices.ptr)[(i + 1) * vertices.stride + 2], 1));
							tri.v2 = Vec3Transform(matWorld, Vector4(((float*)vertices.ptr)[(i + 2) * vertices.stride], ((float*)vertices.ptr)[(i + 2) * vertices.stride + 1], ((float*)vertices.ptr)[(i + 2) * vertices.stride + 2], 1));
							
							if (normals.ptr != nullptr){
								tri.n0 = Vector3(((float*)normals.ptr)[i * normals.stride], ((float*)normals.ptr)[i * normals.stride + 1], ((float*)normals.ptr)[i * normals.stride + 2]);
								tri.n1 = Vector3(((float*)normals.ptr)[(i + 2) * normals.stride], ((float*)normals.ptr)[(i + 1) * normals.stride + 1], ((float*)normals.ptr)[(i + 1) * normals.stride + 2]);
								tri.n2 = Vector3(((float*)normals.ptr)[(i + 2) * normals.stride], ((float*)normals.ptr)[(i + 2) * normals.stride + 1], ((float*)normals.ptr)[(i + 2) * normals.stride + 2]);
							}
							if (texcoords.ptr != nullptr){
								if (texcoords.size == 3){
									tri.t0 = Vector3(((float*)texcoords.ptr)[i * texcoords.stride], ((float*)texcoords.ptr)[i * texcoords.stride + 1], ((float*)texcoords.ptr)[i * texcoords.stride + 2]);
									tri.t1 = Vector3(((float*)texcoords.ptr)[(i + 2) * texcoords.stride], ((float*)texcoords.ptr)[(i + 1) * texcoords.stride + 1], ((float*)texcoords.ptr)[(i + 1) * texcoords.stride + 2]);
									tri.t2 = Vector3(((float*)texcoords.ptr)[(i + 2) * texcoords.stride], ((float*)texcoords.ptr)[(i + 2) * texcoords.stride + 1], ((float*)texcoords.ptr)[(i + 2) * texcoords.stride + 2]);
								}
								else if (texcoords.size == 2){
									tri.t0 = Vector3(((float*)texcoords.ptr)[i * texcoords.stride], ((float*)texcoords.ptr)[i * texcoords.stride + 1], 0 );
									tri.t1 = Vector3(((float*)texcoords.ptr)[(i + 2) * texcoords.stride], ((float*)texcoords.ptr)[(i + 1) * texcoords.stride + 1], 0 );
									tri.t2 = Vector3(((float*)texcoords.ptr)[(i + 2) * texcoords.stride], ((float*)texcoords.ptr)[(i + 2) * texcoords.stride + 1], 0 );								}
							}
							Primitive_ptr prim(new Triangle(tri));
							prims.push_back(prim);
						}
					}
				}
			};
			
			RenderSystemRayTrace::RenderSystemRayTrace(HWND hWnd): RenderSystemOpenGL(hWnd){
				tracer = new RayTracer;

				glClearColor(0, 0, 0, 1);
			}

			RenderSystemRayTrace::~RenderSystemRayTrace(){
				delete tracer;
			}
			void RenderSystemRayTrace::Resize(int width, int height){
			
			}
			
			void RenderSystemRayTrace::SetTransform(TransformType type, const Matrix &matWorld){
				switch (type){
				case SP_VIEW:
					mat[SP_VIEW] = matWorld;
					break;
				}
			}
			void RenderSystemRayTrace::Flush(){
				vector<ABGRColor> image; // low-endian byte order, see http://stackoverflow.com/questions/7786187/opengl-texture-upload-unsigned-byte-vs-unsigned-int-8-8-8-8
				
				tracer->SetView(*camera);
				tracer->Render<ABGRColor>(image, 650, 650);

				glClear(GL_COLOR_BUFFER_BIT);
				glDrawPixels(650, 650, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, image.data());
				glFlush();
			}

			void RenderSystemRayTrace::DrawMesh(const Mesh& mesh){}
			void RenderSystemRayTrace::DrawSolidMesh(const Mesh& mesh){
				//add all triangles to vector<Primitive*> prims
				tracer->SetVertexPointer(mesh.GetPositionSize(), mesh.GetCompiledVertexSize(), mesh.GetCompiledVertices());
				tracer->SetNormalPointer(mesh.GetCompiledVertexSize(), mesh.GetCompiledVertices() + mesh.GetCompiledNormalOffset());
				tracer->SetTexCoordPointer(mesh.GetTexCoordSize(), mesh.GetCompiledVertexSize(), mesh.GetCompiledVertices() + mesh.GetCompiledTexCoordOffset());

				tracer->DrawElements(SP_TRIANGLES, mesh.GetCompiledIndexCount(),4 ,mesh.GetCompiledIndices());

			}
			void RenderSystemRayTrace::DrawWiredMesh(const Mesh& mesh){}
			void RenderSystemRayTrace::DrawScene(Scene&){}
		}
	}
}