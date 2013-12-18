#include "basic.h"
#include <ctime>

namespace space{
	using namespace math;
	namespace graphic{
		
		class Sampler/* : public Object*/{
		protected:
			vector<Vector2> samples;
			uint idx;
			void Init(){
				srand(clock());
				for (uint i = 0; i < samples.size(); i++){
					samples[i] = Vector2(rand() / double(RAND_MAX), rand() / double(RAND_MAX));
				}
			}
		public:
			Sampler(uint size = 1000):idx(0){
				samples.resize(size);
				Init();
			}
			Vector2 GetSample(){
				Vector2 result = samples[idx];
				idx++;
				if (idx > 1000){
					Init();
					idx = 0;
				}
				return result;
			}
		};

		Sample::Sample(){
			sampler = Sampler_ptr(new Sampler(1000));
		}

		Matrix GetTransformMatrix(Vector3 normal, Vector3 pos, float a, float b, float c){
			Matrix scale = MatrixScale(a,b,c);

			Vector3 w = Vec3Normalize(normal); Vector3 u;
			if (Vec3Dot(w, Vector3(0, 0, 1)) < 0.9){
				u = Vec3Normalize(Vec3Cross(w, Vector3(0, 0, 1)));
			}
			else if (Vec3Dot(w, Vector3(0, 1, 0)) < 0.9){
				u = Vec3Normalize(Vec3Cross(w, Vector3(0, 1, 0)));
			}
			else{
				u = Vec3Normalize(Vec3Cross(w, Vector3(1, 0, 0)));
			}
			Vector3 v = Vec3Normalize(Vec3Cross(w, u));
			Matrix rot = MatrixMultiply(scale,
				MatrixMultiply(
				Matrix(Vector4(u), Vector4(v), Vector4(w), Vector4(0, 0, 0, 1)),
				Matrix()
				));

			return MatrixMultiply(rot, MatrixTranslation(pos.x, pos.y, pos.z));
		}

		Vector3 Sample::HemiSphere(Vector3 normal, Vector3 pos, float r, uint exp){
			Vector2 pt = sampler->GetSample();
			float cosPhi = cosf(pt.x * 2.0 * Pi);
			float sinPhi = sinf(pt.x * 2.0 * Pi);
			float cosTheta = powf((1.0 - pt.y), 1.0 / (exp + 1));
			float sinTheta = sqrtf(1.0 - cosTheta * cosTheta);
			float pu = sinTheta * cosPhi;
			float pv = sinTheta * sinPhi;
			float pw = cosTheta;
			return  Vec3Transform(GetTransformMatrix(normal,pos,r,r,r), Vector4(pu, pv, pw));
		}

		Vector3 Sample::Sphere(Vector3 normal, Vector3 pos, float r){
			Vector2 pt = sampler->GetSample();
			float cosPhi = cosf(pt.x * 2.0 * Pi);
			float sinPhi = sinf(pt.x * 2.0 * Pi);
			float cosTheta = cosf( (1- pt.y) * Pi);
			float sinTheta = sqrtf(1.0 - cosTheta * cosTheta);
			float pu = sinTheta * cosPhi;
			float pv = sinTheta * sinPhi;
			float pw = cosTheta;
			return Vec3Transform(GetTransformMatrix(normal, pos, r, r, r), Vector4(pu, pv, pw));
		}

		Vector3 Sample::Square(Vector3 normal, Vector3 pos, float a, float b){
			Vector2 pt = sampler->GetSample(); 
			float pu = -1 + 2 * pt.x;
			float pv = -1 + 2 * pt.y;
			float pw = 0;
			return  Vec3Transform(GetTransformMatrix(normal, pos, a, b, 1), Vector4(pu, pv, pw));
		}
	}
}