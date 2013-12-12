#include "Camera.h"

namespace space{
	using namespace math;

	PerspectiveCamera::PerspectiveCamera(Vector3 eye, Vector3 look) :
		Camera(eye, look), fovy( 60.0f ), aspect(1.0f), zNear(0.1f), zFar(50000.0f){
	}

	PerspectiveCamera::~PerspectiveCamera(){
	}

	void PerspectiveCamera::SetEye(float x, float y, float z){
		vEye.x = x; vEye.y = y; vEye.z = z;
	}
	void PerspectiveCamera::SetEye(const float3& eye){
		SetEye(eye.x, eye.y, eye.z);
	}
	void PerspectiveCamera::SetLookAt(float x, float y, float z){
		vLook.x = x - vEye.x; vLook.y = y - vEye.y; vLook.z = z - vEye.z;
	}
	void PerspectiveCamera::SetLookAt(const float3& lookat){
		SetLookAt(lookat.x, lookat.y, lookat.z);
	}
	void PerspectiveCamera::SetUp(float x, float y, float z){
		vUp.x = x; vUp.y = y; vUp.z = z;
	}
	void PerspectiveCamera::SetUp(const float3& eye){
		SetUp(eye.x, eye.y, eye.z);
	}
	
	void PerspectiveCamera::SetPerspective(float f, float a, float zN, float zF){
		fovy = f; aspect = a; zNear = zN; zFar = zF;
	}


	void PerspectiveCamera::GetPerspective(float &f, float &a, float &zN, float &zF)const{
		f = fovy; a = aspect; zN = zNear; zF = zFar;
	}

	void PerspectiveCamera::ZoomIn(){
		if (fovy - ( Pi / 40) >= Pi / 20){
			fovy -= Pi / 40;
		}
	}

	void PerspectiveCamera::ZoomOut(){
		if (fovy + (Pi / 40) <= Pi / 2){
			fovy += Pi / 40;
		}
	}

	void PerspectiveCamera::Move(Vector3 dir){
		vEye += dir;
	}

	void PerspectiveCamera::RotateL(float degX, float degY){
		Matrix matRotation;
		if (degY != 0){
			matRotation = MatrixMultiply( matRotation , MatrixRotationAxis(vRight, -degY));
			vLook = Vec3Transform(float4(vLook.x,vLook.y,vLook.z,1), matRotation);
			vUp = Vec3Transform(float4(vUp.x, vUp.y, vUp.z, 1), matRotation);
		}

		if (degX != 0)
		{
			matRotation = MatrixMultiply(matRotation, MatrixRotationAxis(Vector3(0, 1, 0), degX));
			vLook = Vec3Transform(float4(vLook.x, vLook.y, vLook.z, 1), matRotation);
			vUp = Vec3Transform(float4(vUp.x, vUp.y, vUp.z, 1), matRotation);
		}

		Vector3 vl;
		vl = Vec3Normalize( vLook );
		vRight = Vec3Normalize(  Vec3Cross(vl, vUp) );
		vUp = Vec3Normalize( Vec3Cross( vRight, vl));
		

	}

	void PerspectiveCamera::RotateE(float degX, float degY){
		Matrix matRotation;
		Vector3 lookat = vEye + vLook;
		Vector3 temp = -vLook;
		if (degY != 0){
			matRotation = MatrixMultiply(matRotation, MatrixRotationAxis(vRight,-degY));
			temp = Vec3Transform(float4(temp.x, temp.y, temp.z, 1), matRotation);
			vUp = Vec3Transform(float4(vUp.x, vUp.y, vUp.z, 1), matRotation);
		}

		if (degX != 0)
		{
			matRotation = MatrixMultiply(matRotation, MatrixRotationAxis(Vector3(0,1,0), degX));
			temp = Vec3Transform(float4(temp.x, temp.y, temp.z, 1), matRotation);
			vUp = Vec3Transform(float4(vUp.x, vUp.y, vUp.z, 1), matRotation);
		}
		vEye = lookat + temp;
		vLook = -temp;

		Vector3 vl;
		vl = Vec3Normalize(vLook);
		vRight = Vec3Normalize(Vec3Cross(vl, vUp));
		vUp = Vec3Normalize(Vec3Cross(vRight, vl));

		ofstream fs("camera.txt", ios_base::app);
		fs << "eye: (" << vEye.x << "," << vEye.y << "," << vEye.z << ")" << endl;
		fs << "lookat:(" << lookat.x << ","<< lookat.y << "," << lookat.z << ")" << endl;
		fs << endl;
	}

	Matrix PerspectiveCamera::GetMatrix() const {
		Vector3 f = Vec3Normalize(vLook); 
		Vector3 s = Vec3Normalize(Vec3Cross(f, vUp));
		Vector3 up = Vec3Normalize(Vec3Cross(s, f));
		Matrix result;

		result = MatrixMultiply(result, MatrixPerspective(fovy, aspect, zNear, zFar));
		result = MatrixMultiply(result, MatrixMultiply( 
			Matrix(
			0,0,1,0,
			0,1,0,0,
			-1,0,0,0,
			0,0,0,1),
			Matrix(
			f.x,f.y,f.z, 0,
			up.x,up.y,up.z,0,
			s.x,s.y,s.z,0,
			0, 0, 0, 1)));
		result = MatrixMultiply(result, MatrixTranslation(-vEye.x, -vEye.y, -vEye.z));
		return result;
	}

	Matrix PerspectiveCamera::GetModelViewMatrix() const{
		Vector3 f = Vec3Normalize(vLook);
		Vector3 s = Vec3Normalize(Vec3Cross(f, vUp));
		Vector3 up = Vec3Normalize(Vec3Cross(s, f));
		Matrix result = MatrixInverse( MatrixMultiply(
			Matrix(
			s.x, up.x, -f.x, 0,
			s.y, up.y, -f.y, 0,
			s.z, up.z, -f.z, 0,
			1, 1, 1, 1),
			Matrix(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			-1, -1, -1, 1)
			)); 
		result = MatrixMultiply(result, MatrixTranslation(-vEye.x, -vEye.y, -vEye.z));
		return result;
	}

	Vector3 BirdEyeCamera::GetForward() const{
		return Vector3(vLook.x, 0, vLook.z);
	}

	void BirdEyeCamera::ZoomIn(){
		if (zoom - 0.05f > 0.3f){
			zoom -= 0.05f;
			PerspectiveCamera::RotateE(0, -5.0f);
			Vector3 lookat = vEye + vLook;
			vLook = vLook * 0.9f;
			vEye = lookat - vLook;
		}
	}
	void BirdEyeCamera::ZoomOut(){
		if (zoom + 0.05f <= 1.00f){
			zoom += 0.05f;
			PerspectiveCamera::RotateE(0, 5.0f);
			Vector3 lookat = vEye + vLook;
			vLook = vLook * 1.1111f;
			vEye = lookat - vLook;
		}
	}

	void BirdEyeCamera::RotateL(float degX, float degY){
		PerspectiveCamera::RotateL(degX, 0);
	}

	void BirdEyeCamera::RotateE(float degX, float degY){
		PerspectiveCamera::RotateE(degX, 0);
	}
}
