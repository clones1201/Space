#ifndef __BASIC_H__
#define __BASIC_H__

#include "Prerequisites.h"

namespace space{

	class Uncopyable{
	protected:
		Uncopyable(){}
		~Uncopyable(){}
	private:
		Uncopyable(const Uncopyable&);
		Uncopyable& operator=(const Uncopyable&);
	};

	class Interface : private Uncopyable{
	public:
		virtual ~Interface(){}
	};

#define __INTERFACE(i) virtual public i

	class Object{
	public:
		virtual ~Object(){}
	};

	namespace Pattern{

		template< typename T>
		class Singleton : private Uncopyable{
		protected:
			static unique_ptr<T> instance;
			Singleton(){}
		public:
			static T* Instance(){
				if (instance == nullptr){
					instance = unique_ptr<T>(new T);
				}
				return instance.get();
			}
		};
		template <typename T>
		unique_ptr<T> Singleton<T>::instance;
	}

	namespace util{
		
		//anyway, it sucks. I should re-write it later...
		template< typename T>
		class BinaryTreeNode{
		public:
			typedef shared_ptr< BinaryTreeNode<T> > Ptr;
		protected:
			T elem;
			Ptr leftChild, rightChild;
		public:

			BinaryTreeNode(){}
			BinaryTreeNode(const T &elem){ BinaryTreeNode::elem = T(elem); }
			BinaryTreeNode(Ptr left,Ptr right, const T &elem);

			virtual void SetChildren(Ptr left,Ptr right);

			void SetElem(const T &elem){
				BinaryTreeNode::elem = T(elem);
			}
			T GetElem()const{
				return elem;
			}

			Ptr GetLeft() const{
				return leftChild;
			}
			Ptr GetRight() const{
				return rightChild;
			}

			virtual bool isLeaf() const{
				return false;
			}
		};

		template< typename T>
		class BinaryTreeLeaf : public BinaryTreeNode<T>{
		protected:
			virtual void SetChildren(BinaryTreeNode<T>::Ptr left, BinaryTreeNode<T>::Ptr right){}
		public:
			typedef shared_ptr< BinaryTreeLeaf<T> > Ptr;

			BinaryTreeLeaf(){}

			BinaryTreeLeaf(const T& elem){
				BinaryTreeLeaf::elem = T(elem);
			}
			virtual bool isLeaf() const{
				return true;
			}
		};

		template< typename T>
		BinaryTreeNode<T>::BinaryTreeNode(Ptr left,Ptr right, const T &elem){
			leftChild = left; rightChild = right;
			BinaryTreeNode::elem = T(elem);
		}

		template< typename T>
		void BinaryTreeNode<T>::SetChildren(Ptr left, Ptr right){
			leftChild = left; rightChild = right;
		}
	}

	namespace math{

		const float Pi = 3.141592654f;

		struct uchar3{
			uchar x, y, z;
		};
		struct uchar4{
			uchar x, y, z, w;
		};

		struct uint2{
			uint x, y;
		};

		struct int2{
			int x, y;
		};

		typedef struct _float2{
			float x, y;
			explicit _float2(float _x = 0, float _y = 0) :x(_x), y(_y){}
		}float2, Vector2;

		typedef shared_ptr< _float2 > float2_ptr, Vector2_ptr;

		inline float2 Vec2Add(const float2& p1, const float2& p2){
			return float2(p1.x + p2.x, p1.y + p2.y);
		}

		inline float2 Vec2Subtract(const float2& p1, const float2& p2){
			return float2(p1.x - p2.x, p1.y - p2.y);
		}

		/*inline float2 Vec2Cross(const float2& p1, const float2& p2){
		return float2(
		(p1.y * p2.z - p1.z * p2.y),
		(p1.z * p2.x - p1.x * p2.z),
		p1.x * p2.y - p1.y * p2.x);
		}*/

		inline float Vec2Dot(const float2& p1, const float2& p2){
			return (p1.x * p2.x + p1.y * p2.y);
		}

		inline float2 Vec2Divede(const float2& p1, float p2){
			if (p2 != 0)
				return float2(p1.x / p2, p1.y / p2);
			else
				return float2(p1.x, p1.y);
		}

		inline float Vec2Length(const float2& p){
			return sqrtf(p.x * p.x + p.y * p.y);
		}

		inline float2 operator+(const float2& p1, const float2& p2){
			return Vec2Add(p1, p2);
		}
		inline float2 operator-(const float2& p1, const float2& p2){
			return Vec2Subtract(p1, p2);
		}
		inline float2 operator*(const float2& p1, float p2){
			return float2(p1.x * p2, p1.y * p2);
		}

		inline float2 operator*(float p2, const float2& p1){
			return float2(p1.x * p2, p1.y * p2);
		}

		inline float2 operator/(const float2& p1, float p2){
			return Vec2Divede(p1, p2);
		}
		inline float2 operator-(const float2& p){
			return float2(-p.x, -p.y);
		}

		inline float2 operator+=(float2& p1, const float2& p2){
			float2 result;
			result.x = p1.x + p2.x; result.y = p1.y + p2.y; 
			return result;
		}
		inline float2 operator-=(float2& p1, const float2& p2){
			float2 result;
			result.x = p1.x - p2.x; result.y = p1.y - p2.y;
			return result;
		}
		inline float2 operator*=(float2& p1, float p2){
			float2 result;
			result.x = p1.x * p2; result.y = p1.y * p2;
			return result;
		}
		inline float2 operator/=(float2& p1, float p2){
			float2 result;
			result.x = p1.x / p2; result.y = p1.y / p2;
			return result;
		}

		inline bool operator== (const float2 &p1,const float2&p2){
			return (p1.x == p2.x && p1.y == p2.y);
		}
		inline bool operator!=(const float2 &p1, const float2 &p2){
			return !(p1 == p2);
		}

		inline float2 Vec2Normalize(const float2& p){
			return float2(p / Vec2Length(p));
		}

		typedef struct _float3{
			float x, y, z;
			explicit _float3(float x = 0, float y = 0, float z = 0) :x(x), y(y), z(z){}
			explicit _float3(const float2& vec2) :x(vec2.x), y(vec2.y), z(0){}
		}float3, Vector3;
		typedef shared_ptr< _float3 > float3_ptr, Vector3_ptr;

		typedef struct _float4{
			float x, y, z, w;
			explicit _float4(float x = 0, float y = 0, float z = 0, float w = 0) :x(x), y(y), z(z), w(w){}
			explicit _float4(const Vector3& vec) :x(vec.x), y(vec.y), z(vec.z), w(1.0f){}
			explicit _float4(const float2& vec2) :x(vec2.x), y(vec2.y), z(0), w(1.0f){}
		}float4, Vector4;
		typedef shared_ptr<_float4> float4_ptr, Vector4_ptr;

		typedef struct _Matrix4{
			float m[16];

			inline void set(const float src[16])
			{
				m[0] = src[0];  m[1] = src[1];  m[2] = src[2];  m[3] = src[3];
				m[4] = src[4];  m[5] = src[5];  m[6] = src[6];  m[7] = src[7];
				m[8] = src[8];  m[9] = src[9];  m[10] = src[10]; m[11] = src[11];
				m[12] = src[12]; m[13] = src[13]; m[14] = src[14]; m[15] = src[15];
			}

			inline void set(float xx, float xy, float xz, float xw,
				float yx, float yy, float yz, float yw,
				float zx, float zy, float zz, float zw,
				float wx, float wy, float wz, float ww)
			{
				m[0] = xx;  m[1] = xy;  m[2] = xz;  m[3] = xw;
				m[4] = yx;  m[5] = yy;  m[6] = yz;  m[7] = yw;
				m[8] = zx;  m[9] = zy;  m[10] = zz;  m[11] = zw;
				m[12] = wx;  m[13] = wy;  m[14] = wz;  m[15] = ww;
			}

			explicit _Matrix4(const float src[16]){
				set(src);
			}
			explicit _Matrix4(float xx = 1, float xy = 0, float xz = 0, float xw = 0,
				float yx = 0, float yy = 1, float yz = 0, float yw = 0,
				float zx = 0, float zy = 0, float zz = 1, float zw = 0,
				float wx = 0, float wy = 0, float wz = 0, float ww = 1){
				set(xx, xy, xz, xw, yx, yy, yz, yw, zx, zy, zz, zw, wx, wy, wz, ww);
			}

			_Matrix4(const Vector4& v0, const Vector4& v1, const Vector4& v2, const Vector4& v3){
				set(v0.x, v1.x, v2.x, v3.x, v0.y, v1.y, v2.y, v3.y, v0.z, v1.z, v2.z, v3.z, v0.w, v1.w, v2.w, v3.w);
			}
			float& operator[](int idx){
				return m[idx];
			}
		}Matrix, Matrix4;
		typedef shared_ptr<_Matrix4> Matrix_ptr, Matrix_ptr;

		inline float ToRadian(float p){
			return p * (Pi / 180);
		}

		inline float3 Vec3Add(const float3& p1, const float3& p2){
			return float3(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
		}

		inline float3 Vec3Subtract(const float3& p1, const float3& p2){
			return float3(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
		}

		inline float3 Vec3Cross(const float3& p1, const float3& p2){
			return float3(
				(p1.y * p2.z - p1.z * p2.y),
				(p1.z * p2.x - p1.x * p2.z),
				p1.x * p2.y - p1.y * p2.x);
		}

		inline float Vec3Dot(const float3& p1, const float3& p2){
			return (p1.x * p2.x + p1.y * p2.y + p1.z * p2.z);
		}

		inline float3 Vec3Divede(const float3& p1, float p2){
			if (p2 != 0)
				return float3(p1.x / p2, p1.y / p2, p1.z / p2);
			else
				return float3(p1.x, p1.y, p1.z);
		}

		inline float Vec3Length(const float3& p){
			return sqrtf(p.x * p.x + p.y * p.y + p.z * p.z);
		}

		inline float3 operator+(const float3& p1, const float3& p2){
			return Vec3Add(p1, p2);
		}
		inline float3 operator-(const float3& p1, const float3& p2){
			return Vec3Subtract(p1, p2);
		}
		inline float3 operator*(const float3& p1, float p2){
			return float3(p1.x * p2, p1.y * p2, p1.z * p2);
		}

		inline float3 operator*(float p2, const float3& p1){
			return float3(p1.x * p2, p1.y * p2, p1.z * p2);
		}

		inline float3 operator/(const float3& p1, float p2){
			return Vec3Divede(p1, p2);
		}
		inline float3 operator-(const float3& p){
			return float3(-p.x, -p.y, -p.z);
		}

		inline float3 operator+=(const float3& p1, const float3& p2){
			float3 result;
			result.x = p1.x + p2.x; result.y = p1.y + p2.y; result.z = p1.z + p2.z;
			return p1;
		}
		inline float3 operator-=(const float3& p1, const float3& p2){
			float3 result;
			result.x = p1.x - p2.x; result.y = p1.y - p2.y; result.z = p1.z - p2.z;
			return p1;
		}
		inline float3 operator*=(const float3& p1, float p2){
			float3 result;
			result.x = p1.x * p2; result.y = p1.y * p2; result.z = p1.z * p2;
			return p1;
		}
		inline float3 operator/=(const float3& p1, float p2){
			float3 result;
			result.x = p1.x / p2; result.y = p1.y / p2; result.z = p1.z / p2;
			return p1;
		}

		inline bool operator== (const float3 &p1,const float3&p2){
			return (p1.x == p2.x && p1.y == p2.y && p1.z == p2.z);
		}

		inline bool operator!=(const float3 &p1, const float3&p2){
			return !(p1 == p2);
		}

		inline float Vec4Length(const float4& p){
			return sqrtf(p.x * p.x + p.y * p.y + p.z * p.z + p.w * p.w);
		}
		inline float3 Vec3Normalize(const float3& p){
			return float3(p / Vec3Length(p));
		}
		inline float3 Vec3Transform(const float4& p, const Matrix& mat){
			return float3(p.x * mat.m[0] + p.y * mat.m[4] + p.z * mat.m[8] + p.w * mat.m[12],
				p.x * mat.m[1] + p.y * mat.m[5] + p.z * mat.m[9] + p.w * mat.m[13],
				p.x * mat.m[2] + p.y * mat.m[6] + p.z * mat.m[10] + p.w * mat.m[14]);
		}
		inline float3 Vec3Transform(const Matrix& mat, const float4& p){
			return float3(p.x * mat.m[0] + p.y * mat.m[1] + p.z * mat.m[2] + p.w * mat.m[3],
				p.x * mat.m[4] + p.y * mat.m[5] + p.z * mat.m[6] + p.w * mat.m[7],
				p.x * mat.m[8] + p.y * mat.m[9] + p.z * mat.m[10] + p.w * mat.m[11]);
		}
		inline Vector4 Vec4Transform(const Vector4& p, const Matrix& mat){
			return float4(p.x * mat.m[0] + p.y * mat.m[4] + p.z * mat.m[8] + p.w * mat.m[12],
				p.x * mat.m[1] + p.y * mat.m[5] + p.z * mat.m[9] + p.w * mat.m[13],
				p.x * mat.m[2] + p.y * mat.m[6] + p.z * mat.m[10] + p.w * mat.m[14],
				p.x * mat.m[3] + p.y * mat.m[7] + p.z * mat.m[11] + p.w * mat.m[15]);
		}
		inline Vector4 Vec4Transform(const Matrix& mat, const Vector4& p){
			return float4(p.x * mat.m[0] + p.y * mat.m[1] + p.z * mat.m[2] + p.w * mat.m[3],
				p.x * mat.m[4] + p.y * mat.m[5] + p.z * mat.m[6] + p.w * mat.m[7],
				p.x * mat.m[8] + p.y * mat.m[9] + p.z * mat.m[10] + p.w * mat.m[11],
				p.x * mat.m[12] + p.y * mat.m[13] + p.z * mat.m[14] + p.w * mat.m[15]);
		}

		inline Matrix MatrixIdentity(){
			return *new Matrix;
		}
		inline Matrix MatrixTranslation(float dX, float dY, float dZ){
			return Matrix(
				1, 0, 0, dX,
				0, 1, 0, dY,
				0, 0, 1, dZ,
				0, 0, 0, 1);
		}
		inline Matrix MatrixScale(float sX, float sY, float sZ){
			return Matrix(
				sX, 0, 0, 0,
				0, sY, 0, 0,
				0, 0, sZ, 0,
				0, 0, 0, 1);
		}

		inline Matrix MatrixRotationAxis(const Vector3& axis, const float& angle){
			float x = axis.x, y = axis.y, z = axis.z;
			float c = cosf(ToRadian(angle)), s = sinf(ToRadian(angle));
			return Matrix(
				x*x*(1 - c) + c, x*y*(1 - c) - z*s, x*z*(1 - c) + y*s, 0,
				x*y*(1 - c) + z*s, y*y*(1 - c) + c, y*z*(1 - c) - x*s, 0,
				x*z*(1 - c) - y*s, y*z*(1 - c) + x*s, z*z*(1 - c) + c, 0,
				0, 0, 0, 1
				);
		}
		inline Matrix MatrixRotationX(const float& angle){
			return MatrixRotationAxis(Vector3(1, 0, 0), angle);
		}
		inline Matrix MatrixRotationY(const float& angle){
			return MatrixRotationAxis(Vector3(0, 1, 0), angle);
		}
		inline Matrix MatrixRotationZ(const float& angle){
			return MatrixRotationAxis(Vector3(0, 0, 1), angle);
		}

		inline Matrix MatrixMultiply(const Matrix& p1, const Matrix& p2){
			float *m = (float*)&p1, *n = (float*)&p2;
			return Matrix(
				m[0] * n[0] + m[1] * n[4] + m[2] * n[8] + m[3] * n[12],
				m[0] * n[1] + m[1] * n[5] + m[2] * n[9] + m[3] * n[13],
				m[0] * n[2] + m[1] * n[6] + m[2] * n[10] + m[3] * n[14],
				m[0] * n[3] + m[1] * n[7] + m[2] * n[11] + m[3] * n[15],

				m[4] * n[0] + m[5] * n[4] + m[6] * n[8] + m[7] * n[12],
				m[4] * n[1] + m[5] * n[5] + m[6] * n[9] + m[7] * n[13],
				m[4] * n[2] + m[5] * n[6] + m[6] * n[10] + m[7] * n[14],
				m[4] * n[3] + m[5] * n[7] + m[6] * n[11] + m[7] * n[15],

				m[8] * n[0] + m[9] * n[4] + m[10] * n[8] + m[11] * n[12],
				m[8] * n[1] + m[9] * n[5] + m[10] * n[9] + m[11] * n[13],
				m[8] * n[2] + m[9] * n[6] + m[10] * n[10] + m[11] * n[14],
				m[8] * n[3] + m[9] * n[7] + m[10] * n[11] + m[11] * n[15],

				m[12] * n[0] + m[13] * n[4] + m[14] * n[8] + m[15] * n[12],
				m[12] * n[1] + m[13] * n[5] + m[14] * n[9] + m[15] * n[13],
				m[12] * n[2] + m[13] * n[6] + m[14] * n[10] + m[15] * n[14],
				m[12] * n[3] + m[13] * n[7] + m[14] * n[11] + m[15] * n[15]);
		}

		inline Matrix operator*(const Matrix& m1, const Matrix& m2){
			return MatrixMultiply(m1, m2);
		}

		inline Matrix MatrixTranspose(const Matrix& mat)
		{
			const float *m = mat.m;
			Matrix result;
			result.m[0] = m[0];   result.m[1] = m[4];   result.m[2] = m[8];   result.m[3] = m[12];
			result.m[4] = m[1];   result.m[5] = m[5];   result.m[6] = m[9];   result.m[7] = m[13];
			result.m[8] = m[2];   result.m[9] = m[6];   result.m[10] = m[10];  result.m[11] = m[14];
			result.m[12] = m[3];   result.m[13] = m[7];   result.m[14] = m[11];  result.m[15] = m[15];
			return result;
		}

		inline Matrix4 MatrixFrustum(float l, float r, float b, float t, float n, float f)
		{
			Matrix4 mat;
			mat[0] = 2 * n / (r - l);
			mat[2] = (r + l) / (r - l);
			mat[5] = 2 * n / (t - b);
			mat[6] = (t + b) / (t - b);
			mat[10] = -(f + n) / (f - n);
			mat[11] = -(2 * f * n) / (f - n);
			mat[14] = -1;
			mat[15] = 0;
			return mat;
		}
		inline Matrix4 MatrixPerspective(float fovY, float aspect, float front, float back)
		{
			float tangent = tanf(ToRadian(fovY / 2)); // tangent of half fovY
			float height = front * tangent;         // half height of near plane
			float width = height * aspect;          // half width of near plane

			// params: left, right, bottom, top, near, far
			return MatrixFrustum(-width, width, -height, height, front, back);
		}

		inline float MatrixDeterminant(const Matrix&){
			float result = 0;

			return result;
		}

		inline Matrix MatrixInverse(const Matrix& mat){
			//float determ = MatrixDeterminant(m);
			Matrix inv;
			Matrix ori(mat);

			if (ori.m[0] == 0){
				uint i;
				for (i = 1; i < 3; i++){
					if (ori.m[i * 4] != 0) break;
				}
				Vector4 temp(ori.m[0], ori.m[1], ori.m[2], ori.m[3]);
				Vector4 tempInv(inv.m[0], inv.m[1], inv.m[2], inv.m[3]);
				ori.m[0] = ori.m[i * 4]; inv.m[0] = inv.m[i * 4];
				ori.m[1] = ori.m[i * 4 + 1]; inv.m[1] = inv.m[i * 4 + 1];
				ori.m[2] = ori.m[i * 4 + 2]; inv.m[2] = inv.m[i * 4 + 2];
				ori.m[3] = ori.m[i * 4 + 3]; inv.m[3] = inv.m[i * 4 + 3];

				ori.m[i * 4 + 0] = temp.x; inv.m[i * 4 + 0] = tempInv.x;
				ori.m[i * 4 + 1] = temp.y; inv.m[i * 4 + 1] = tempInv.y;
				ori.m[i * 4 + 2] = temp.z; inv.m[i * 4 + 2] = tempInv.z;
				ori.m[i * 4 + 3] = temp.w; inv.m[i * 4 + 3] = tempInv.w;
			}
			// upper right triangle
			float d = ori.m[4] / ori.m[0];
			ori.m[4] = 0; inv.m[4] -= inv.m[0] * d;
			ori.m[5] -= ori.m[1] * d; inv.m[5] -= inv.m[1] * d;
			ori.m[6] -= ori.m[2] * d; inv.m[6] -= inv.m[2] * d;
			ori.m[7] -= ori.m[3] * d; inv.m[7] -= inv.m[3] * d;
			d = ori.m[8] / ori.m[0];
			ori.m[8] = 0; inv.m[8] -= inv.m[0] * d;
			ori.m[9] -= ori.m[1] * d; inv.m[9] -= inv.m[1] * d;
			ori.m[10] -= ori.m[2] * d; inv.m[10] -= inv.m[2] * d;
			ori.m[11] -= ori.m[3] * d; inv.m[11] -= inv.m[3] * d;
			d = ori.m[12] / ori.m[0];
			ori.m[12] = 0; inv.m[12] -= inv.m[0] * d;
			ori.m[13] -= ori.m[1] * d; inv.m[13] -= inv.m[1] * d;
			ori.m[14] -= ori.m[2] * d; inv.m[14] -= inv.m[2] * d;
			ori.m[15] -= ori.m[3] * d; inv.m[15] -= inv.m[3] * d;

			if (ori.m[5] == 0){
				uint i;
				for (i = 2; i < 3; i++){
					if (ori.m[i * 4] != 0) break;
				}
				Vector4 temp(ori.m[4], ori.m[5], ori.m[6], ori.m[7]);
				Vector4 tempInv(inv.m[4], inv.m[5], inv.m[6], inv.m[7]);
				ori.m[4] = ori.m[i * 4]; inv.m[4] = inv.m[i * 4];
				ori.m[5] = ori.m[i * 4 + 1]; inv.m[5] = inv.m[i * 4 + 1];
				ori.m[6] = ori.m[i * 4 + 2]; inv.m[6] = inv.m[i * 4 + 2];
				ori.m[7] = ori.m[i * 4 + 3]; inv.m[7] = inv.m[i * 4 + 3];

				ori.m[i * 4 + 0] = temp.x; inv.m[i * 4 + 0] = tempInv.x;
				ori.m[i * 4 + 1] = temp.y; inv.m[i * 4 + 1] = tempInv.y;
				ori.m[i * 4 + 2] = temp.z; inv.m[i * 4 + 2] = tempInv.z;
				ori.m[i * 4 + 3] = temp.w; inv.m[i * 4 + 3] = tempInv.w;
			}
			d = ori.m[9] / ori.m[5];
			ori.m[8] = 0; inv.m[8] -= inv.m[4] * d;
			ori.m[9] = 0; inv.m[9] -= inv.m[5] * d;
			ori.m[10] -= ori.m[6] * d; inv.m[10] -= inv.m[6] * d;
			ori.m[11] -= ori.m[7] * d; inv.m[11] -= inv.m[7] * d;
			d = ori.m[13] / ori.m[5];
			ori.m[12] = 0; inv.m[12] -= inv.m[4] * d;
			ori.m[13] = 0; inv.m[13] -= inv.m[5] * d;
			ori.m[14] -= ori.m[6] * d; inv.m[14] -= inv.m[6] * d;
			ori.m[15] -= ori.m[7] * d; inv.m[15] -= inv.m[7] * d;

			if (ori.m[10] == 0){
				uint i;
				for (i = 1; i < 3; i++){
					if (ori.m[i * 4] != 0) break;
				}
				Vector4 temp(ori.m[8], ori.m[9], ori.m[10], ori.m[11]);
				Vector4 tempInv(inv.m[8], inv.m[9], inv.m[10], inv.m[11]);
				ori.m[8] = ori.m[i * 4]; inv.m[8] = inv.m[i * 4];
				ori.m[9] = ori.m[i * 4 + 1]; inv.m[9] = inv.m[i * 4 + 1];
				ori.m[10] = ori.m[i * 4 + 2]; inv.m[10] = inv.m[i * 4 + 2];
				ori.m[11] = ori.m[i * 4 + 3]; inv.m[11] = inv.m[i * 4 + 3];

				ori.m[i * 4 + 0] = temp.x; inv.m[i * 4 + 0] = tempInv.x;
				ori.m[i * 4 + 1] = temp.y; inv.m[i * 4 + 1] = tempInv.y;
				ori.m[i * 4 + 2] = temp.z; inv.m[i * 4 + 2] = tempInv.z;
				ori.m[i * 4 + 3] = temp.w; inv.m[i * 4 + 3] = tempInv.w;
			}
			d = ori.m[14] / ori.m[10];
			ori.m[12] = 0; inv.m[12] -= inv.m[8] * d;
			ori.m[13] = 0; inv.m[13] -= inv.m[9] * d;
			ori.m[14] = 0; inv.m[14] -= inv.m[10] * d;
			ori.m[15] -= ori.m[11] * d; inv.m[15] -= inv.m[11] * d;

			//
			//if (ori.m[15] == 0) return inv;
			d = ori.m[11] / ori.m[15];
			ori.m[8] -= ori.m[12] * d; inv.m[8] -= inv.m[12] * d;
			ori.m[9] -= ori.m[13] * d; inv.m[9] -= inv.m[13] * d;
			ori.m[10] -= ori.m[14] * d; inv.m[10] -= inv.m[14] * d;
			ori.m[11] = 0; inv.m[11] -= inv.m[15] * d;
			d = ori.m[7] / ori.m[15];
			ori.m[4] -= ori.m[12] * d; inv.m[4] -= inv.m[12] * d;
			ori.m[5] -= ori.m[13] * d; inv.m[5] -= inv.m[13] * d;
			ori.m[6] -= ori.m[14] * d; inv.m[6] -= inv.m[14] * d;
			ori.m[7] = 0; inv.m[7] -= inv.m[15] * d;
			d = ori.m[3] / ori.m[15];
			ori.m[0] -= ori.m[12] * d; inv.m[0] -= inv.m[12] * d;
			ori.m[1] -= ori.m[13] * d; inv.m[1] -= inv.m[13] * d;
			ori.m[2] -= ori.m[14] * d; inv.m[2] -= inv.m[14] * d;
			ori.m[3] = 0; inv.m[3] -= inv.m[15] * d;

			d = ori.m[6] / ori.m[10];
			ori.m[4] -= ori.m[8] * d; inv.m[4] -= inv.m[8] * d;
			ori.m[5] -= ori.m[9] * d; inv.m[5] -= inv.m[9] * d;
			ori.m[6] = 0; inv.m[6] -= inv.m[10] * d;
			ori.m[7] = 0; inv.m[7] -= inv.m[11] * d;
			d = ori.m[2] / ori.m[10];
			ori.m[0] -= ori.m[8] * d; inv.m[0] -= inv.m[8] * d;
			ori.m[1] -= ori.m[9] * d; inv.m[1] -= inv.m[9] * d;
			ori.m[2] = 0; inv.m[2] -= inv.m[10] * d;
			ori.m[3] = 0; inv.m[3] -= inv.m[11] * d;

			d = ori.m[1] / ori.m[5];
			ori.m[0] -= ori.m[4] * d; inv.m[0] -= inv.m[4] * d;
			ori.m[1] = 0; inv.m[1] -= inv.m[5] * d;
			ori.m[2] = 0; inv.m[2] -= inv.m[6] * d;
			ori.m[3] = 0; inv.m[3] -= inv.m[7] * d;

			//
			d = ori.m[0];
			ori.m[0] = ori.m[0] / d; inv.m[0] = inv.m[0] / d;
			ori.m[1] = ori.m[1] / d; inv.m[1] = inv.m[1] / d;
			ori.m[2] = ori.m[2] / d; inv.m[2] = inv.m[2] / d;
			ori.m[3] = ori.m[3] / d; inv.m[3] = inv.m[3] / d;
			d = ori.m[5];
			ori.m[4] = ori.m[4] / d; inv.m[4] = inv.m[4] / d;
			ori.m[5] = ori.m[5] / d; inv.m[5] = inv.m[5] / d;
			ori.m[6] = ori.m[6] / d; inv.m[6] = inv.m[6] / d;
			ori.m[7] = ori.m[7] / d; inv.m[7] = inv.m[7] / d;
			d = ori.m[10];
			ori.m[8] = ori.m[8] / d; inv.m[8] = inv.m[8] / d;
			ori.m[9] = ori.m[9] / d; inv.m[9] = inv.m[9] / d;
			ori.m[10] = ori.m[10] / d; inv.m[10] = inv.m[10] / d;
			ori.m[11] = ori.m[11] / d; inv.m[11] = inv.m[11] / d;
			d = ori.m[15];
			ori.m[12] = ori.m[12] / d; inv.m[12] = inv.m[12] / d;
			ori.m[13] = ori.m[13] / d; inv.m[13] = inv.m[13] / d;
			ori.m[14] = ori.m[14] / d; inv.m[14] = inv.m[14] / d;
			ori.m[15] = ori.m[15] / d; inv.m[15] = inv.m[15] / d;

			return inv;
		}
	}
	namespace graphic{

		typedef class _Color{
		public:
			float r, g, b, a;
			_Color() :r(0), g(0), b(0), a(1){}
			explicit _Color(float _r, float _g, float _b, float _a) :r(_r), g(_g), b(_b), a(_a){}
		}Color;
		typedef shared_ptr<_Color> Color_ptr;

		const Color black = Color(0, 0, 0, 1);
		const Color gray = Color(0.5, 0.5, 0.5, 1);
		const Color white = Color(1, 1, 1, 1);
		const Color red = Color(1, 0, 0, 1);
		const Color blue = Color(0, 0, 1, 1);
		const Color green = Color(0, 1, 0, 1);
		const Color yellow = Color(1, 1, 0, 1);
		const Color Magenta = Color(1, 0, 1, 1);
		const Color cyan = Color(0, 1, 1, 1);
		const Color orange = Color(1, 0.5f, 0, 1);
		const Color brown = Color(150.0f / 255, 75.0f / 255, 0.0f, 1);
		const Color navyblue = Color(0, 0, 0.5f, 1);
		const Color gold = Color(1.0f, 215 / 255.0f, 0.0f, 1);

		inline Color operator+(const Color &c1, const Color &c2){
			return Color(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b, c1.a + c2.a);
		}
		inline Color operator-(const Color &c1, const Color &c2){
			return Color(c1.r - c2.r, c1.g - c2.g, c1.b - c2.b, c1.a - c2.a);
		}
		inline Color operator*(const Color &c1, const Color &c2){
			return Color(c1.r * c2.r, c1.g * c2.g, c1.b * c2.b, c1.a * c2.a);
		}
		inline Color operator/(const Color &c1, float f){
			return Color(c1.r / f, c1.g / f, c1.b / f, c1.a / f);
		}
		inline Color operator*(const Color &c1, float f){
			return Color(c1.r * f, c1.g * f, c1.b * f, c1.a * f);
		}
		inline Color operator*(float f, const Color &c1){
			return c1 * f;
		}

		class RGBColor{
		public:
			uchar r, g, b;
			explicit RGBColor(Color &c){
				r = c.r > 1.0f ? 255 : c.r * 255;
				g = c.g > 1.0f ? 255 : c.g * 255;
				b = c.b > 1.0f ? 255 : c.b * 255;
			}
		};
		typedef shared_ptr<RGBColor> RGBColor_ptr;
		class RGBAColor{
		public:
			uchar r, g, b, a;
			explicit RGBAColor(Color &c){
				r = c.r > 1.0f ? 255 : c.r * 255;
				g = c.g > 1.0f ? 255 : c.g * 255;
				b = c.b > 1.0f ? 255 : c.b * 255;
				a = c.a > 1.0f ? 255 : c.a * 255;
			}
		};
		typedef shared_ptr<RGBAColor> RGBAColor_ptr;
		class ARGBColor{
		public:
			uchar a, r, g, b;
			explicit ARGBColor(Color &c){
				r = c.r > 1.0f ? 255 : c.r * 255;
				g = c.g > 1.0f ? 255 : c.g * 255;
				b = c.b > 1.0f ? 255 : c.b * 255;
				a = c.a > 1.0f ? 255 : c.a * 255;
			}
		};
		typedef shared_ptr<ARGBColor> ARGBColor_ptr;
		class ABGRColor{
		public:
			uchar a, b, g, r;
			explicit ABGRColor(Color &c){
				r = c.r > 1.0f ? 255 : c.r * 255;
				g = c.g > 1.0f ? 255 : c.g * 255;
				b = c.b > 1.0f ? 255 : c.b * 255;
				a = c.a > 1.0f ? 255 : c.a * 255;
			}
		};
		typedef shared_ptr<ABGRColor> ABGRColor_ptr;
		struct Ray{
			math::Vector3 ori, dir;
		};
		typedef shared_ptr<Ray> Ray_ptr;

		struct Rect{
			math::int2 a, b;
		};

		struct Triangle{
			math::Vector3 v0, v1, v2;
			math::Vector3 n0, n1, n2;
			math::Vector3 t0, t1, t2;

			// see: http://geomalgorithms.com/a06-_intersect-2.html
			bool Intersect(Ray ray, float& t) const{
				math::Vector3 u, v;
				u = v1 - v0;
				v = v2 - v0;
				math::Vector3 nt = Vec3Normalize(Vec3Cross(u, v));
				if (Vec3Dot(nt, n0) < 0){
					math::Vector3 temp = v;
					v = u; u = temp;
					nt = Vec3Cross(u, v);
				}

				float fm;
				if ((fm = Vec3Dot(nt, ray.dir)) == 0){ // parallel to plane
					return false;
				}
				float r1 = Vec3Dot(nt, v0 - ray.ori) / Vec3Dot(nt, ray.dir);
				if (r1 < 0){// wrong direction
					return false;
				}

				math::Vector3 pi = ray.ori + ray.dir * r1;
				math::Vector3 w = pi - v0;

				float uv = Vec3Dot(u, v);
				float wv = Vec3Dot(w, v);
				float vv = Vec3Dot(v, v);
				float wu = Vec3Dot(w, u);
				float uu = Vec3Dot(u, u);

				float si = (uv * wv - vv * wu) / (uv * uv - uu * vv);
				float ti = (uv * wu - uu * wv) / (uv * uv - uu * vv);

				if (si < 0 || ti < 0 || si + ti > 1){//outside of triangle
					return false;
				}

				t = r1;
				return true;
			}
		};
		typedef shared_ptr<Triangle> Triangle_ptr;

		class BBox {
		public:
			math::Vector3 bmin, bmax;
			BBox() :bmin(), bmax(){}
			bool Intersect(Ray ray, float&tmin, float&tmax) const{

				float ox = ray.ori.x; float oy = ray.ori.y; float oz = ray.ori.z;
				float dx = ray.dir.x; float dy = ray.dir.y; float dz = ray.dir.z;

				float tx_min, ty_min, tz_min;
				float tx_max, ty_max, tz_max;

				float a = 1.0 / dx;
				if (a >= 0){ tx_min = (bmin.x - ox) * a;	tx_max = (bmax.x - ox) * a; }
				else{ tx_min = (bmax.x - ox) * a;	tx_max = (bmin.x - ox) * a; }

				float b = 1.0 / dy;
				if (b >= 0){ ty_min = (bmin.y - oy) * b;	ty_max = (bmax.y - oy) * b; }
				else{ ty_min = (bmax.y - oy) * b;	ty_max = (bmin.y - oy) * b; }

				float c = 1.0 / dz;
				if (c >= 0){ tz_min = (bmin.z - oz) * c;	tz_max = (bmax.z - oz) * c; }
				else{ tz_min = (bmax.z - oz) * c;	tz_max = (bmin.z - oz) * c; }

				//find largest entering t value
				tmin = max(tz_min, max(tx_min, ty_min));
				//find smallest exiting t value
				tmax = min(tz_max, min(tx_max, ty_max));

				math::Vector3 min, max;
				max = ray.dir * tmax + ray.ori;
				min = ray.dir * tmin + ray.ori;

				return (tmin < tmax && tmax > 0.001);
			}
		};

		inline BBox operator+(const BBox&b1, const BBox& b2){
			BBox result;
			result.bmax.x = max(b1.bmax.x, b2.bmax.x);
			result.bmax.y = max(b1.bmax.y, b2.bmax.y);
			result.bmax.z = max(b1.bmax.z, b2.bmax.z);

			result.bmin.x = min(b1.bmin.x, b2.bmin.x);
			result.bmin.y = min(b1.bmin.y, b2.bmin.y);
			result.bmin.z = min(b1.bmin.z, b2.bmin.z);
			return result;
		}


		class Light{

		};

		typedef std::vector<Light*> LightList;

		class Material{
		public:
			Color ambient;
			Color diffuse;
			Color specular;
			float ka, kd, ks;
			uint n;
			float reflect,refract,greflect;
			Material() :ka(0), kd(0), ks(0), n(1), reflect(0){}
		};
		typedef shared_ptr<Material> Material_ptr;

		class Sampler;
		typedef shared_ptr<Sampler> Sampler_ptr;

		class Sample : public Pattern::Singleton<Sample>{
		private:
			Sampler_ptr sampler;
		protected:
			Sample();
			friend class Pattern::Singleton<Sample>;
		public:
			math::Vector3 HemiSphere(math::Vector3 normal, math::Vector3 pos, float r, uint exp);
			math::Vector3 Sphere(math::Vector3 normal, math::Vector3 pos, float r);
			math::Vector3 Square(math::Vector3 normal, math::Vector3 pos, float a, float b);
		};
	}
}

#endif