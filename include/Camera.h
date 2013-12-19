#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "basic.h"

namespace space{
	class ICamera : public Interface{
	public:
		virtual math::Matrix GetMatrix() const = 0;
		virtual math::Matrix GetModelViewMatrix() const = 0;

		virtual void SetEye(float x, float y, float z) = 0;
		virtual void SetEye(const math::Vector3&) = 0;
		virtual void SetLookAt(float x, float y, float z) = 0;
		virtual void SetLookAt(const math::Vector3&) = 0;
		virtual void SetUp(float x, float y, float z) = 0;
		virtual void SetUp(const math::Vector3&) = 0;

		virtual math::Vector3 GetForward() const = 0;
		virtual math::Vector3 GetRight() const = 0;
		virtual math::Vector3 GetLook() const = 0;
		virtual math::Vector3 GetEye() const = 0;

		virtual void ZoomIn() = 0;
		virtual void ZoomOut() = 0;
		virtual void Move(math::Vector3 dir) = 0;
		virtual void RotateL(float degX, float degY) = 0;
		virtual void RotateE(float degX, float degY) = 0;
	};

	class Camera : public Object, public ICamera{
	protected:
		math::Vector3 vEye, vLook;
		math::Vector3 vUp, vRight;
	public:
		Camera(math::Vector3 eye = math::Vector3(0, 0, 0), math::Vector3 look = math::Vector3(0, 0, -1)) :
			vEye(eye), vLook(look), vUp(0, 1, 0), vRight(1, 0, 0){
			math::Vector3 vl;
			vl = Vec3Normalize(vLook);
			vRight = Vec3Normalize(Vec3Cross(vl, vUp));
			vUp = Vec3Normalize(Vec3Cross(vRight, vl));
		}

		virtual math::Matrix GetMatrix() const{ return *(new math::Matrix); }
		virtual math::Matrix GetModelViewMatrix() const{ return *(new math::Matrix); }

		virtual void SetEye(float x, float y, float z){}
		virtual void SetEye(const math::Vector3&){}
		virtual void SetLookAt(float x, float y, float z){}
		virtual void SetLookAt(const math::Vector3&){}
		virtual void SetUp(float x, float y, float z){}
		virtual void SetUp(const math::Vector3&){}

		virtual math::Vector3 GetForward() const{ return Vec3Normalize(vLook); }
		virtual math::Vector3 GetRight() const{ return Vec3Normalize(vRight); }
		virtual math::Vector3 GetLook() const{ return Vec3Normalize(vLook); }
		virtual math::Vector3 GetEye() const{ return vEye; }

		virtual void ZoomIn(){}
		virtual void ZoomOut(){}
		virtual void Move(math::Vector3 dir){}
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
		PerspectiveCamera(math::Vector3 eye = math::Vector3(0, 0, 0), math::Vector3 look = math::Vector3(0, 0, -1));
		~PerspectiveCamera();

		//Interface is Uncopyble, but if we override the copy constructor, it become copyble.
		PerspectiveCamera(const PerspectiveCamera& camera):Camera(camera.vEye,camera.vLook),
			fovy(camera.fovy),aspect(camera.aspect),zNear(camera.zNear),zFar(camera.zFar){
		}
		
		void SetPerspective(float fovy, float aspect, float zNear, float zFar);
		void SetAspect(float _aspect){
			aspect = _aspect;
		}

		math::Matrix GetMatrix() const final;
		/*only for ray trace, because camera will move*/
		math::Matrix GetModelViewMatrix() const final;
		void GetPerspective(float &fovy, float &aspect, float &zNear, float &zFar)const;

		virtual math::Vector3 GetForward() const{ return Vec3Normalize(vLook); }
		virtual math::Vector3 GetRight() const{ return Vec3Normalize(vRight); }
		virtual math::Vector3 GetLook() const{ return Vec3Normalize(vLook); }
		virtual math::Vector3 GetEye() const{ return vEye; }

		virtual void SetEye(float x, float y, float z);
		virtual void SetEye(const math::Vector3&);
		virtual void SetLookAt(float x, float y, float z);
		virtual void SetLookAt(const math::Vector3&);
		virtual void SetUp(float x, float y, float z);
		virtual void SetUp(const math::Vector3&);
		
		void ZoomIn();
		void ZoomOut();
		void Move(math::Vector3 dir);
		void RotateL(float degX, float degY);
		void RotateE(float degX, float degY);
	};
	typedef shared_ptr<PerspectiveCamera> PerspectiveCamera_ptr;

	class BirdEyeCamera : public PerspectiveCamera{
	private:
		float zoom;
	public:
		BirdEyeCamera(float fHeight,float sealevel) :
			PerspectiveCamera(math::Vector3(0,fHeight,0), math::Vector3(sealevel - fHeight , sealevel - fHeight , 0)) {
			zoom = 1;
		}
		~BirdEyeCamera(){}

		virtual math::Vector3 GetForward() const; 

		void ZoomIn();
		void ZoomOut();

		void RotateL(float degX, float degY);
		void RotateE(float degX, float degY);
		 
	};
}
#endif