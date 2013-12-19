#include "Radiosity.h"

namespace space{
	namespace graphic{
		namespace radiosity{
			void Process(Scene& scene){
				for (vector<Entity*>::iterator eIter = scene.GetEntitys().begin();
					eIter < scene.GetEntitys().end(); eIter++){
					
					Texture tex;

					vector<uchar> image;
					uint width = 1000, height = 1000;
					for (uint i = 0; i < width * height; i++){
						image.push_back(rand());
						image.push_back(rand());
						image.push_back(rand());
					}
					tex.Load(image,width,height);
					
					(*eIter)->SetTexture(tex);
				}
			}

			vector<uchar> SetupShadow(Mesh& mesh, uint width, uint height){
				vector<uchar> image;
				math::Vector3 LightPos(2, 3, 2);
				for (uint i = 0; i < width * height; i++){
					Ray ray, rayt; bool result = false; math::Vector3 normal, position;
					ray.ori = math::Vector3(-5 + i / width * (1.0 / width), -1, -5 + i % height * (1.0 / height));
					ray.dir = math::Vec3Normalize(LightPos - ray.ori);

					bool b1 = mesh.Intersect(ray, normal, position);

					if (b1){
						image.push_back(0);
						image.push_back(0);
						image.push_back(0);
					}
					else{
						image.push_back(255);
						image.push_back(255);
						image.push_back(255);
					}
				}
				vector<uchar> pngimage;
				lodepng::encode("shadowmap.png", image, width, height, LCT_RGB, 8);
				return image;
			}

			void DrawTextureMap(Mesh& mesh, uint width, uint height){
				vector<uchar> image;
				image.resize(width*height);

				math::Vector3 LightPos(2, 3, 2);
				auto Indices = mesh.GetCompiledIndices();
				for (uint i = 0; i < mesh.GetCompiledIndexCount(); i += 3){
					Triangle tri = mesh.GetTriangle(Indices[i], Indices[i + 1], Indices[i + 2]);

					math::Vector3 nt = Vec3Normalize(tri.n0 + tri.n1 + tri.n2);

					float radiosity = math::Vec3Dot(nt, LightPos - tri.v0);

				}

			}

			void GeneratePatch(Mesh& mesh){

			}
		}
	}
}