#include "RayTracing.h"

namespace space{
	namespace graphic{
		namespace raytrace{
			using namespace math;

			const float kEpsilon = 0.001;

			class Sphere : public Primitive{
			private:
				float r;
				math::Vector3 centre;
			public:
				Sphere(Material_ptr m, Texture_ptr t, float r = 1, math::Vector3 centre = math::Vector3(0, 0, 0)) :
					Primitive(m, t), r(r), centre(centre){}
				bool Intersect(Ray ray, float&tmin, Shader& sd){
					float t;
					sd.hitAnObject = false;
					math::Vector3 temp = ray.ori - centre;
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

							sd.tex_ptr = texture;
							sd.u = sd.hitPos.z;
							sd.v = sd.hitPos.x;

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

							sd.tex_ptr = texture;
							sd.u = sd.hitPos.z;
							sd.v = sd.hitPos.x;

							sd.ray = ray;
							sd.hitAnObject = true;
							return true;
						}
					}
					return false;
				}

				virtual void CalculateBoundsBox(math::Vector3 &max, math::Vector3 &min)const{
					max.x = centre.x + r;
					max.y = centre.y + r;
					max.z = centre.z + r;

					min.x = centre.x - r;
					min.y = centre.y - r;
					min.z = centre.z - r;
				}
			};

			class Plane : public Primitive{
			private:
				math::Vector3 normal;
				math::Vector3 pos;
			public:
				Plane(Material_ptr m, Texture_ptr t, Vector3 normal = Vector3(0, 1, 0), Vector3 pos = Vector3(0, 0, 0)) :
					Primitive(m, t), normal(normal), pos(pos){}
				bool Intersect(Ray ray, float &t, Shader& sd){
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

					sd.tex_ptr = texture;
					sd.u = sd.hitPos.z;
					sd.v = sd.hitPos.x;

					sd.ray = ray;
					return true;
				}

				virtual void CalculateBoundsBox(math::Vector3 &max, math::Vector3 &min)const{

				}
			};

			class Triangle : public Primitive{
			private:
				graphic::Triangle tri;
			public:
				Triangle(Material_ptr m, Texture_ptr t, const graphic::Triangle &_tri) :
					Primitive(m, t), tri(_tri){}
				bool Intersect(Ray ray, float&t, Shader& sd){
					if (tri.Intersect(ray, t)){
						sd.hitAnObject = true;
						sd.normal = (tri.n0 + tri.n1 + tri.n2) / 3; // flat surface
						sd.hitPos = ray.ori + t * ray.dir + kEpsilon * sd.normal;
						sd.material = *material;
						sd.tex_ptr = texture;

						sd.u = (tri.t0.x + tri.t1.x + tri.t2.x) / 3;
						sd.v = (tri.t0.y + tri.t1.y + tri.t2.y) / 3;

						sd.ray = ray;
						return true;
					}
					return false;
				}

				virtual void CalculateBoundsBox(math::Vector3 &max, math::Vector3 &min)const{
					max.x = max(tri.v0.x, max(tri.v1.x, tri.v2.x));
					max.y = max(tri.v0.y, max(tri.v1.y, tri.v2.y));
					max.z = max(tri.v0.z, max(tri.v1.z, tri.v2.z));

					min.x = min(tri.v0.x, min(tri.v1.x, tri.v2.x));
					min.y = min(tri.v0.y, min(tri.v1.y, tri.v2.y));
					min.z = min(tri.v0.z, min(tri.v1.z, tri.v2.z));
				}
			};

			const uint maxDepth = 10;
			const uint maxObjects = 16;

			bool BSPNode::Intersect(Ray ray, float&t, Shader& sd){
				bool result;
				float tmax, tmin;
				if (result = elem.Intersect(ray, tmax, tmin)){

					leftChild;

				}
				return result;
			}
			bool BSPLeaf::Intersect(Ray ray, float&t, Shader& sd){
				bool result;
				float tmin;
				for (uint i = 0; i < prims.size(); i++){
					Shader sdt;
					if (result = prims[i]->Intersect(ray, tmin, sdt)){
						if (tmin < t){
							sd = sdt;
							t = tmin;
						}
					}
				}
				return result;
			}

			enum Axis{
				AxisX = 0,AxisY = 1,AxisZ = 2
			};

			void BuildTree(BSPNode::Ptr& node,const BBox& nodeBox,const vector<BBox>& bounds,const vector<Primitive_ptr>& prims, uint depth){
				node = (BSPNode::Ptr)(new BSPNode(nodeBox));
				
				/* reach threhold, create leaf */
				if (depth == 0 || bounds.size() <= maxObjects){
					node = (BSPNode::Ptr)(new BSPLeaf(nodeBox,prims));
					return;
				}
				vector<Primitive_ptr> leftprims, rightprims;
				vector<BBox> leftboxs, rightboxs;
				BBox leftBox, rightBox;
				BSPNode::Ptr left = nullptr, right = nullptr;

				/* find the longest axis */
				float dx, dy, dz;
				dx = nodeBox.bmax.x - nodeBox.bmin.x;
				dy = nodeBox.bmax.y - nodeBox.bmin.y;
				dz = nodeBox.bmax.z - nodeBox.bmin.z;
				Axis axis;
				if (dx > dy){
					axis = AxisX;
					if (dz > dx){
						axis = AxisZ;
					}
				}
				else{
					axis = AxisY;
					if (dz > dy){
						axis = AxisZ;
					}
				}

				auto piter = prims.begin();
				auto biter = bounds.begin();
				/* split the prims */
					leftBox = rightBox = nodeBox;
					((float*)&(leftBox.bmax))[axis] = (((float*)&(nodeBox.bmax))[axis] + ((float*)&(nodeBox.bmin))[axis]) / 2;
					((float*)&(rightBox.bmin))[axis] = (((float*)&(nodeBox.bmax))[axis] + ((float*)&(nodeBox.bmin))[axis]) / 2;
					for (; piter != prims.end() && biter != bounds.end();){
						BBox box;
						(*piter)->CalculateBoundsBox(box.bmax, box.bmin);
						if (((float*)&(box.bmax))[axis] < ((float*)&(leftBox.bmax))[axis]){
							leftboxs.push_back(box);
							leftprims.push_back(*piter);
						}
						else if (((float*)&(box.bmin))[axis] > ((float*)&(rightBox.bmin))[axis]){
							rightboxs.push_back(box);
							rightprims.push_back(*piter);
						}
						else{
							leftboxs.push_back(box);
							leftprims.push_back(*piter);
							rightboxs.push_back(box);
							rightprims.push_back(*piter);
						}
						piter++; biter++;
					}
					
				/* recurisive bulid the tree */
				BuildTree(left, leftBox,leftboxs,leftprims,depth-1);
				BuildTree(right, rightBox,rightboxs,rightprims,depth-1);
				
				node->SetChildren(left, right);
			}
  
			BSPNode::Ptr BuildBSPTree(const vector<Primitive_ptr>& prims,uint depth){
				vector<BBox> bounds;
				BBox allbox;
				/* Calculate all Bounding box */
				for (uint i = 0; i < prims.size(); i++){
					BBox box;
					prims[i]->CalculateBoundsBox(box.bmax, box.bmin);
					if (! (box.bmax == box.bmin)){
						bounds.push_back(box);
						allbox = allbox + box;
					}
				}
				
				BSPNode::Ptr root = nullptr;
				BuildTree(root, allbox, bounds,prims, 10);

				return root;
			}
			void CreatePrimitives(vector<Primitive_ptr>& prims, const Mesh& mesh){
				uint count = mesh.GetCompiledIndexCount();
				uint stride = mesh.GetCompiledVertexSize();
				const float *vertices = mesh.GetCompiledVertices();
				const uint* indices = mesh.GetCompiledIndices();
				
				Matrix matWorld;
				Vector3 o = Vec3Transform(matWorld, Vector4(0, 0, 0, 1));
				for (uint i = 0; i < count; i += 3){
					graphic::Triangle tri;
					uint idx0 = indices[i];
					uint idx1 = indices[i + 1];
					uint idx2 = indices[i + 2];

					tri.v0 = Vec3Transform(matWorld,
						Vector4(((float*)vertices)[idx0 * stride],
						((float*)vertices)[idx0 * stride + 1],
						((float*)vertices)[idx0 * stride + 2], 1));
					tri.v1 = Vec3Transform(matWorld,
						Vector4(((float*)vertices)[idx1 * stride],
						((float*)vertices)[idx1 * stride + 1],
						((float*)vertices)[idx1 * stride + 2], 1));
					tri.v2 = Vec3Transform(matWorld,
						Vector4(((float*)vertices)[idx2 * stride],
						((float*)vertices)[idx2 * stride + 1],
						((float*)vertices)[idx2 * stride + 2], 1));

					prims.push_back(Primitive_ptr(new raytrace::Triangle(nullptr, nullptr, tri)));
				}
			}
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
				Color ambient = ls * m.ambient * m.ka;
				Color lambertian, Glossy, Reflect;

				Color color;
				if (sd.tex_ptr.get() == nullptr){
					color = m.diffuse;
				}
				else{
					color = sd.tex_ptr->GetTextureColor(sd.u, sd.v);
				}

				if (ndotwi > 0.01){
					/* Lambertian */
					lambertian = ls * color * m.kd * ndotwi;
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
					if (prims[i]->Intersect(ray, t, sdt)){
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
						if (prims[i]->Intersect(shadowRay, t, Shader())){
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

					/*Glossy Reflection*/
					Color greflectColor;
					/*if (depth > 1){
						for (uint i = 0; i < 90; i++){
						Vector3 greflect = Sample::Instance()->HemiSphere(-wo + sd.normal * 2.0 * ndotwo, sd.hitPos, 1, sd.material.n) - sd.hitPos;
						Ray greflectRay;
						greflectRay.ori = sd.hitPos; greflectRay.dir = Vec3Normalize(greflect);
						greflectColor = greflectColor + 1 / 90.0 * Trace(prims, greflectRay, 1);
						}
						}*/
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

					/* Indirect illumination */
					/* using Monte Carlo
					/* not working good
					*/
					/*Color indirectIllumination;
					Shader isd;

					if (depth > 1){
					for (uint i = 0; i < 90; i++){
					Ray iRay;
					iRay.ori = sd.hitPos;
					iRay.dir = Vec3Normalize(Sample::Instance()->HemiSphere(sd.normal, sd.hitPos, 1, 0));
					indirectIllumination = indirectIllumination + (1 / 90.0) * Trace(prims, iRay, 1);
					}
					}*/

					return color/* + 0.5f * indirectIllumination */
						+ sd.material.reflect * Trace(prims, reflectRay, depth - 1)
						+ sd.material.greflect * greflectColor
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

			void RenderSystemRayTrace::RayTracer::SetTexture(Texture* tex){
				if (tex == nullptr){
					currentTexture = nullptr;
					return;
				}
				currentTexture = Texture_ptr(new Texture(*tex));
				if (textures.end() != find(textures.begin(), textures.end(), currentTexture)){
					textures.push_back(currentTexture);
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

				prims.push_back(Primitive_ptr(new Sphere(currentMaterial, currentTexture, r, centre)));
			}
			void RenderSystemRayTrace::RayTracer::DrawPlane(Vector3 normal){
				Vector3 position = Vec3Transform(matWorld, Vector4(0, 0, 0, 1));
				normal = Vec3Transform(matWorld, Vector4(normal)) - position;

				prims.push_back(Primitive_ptr(new Plane(currentMaterial, currentTexture, normal, position)));
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
						prims.push_back(Primitive_ptr(new Triangle(currentMaterial, currentTexture, tri)));
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

			void RenderSystemRayTrace::SetTexture(Texture* tex){
				tracer->SetTexture(tex);
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