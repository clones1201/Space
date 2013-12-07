#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "basic.h"

namespace space{
	using namespace math;

	class ICamera : public Interface{
	public:
		virtual Matrix GetMatrix() const = 0;
		virtual Matrix GetModelViewMatrix() const = 0;

		virtual void SetEye(float x, float y, float z) = 0;
		virtual void SetEye(const Vector3&) = 0;
		virtual void SetLookAt(float x, float y, float z) = 0;
		virtual void SetLookAt(const Vector3&) = 0;
		virtual void SetUp(float x, float y, float z) = 0;
		virtual void SetUp(const Vector3&) = 0;

		virtual Vector3 GetForward() const = 0;
		virtual Vector3 GetRight() const = 0;
		virtual Vector3 GetLook() const = 0;
		virtual Vector3 GetEye() const = 0;

		virtual void ZoomIn() = 0;
		virtual void ZoomOut() = 0;
		virtual void Move(Vector3 dir) = 0;
		virtual void RotateL(float degX, float degY) = 0;
		virtual void RotateE(float degX, float degY) = 0;
	};

	class Camera : public Object, public ICamera{
	protected:
		Vector3 vEye, vLook;
		Vector3 vUp, vRight;
	public:
		Camera(Vector3 eye = Vector3(0, 0, 0), Vector3 look = Vector3(0, 0, -1)) :
			vEye(eye), vLook(look), vUp(0, 1, 0), vRight(1, 0, 0){
			Vector3 vl;
			vl = Vec3Normalize(vLook);
			vRight = Vec3Normalize(Vec3Cross(vl, vUp));
			vUp = Vec3Normalize(Vec3Cross(vRight, vl));
		}

		virtual Matrix GetMatrix() const{ return *(new Matrix); }
		virtual Matrix GetModelViewMatrix() const{ return *(new Matrix); }

		virtual void SetEye(float x, float y, float z){}
		virtual void SetEye(const Vector3&){}
		virtual void SetLookAt(float x, float y, float z){}
		virtual void SetLookAt(const Vector3&){}
		virtual void SetUp(float x, float y, float z){}
		virtual void SetUp(const Vector3&){}

		virtual Vector3 GetForward() const{ return Vec3Normalize(vLook); }
		virtual Vector3 GetRight() const{ return Vec3Normalize(vRight); }
		virtual Vector3 GetLook() const{ return Vec3Normalize(vLook); }
		virtual Vector3 GetEye() const{ return vEye; }

		virtual void ZoomIn(){}
		virtual void ZoomOut(){}
		virtual void Move(Vector3 dir){}
		virtual void RotateL(float degX, float degY){}
		virtual void RotateE(float degX, float degY){}
	};

	class IPerspectiveCamera : public ICamera{
	public:
		virtual void SetPerspective(float fovy, float aspect, float zNear, float zFar) = 0;
		virtual void SetAspect(float _aspect) = 0;

		void GetPerspective(float &fovy, float &aspect, float &zNear, float &zFar)const;
	};

	class PerspectiveCamera : public Camera, public IPerspectiveCamera{
	private:
		float fovy;
		float aspect;
		float zNear;
		float zFar;
	public:
		PerspectiveCamera(Vector3 eye = Vector3(0, 0, 0), Vector3 look = Vector3(0, 0, -1));
		~PerspectiveCamera();

		//Interface is Uncopyble, but if we override the copy constructor, it become copyble.
		PerspectiveCamera(const PerspectiveCamera& camera):Camera(camera.vEye,camera.vLook),
			fovy(camera.fovy),aspect(camera.aspect),zNear(camera.zNear),zFar(camera.zFar){
		}
		
		void SetPerspective(float fovy, float aspect, float zNear, float zFar);
		void SetAspect(float _aspect){
			aspect = _aspect;
		}

		Matrix GetMatrix() const final;
		Matrix GetModelViewMatrix() const final;
		void GetPerspective(float &fovy, float &aspect, float &zNear, float &zFar)const;

		virtual Vector3 GetForward() const{ return Vec3Normalize(vLook); }
		virtual Vector3 GetRight() const{ return Vec3Normalize(vRight); }
		virtual Vector3 GetLook() const{ return Vec3Normalize(vLook); }
		virtual Vector3 GetEye() const{ return vEye; }

		virtual void SetEye(float x, float y, float z);
		virtual void SetEye(const Vector3&);
		virtual void SetLookAt(float x, float y, float z);
		virtual void SetLookAt(const Vector3&);
		virtual void SetUp(float x, float y, float z);
		virtual void SetUp(const Vector3&);
		
		void ZoomIn();
		void ZoomOut();
		void Move(Vector3 dir);
		void RotateL(float degX, float degY);
		void RotateE(float degX, float degY);
	};

	class BirdEyeCamera : public PerspectiveCamera{
	private:
		float zoom;
	public:
		BirdEyeCamera(float fHeight,float sealevel) :
			PerspectiveCamera(Vector3(0,fHeight,0), Vector3(sealevel - fHeight , sealevel - fHeight , 0)) {
			zoom = 1;
		}
		~BirdEyeCamera(){}

		virtual Vector3 GetForward() const; 

		void ZoomIn();
		void ZoomOut();

		void RotateL(float degX, float degY);
		void RotateE(float degX, float degY);
		 
	};
}
#endif