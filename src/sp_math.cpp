#include "basic.h"
#include "global.h"

namespace space{
	namespace math{

		float3 Vec3Add(const float3& p1, const float3& p2){
			return float3(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
		}
		float3 Vec3Subtract(const float3& p1, const float3& p2){
			return float3(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
		}
		float3 Vec3Cross(const float3& p1, const float3& p2){
			return float3(
				(p1.y * p2.z - p1.z * p2.y),
				(p1.z * p2.x - p1.x * p2.z),
				p1.x * p2.y - p1.y * p2.x);
		}
		float Vec3Dot(const float3& p1, const float3& p2){
			return (p1.x * p2.x + p1.y * p2.y + p1.z * p2.z);
		}
		float3 Vec3Divede(const float3& p1, float p2){
			if (p2 != 0)
				return float3(p1.x / p2, p1.y / p2, p1.z / p2);
			else
				return float3(p1.x, p1.y, p1.z);
		}
		float Vec3Length(const float3& p){
			return sqrtf(p.x * p.x + p.y * p.y + p.z * p.z);
		}
		float3 Vec3Normalize(const float3& p){
			return float3(p / Vec3Length(p));
		}
		float3 Vec3Transform(const float4& p, const Matrix& mat){
			return float3(p.x * mat.m[0] + p.y * mat.m[1] + p.z * mat.m[2] + p.w * mat.m[3],
				p.x * mat.m[4] + p.y * mat.m[5] + p.z * mat.m[6] + p.w * mat.m[7],
				p.x * mat.m[8] + p.y * mat.m[9] + p.z * mat.m[10] + p.w * mat.m[11]);
		}

		float3 operator+(const float3& p1, const float3& p2){
			return Vec3Add(p1, p2);
		}
		float3 operator-(const float3& p1, const float3& p2){
			return Vec3Subtract(p1, p2);
		}
		float3 operator*(const float3& p1, float p2){
			return float3(p1.x * p2, p1.y * p2, p1.z * p2);
		}

		float3 operator*(float p2,const float3& p1){
			return float3(p1.x * p2, p1.y * p2, p1.z * p2);
		}

		float3 operator/(const float3& p1, float p2){
			return Vec3Divede(p1, p2);
		}
		float3 operator-(const float3& p){
			return float3(-p.x, -p.y, -p.z);
		}

		float3& operator+=(float3& p1, const float3& p2){
			p1.x += p2.x; p1.y += p2.y; p1.z += p2.z;
			return p1;
		}
		float3& operator-=(float3& p1, const float3& p2){
			p1.x -= p2.x; p1.y -= p2.y; p1.z -= p2.z;
			return p1;
		}
		float3& operator*=(float3& p1, float p2){
			p1.x *= p2; p1.y *= p2; p1.z *= p2;
			return p1;
		}
		float3& operator/=(float3& p1, float p2){
			p1.x /= p2; p1.y /= p2; p1.z /= p2;
			return p1;
		}

		float Vec4Length(const float4& p){
			return sqrtf(p.x * p.x + p.y * p.y + p.z * p.z + p.w * p.w);
		}
		
		Matrix operator*(const Matrix& m1, const Matrix& m2){
			return MatrixMultiply(m1, m2);
		}

		Matrix MatrixIdentity(){
			return *new Matrix;
		}
		Matrix MatrixTranslation(float dX, float dY, float dZ){
			return Matrix(
				1, 0, 0, dX,
				0, 1, 0, dY,
				0, 0, 1, dZ,
				0, 0, 0, 1);
		}
		Matrix MatrixScale(float sX, float sY, float sZ){
			return Matrix(
				sX, 0, 0, 0,
				0, sY, 0, 0,
				0, 0, sZ, 0,
				0, 0, 0, 1);
		}

		Matrix MatrixRotationAxis( const Vector3& axis, const float& angle){
			float x = axis.x, y = axis.y, z = axis.z;
			float c = cosf( ToRadian(angle) ), s = sinf( ToRadian(angle) );
			return Matrix(
				x*x*(1 - c) + c, x*y*(1 - c) - z*s, x*z*(1 - c) + y*s, 0,
				x*y*(1 - c) + z*s, y*y*(1 - c) + c, y*z*(1 - c) - x*s, 0,
				x*z*(1 - c) - y*s, y*z*(1 - c) + x*s, z*z*(1 - c) + c, 0,
				0, 0, 0, 1
				);
		}
		Matrix MatrixRotationX( const float& angle){
			return MatrixRotationAxis( Vector3(1, 0, 0), angle);
		}
		Matrix MatrixRotationY( const float& angle){
			return MatrixRotationAxis( Vector3(0, 1, 0), angle);
		}
		Matrix MatrixRotationZ( const float& angle){
			return MatrixRotationAxis( Vector3(0, 0, 1), angle);
		}

		Matrix MatrixMultiply(const Matrix& p1, const Matrix& p2){
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

		Matrix MatrixTranspose(const Matrix& mat)
		{
			const float *m = mat.m;
			Matrix result;
			result.m[0] = m[0];   result.m[1] = m[4];   result.m[2] = m[8];   result.m[3] = m[12];
			result.m[4] = m[1];   result.m[5] = m[5];   result.m[6] = m[9];   result.m[7] = m[13];
			result.m[8] = m[2];   result.m[9] = m[6];   result.m[10] = m[10];  result.m[11] = m[14];
			result.m[12] = m[3];   result.m[13] = m[7];   result.m[14] = m[11];  result.m[15] = m[15];
			return result;
		}

		Matrix4 MatrixFrustum(float l, float r, float b, float t, float n, float f)
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
		Matrix4 MatrixPerspective(float fovY, float aspect, float front, float back)
		{
			float tangent = tanf( ToRadian( fovY / 2 ) ); // tangent of half fovY
			float height = front * tangent;         // half height of near plane
			float width = height * aspect;          // half width of near plane

			// params: left, right, bottom, top, near, far
			return MatrixFrustum(-width, width, -height, height, front, back);
		}

		float MatrixDeterminant(const Matrix&){
			float result = 0;

			return result;
		}
		
	}
}