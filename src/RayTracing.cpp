#include "RayTracing.h"

namespace space{
	namespace graphic{
		namespace raytrace{
			const float kEpsilon = 0.001;

			class Sphere : public Primitive{
			private:
				float r;
				Vector3 centre;
			public:
				Sphere(Material_ptr m, float r = 1, Vector3 centre = Vector3(0, 0, 0)) :
					Primitive(m), r(r), centre(centre){}
				bool Hit(Ray ray, float&tmin, Shader& sd){
					float t;
					sd.hitAnObject = false;
					Vector3 temp = ray.ori - centre;
					float a = Vec3Dot(ray.dir, ray.dir);
					float b = 2.0 * Vec3Dot(temp, ray.dir);
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
							sd.hitPos = ray.ori + t * ray.dir;
							sd.normal = Vec3Normalize(sd.hitPos - centre);
							sd.material = *material;
							sd.ray = ray;
							sd.hitAnObject = true;
							return true;
						}

						t = (-b + e) / denom;

						if (t > kEpsilon){
							tmin = t;
							sd.hitPos = ray.ori + t * ray.dir;
							sd.normal = Vec3Normalize(sd.hitPos - centre);
							sd.material = *material;
							sd.ray = ray;
							sd.hitAnObject = true;
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
				Plane(Material_ptr m, Vector3 normal = Vector3(0, 1, 0), Vector3 pos = Vector3(0, 0, 0)) :
					Primitive(m), normal(normal), pos(pos){}
				bool Hit(Ray ray, float &t, Shader& sd){
					sd.hitAnObject = false;
					float fm;
					if ((fm = Vec3Dot(normal, ray.dir)) == 0){ // parallel to plane
						return false;
					}
					float r1 = Vec3Dot(normal, pos - ray.ori) / Vec3Dot(normal, ray.dir);
					if (r1 < 0){// wrong direction
						return false;
					}

					sd.hitAnObject = true;
					sd.normal = normal;
					sd.hitPos = ray.ori + ray.dir * r1 + kEpsilon * sd.normal;
					t = r1;
					sd.material = *material;
					sd.ray = ray;
					return true;
				}
			};

			class Triangle : public Primitive{
			private:
				graphic::Triangle tri;
			public:
				Triangle(Material_ptr m, const graphic::Triangle &_tri) :
					Primitive(m), tri(_tri){}
				bool Hit(Ray ray, float&t, Shader& sd){
					if (tri.Intersect(ray, t)){
						sd.hitAnObject = true;
						sd.normal = (tri.n0 + tri.n1 + tri.n2) / 3;
						sd.hitPos = ray.ori + t * ray.dir + kEpsilon * sd.normal;
						sd.material = *material;
						sd.ray = ray;
						return true;
					}
					return false;
				}
			};

			Color RenderSystemRayTrace::RayTracer::Shade(const Shader& sd, const Vector3&wi,/*out*/ Vector3& wo, bool isInshadow){
				wo = -sd.ray.dir;
				Material m = sd.material;
				float ndotwi = Vec3Dot(sd.normal, wi);
				Vector3 r = -wi + sd.normal * 2.0 * ndotwi;
				float rdotwo = Vec3Dot(r, wo);
				Color ls;
				if (isInshadow){
					ls = Color(0.1f, 0.1f, 0.1f, 1.0f);
				}
				else{
					ls = Color(1.0f, 1.0f, 1.0f, 1.0f);
				}
				/* Ambien */
				Color ambient = ls * (m.ambient + m.diffuse) * m.ka;
				Color lambertian, Glossy, Reflect;

				if (ndotwi > 0.01){
					/* Lambertian */
					lambertian = ls * m.diffuse * m.kd * ndotwi;
					/* Glossy */
					Glossy = ls * m.specular * m.ks * powf(rdotwo, sd.material.n);
				}
				return ambient + lambertian + Glossy;
			}
			Color RenderSystemRayTrace::RayTracer::Trace(const vector<Primitive_ptr> &prims, Ray ray, uint depth){
				if (depth == 0)
					return black;

				float t, tmin = INFINITY;
				Shader sd;
				for (uint i = 0; i < prims.size(); i++){
					Shader sdt;

					/* late, change it to a BSP tree */
					if (prims[i]->Hit(ray, t, sdt)){
						if (t < tmin){
							sd = sdt;
							tmin = t;
						}
					}
				}
				if (sd.hitAnObject){
					if (t > 5000) return black;

					Matrix matView = camera->GetModelViewMatrix();
					Vector3 lightpos = Vec3Transform(matView, Vector4(0, 2, 2, 1));
					Vector3 wi, wo;
					wi = Vec3Normalize(lightpos - sd.hitPos);

					/* Shadow */
					Ray shadowRay; shadowRay.ori = sd.hitPos;
					shadowRay.dir = wi;

					bool isInShadow = false;
					for (uint i = 0; i < prims.size(); i++){
						if (prims[i]->Hit(shadowRay, t, Shader())){
							isInShadow = true;
							break;
						}
					}
					Color color = Shade(sd, wi, wo, isInShadow);

					/* Reflection */
					float ndotwo = Vec3Dot(sd.normal, wo);
					Vector3 reflect = -wo + sd.normal * 2.0 * ndotwo;
					Ray reflectRay;
					reflectRay.ori = sd.hitPos; reflectRay.dir = Vec3Normalize(reflect);

					/*Refraction*/
					//Vector3 refract = -wo + -sd.normal * ndotwo * 1.3;
					//Ray refractRay;
					//refractRay.ori = sd.hitPos - 2 * kEpsilon * sd.normal; /* below the surface */
					//refractRay.dir = Vec3Normalize(refract);
					//Shader rsd;
					///* inside the object, find the way out */
					//for (uint i = 0; i < prims.size(); i++){
					//	if (prims[i]->Hit(refractRay, t, rsd)){
					//		wo = -refractRay.dir;
					//		ndotwo = Vec3Dot(sd.normal, wo);
					//		refract = -wo + rsd.normal * ndotwo * 1/1.3;
					//		refractRay.ori = rsd.hitPos; 
					//		refractRay.dir = Vec3Normalize(refract);
					//		break;
					//	}
					//}
					/*Glossy Reflection*/

					/* Indirect illumination */
					/* using Monte Carlo 
					/* not working good
					*/
					/*Color indirectIllumination;
					Shader isd;

					if (depth > 3){
						for (uint i = 0; i < 80; i++){
							Ray iRay;
							iRay.ori = sd.hitPos;
							iRay.dir = Vec3Normalize(Sample::Instance()->HemiSphere(sd.normal, sd.hitPos, 1, 0));
							indirectIllumination = indirectIllumination + (1 / 80.0) * Trace(prims, iRay, 1);
						}
					}*/

					return color /*+ indirectIllumination */
						+ sd.material.reflect * Trace(prims, reflectRay, depth - 1)
						/* + sd.material.refract * Trace(prims, refractRay, depth - 1)*/
						;
				}
				else return black;
			}
			void RenderSystemRayTrace::RayTracer::SetView(const PerspectiveCamera&camera){
				RayTracer::camera = PerspectiveCamera_ptr(new PerspectiveCamera(camera));
			}

			void RenderSystemRayTrace::RayTracer::SetMatrix(const Matrix& mat){
				matWorld = camera->GetModelViewMatrix() * mat;
			}

			void RenderSystemRayTrace::RayTracer::SetColor(const Color& color){
				Material_ptr m = Material_ptr(new Material(*currentMaterial.get()));
				m->diffuse = color;
				m->ambient = color;
				//m->specular = color;
				if (materials.end() == find(materials.begin(), materials.end(), m)){
					materials.push_back(m);
				}
				currentMaterial = m;
			}

			void RenderSystemRayTrace::RayTracer::SetMaterial(const Material& m){
				currentMaterial = Material_ptr(new Material(m));
				if (materials.end() != find(materials.begin(), materials.end(), currentMaterial)){
					materials.push_back(currentMaterial);
				}
			}
			/* couldn't decide the interface...
			/* seems that Direct3D style have more flexibility,
			/* but OpenGl style may be easier to implement ...*/

			void RenderSystemRayTrace::RayTracer::SetVertexPointer(uint size, uint stride, const float* vertices){
				RayTracer::vertices.ptr = (float*)vertices;
				RayTracer::vertices.size = size;
				RayTracer::vertices.stride = stride;
			}
			void RenderSystemRayTrace::RayTracer::SetNormalPointer(uint stride, const float* normals){
				RayTracer::normals.ptr = (float*)normals;
				RayTracer::normals.size = 3;
				RayTracer::normals.stride = stride;
			}
			void RenderSystemRayTrace::RayTracer::SetTexCoordPointer(uint size, uint stride, const float* texcoords){
				RayTracer::texcoords.ptr = (float*)texcoords;
				RayTracer::texcoords.size = size;
				RayTracer::texcoords.stride = stride;
			}
			void RenderSystemRayTrace::RayTracer::DrawSphere(float r){
				Vector3 centre = Vec3Transform(matWorld, Vector4(0, 0, 0, 1));

				prims.push_back(Primitive_ptr(new Sphere(currentMaterial, r, centre)));
			}
			void RenderSystemRayTrace::RayTracer::DrawPlane(Vector3 normal){
				Vector3 position = Vec3Transform(matWorld, Vector4(0, 0, 0, 1));
				normal = Vec3Transform(matWorld, Vector4(normal)) - position;

				prims.push_back(Primitive_ptr(new Plane(currentMaterial, normal, position)));
			}
			void RenderSystemRayTrace::RayTracer::DrawElements(PrimitiveType type, uint count, uint size, const uint* indices){
				// do vertex process here
				Vector3 o = Vec3Transform(matWorld, Vector4(0, 0, 0, 1));
				switch (type){
				case SP_TRIANGLES:

					for (uint i = 0; i < count; i += 3){
						graphic::Triangle tri;
						uint idx0 = indices[i];
						uint idx1 = indices[i + 1];
						uint idx2 = indices[i + 2];

						tri.v0 = Vec3Transform(matWorld,
							Vector4(((float*)vertices.ptr)[idx0 * vertices.stride],
							((float*)vertices.ptr)[idx0 * vertices.stride + 1],
							((float*)vertices.ptr)[idx0 * vertices.stride + 2], 1));
						tri.v1 = Vec3Transform(matWorld,
							Vector4(((float*)vertices.ptr)[idx1 * vertices.stride],
							((float*)vertices.ptr)[idx1 * vertices.stride + 1],
							((float*)vertices.ptr)[idx1 * vertices.stride + 2], 1));
						tri.v2 = Vec3Transform(matWorld,
							Vector4(((float*)vertices.ptr)[idx2 * vertices.stride],
							((float*)vertices.ptr)[idx2 * vertices.stride + 1],
							((float*)vertices.ptr)[idx2 * vertices.stride + 2], 1));

						if (normals.ptr != nullptr){
							tri.n0 = Vec3Transform(matWorld,
								Vector4(((float*)normals.ptr)[idx0 * normals.stride],
								((float*)normals.ptr)[idx0 * normals.stride + 1],
								((float*)normals.ptr)[idx0 * normals.stride + 2], 1)) - o;
							tri.n1 = Vec3Transform(matWorld,
								Vector4(((float*)normals.ptr)[idx1 * normals.stride],
								((float*)normals.ptr)[idx1 * normals.stride + 1],
								((float*)normals.ptr)[idx1 * normals.stride + 2], 1)) - o;
							tri.n2 = Vec3Transform(matWorld,
								Vector4(((float*)normals.ptr)[idx2 * normals.stride],
								((float*)normals.ptr)[idx2 * normals.stride + 1],
								((float*)normals.ptr)[idx2 * normals.stride + 2], 1)) - o;
						}
						if (texcoords.ptr != nullptr){
							if (texcoords.size == 3){
								tri.t0 = Vector3(((float*)texcoords.ptr)[idx0 * texcoords.stride],
									((float*)texcoords.ptr)[idx0 * texcoords.stride + 1],
									((float*)texcoords.ptr)[idx0 * texcoords.stride + 2]);
								tri.t1 = Vector3(((float*)texcoords.ptr)[idx1 * texcoords.stride],
									((float*)texcoords.ptr)[idx1 * texcoords.stride + 1],
									((float*)texcoords.ptr)[idx1 * texcoords.stride + 2]);
								tri.t2 = Vector3(((float*)texcoords.ptr)[idx2 * texcoords.stride],
									((float*)texcoords.ptr)[idx2 * texcoords.stride + 1],
									((float*)texcoords.ptr)[idx2 * texcoords.stride + 2]);
							}
							else if (texcoords.size == 2){
								tri.t0 = Vector3(((float*)texcoords.ptr)[idx0 * texcoords.stride],
									((float*)texcoords.ptr)[idx0 * texcoords.stride + 1], 0);
								tri.t1 = Vector3(((float*)texcoords.ptr)[idx1 * texcoords.stride],
									((float*)texcoords.ptr)[idx1 * texcoords.stride + 1], 0);
								tri.t2 = Vector3(((float*)texcoords.ptr)[idx2 * texcoords.stride],
									((float*)texcoords.ptr)[idx2 * texcoords.stride + 1], 0);
							}
						}
						//Primitive_ptr prim(new Triangle(currentMaterial, tri));
						prims.push_back(Primitive_ptr(new Triangle(currentMaterial, tri)));
					}
				}
			}
			RenderSystemRayTrace::RenderSystemRayTrace(HWND hWnd, uint width, uint height) : RenderSystemOpenGL(hWnd, width, height){
				tracer = new RayTracer;
				glClearColor(0, 0, 0, 1);
				Material m;
				m.diffuse = gray; m.kd = 0.6;

			}

			RenderSystemRayTrace::~RenderSystemRayTrace(){
				delete tracer;
			}
			void RenderSystemRayTrace::Resize(int _width, int _height){
				RenderSystem::width = _width;
				RenderSystem::height = _height;
				camera->SetAspect(_width / float(_height));
			}

			void RenderSystemRayTrace::SetView(const PerspectiveCamera& camera){
				tracer->SetView(camera);
				RenderSystemOpenGL::SetView(camera);
			}
			void RenderSystemRayTrace::SetColor(const Color& c){
				tracer->SetColor(c);
			}

			void RenderSystemRayTrace::SetMaterial(const Material& m){
				tracer->SetMaterial(m);
			}

			void RenderSystemRayTrace::SetTransform(TransformType type, const Matrix &matWorld){

				switch (type){
				case SP_VIEW:

					tracer->SetMatrix(matWorld);
					break;
				}
			}
			void RenderSystemRayTrace::Flush(){
				vector<ABGRColor> image; // low-endian byte order, see http://stackoverflow.com/questions/7786187/opengl-texture-upload-unsigned-byte-vs-unsigned-int-8-8-8-8

				tracer->SetView(*camera);
				tracer->Render<ABGRColor>(image, RenderSystem::width, RenderSystem::height);

				glClear(GL_COLOR_BUFFER_BIT);
				glDrawPixels(RenderSystem::width, RenderSystem::height, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, image.data());
				glFlush();
			}

			void RenderSystemRayTrace::DrawMesh(const Mesh& mesh){}
			void RenderSystemRayTrace::DrawSolidMesh(const Mesh& mesh){
				//add all triangles to vector<Primitive*> prims
				tracer->SetVertexPointer(mesh.GetPositionSize(), mesh.GetCompiledVertexSize(), mesh.GetCompiledVertices());
				tracer->SetNormalPointer(mesh.GetCompiledVertexSize(), mesh.GetCompiledVertices() + mesh.GetCompiledNormalOffset());
				tracer->SetTexCoordPointer(mesh.GetTexCoordSize(), mesh.GetCompiledVertexSize(), mesh.GetCompiledVertices() + mesh.GetCompiledTexCoordOffset());

				tracer->DrawElements(SP_TRIANGLES, mesh.GetCompiledIndexCount(), 4, mesh.GetCompiledIndices());

			}
			void RenderSystemRayTrace::DrawWiredMesh(const Mesh& mesh){}
			void RenderSystemRayTrace::DrawScene(Scene&){}
			void RenderSystemRayTrace::DrawSphere(float r){
				tracer->DrawSphere(r);
			}
			void RenderSystemRayTrace::DrawCube(float a, float b, float c){}
			void RenderSystemRayTrace::DrawPlane(Vector3 normal){
				tracer->DrawPlane(normal);
			}
		}
	}
}