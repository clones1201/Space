#ifndef __RENDERSYSTEM_H__
#define __RENDERSYSTEM_H__

#include "Prerequisites.h"
#include "basic.h"
#include "Camera.h"
#include "SceneManager.h"
#include "RenderTarget.h"

namespace space{
	namespace graphic{

		typedef map<string, IRenderTarget*> RenderTargetMap;


		class IRenderSystem : public  Interface{
		protected:
			RenderTargetMap mRenderTargets;
			IRenderTarget * mActiveRenderTarget;
		public:
			IRenderSystem(){}
			~IRenderSystem(){}

			virtual IRenderWindow* _createRenderWindow(const String &name, unsigned int width, unsigned int height,
				bool fullScreen) = 0;

			//virtual void AttachRenderTarget(IRenderTarget &rt) = 0;
			//virtual void DetachRenderTarget(const string &name) = 0;
			
			virtual IRenderWindow* _Initialize(bool autoCreateWindow) = 0;

			virtual void _Render() = 0;

			virtual void _BeginScene() = 0;

			virtual void _EndScene() = 0;

			virtual void ShutDown(void) = 0;

			virtual void SetAmbientLight(float r, float g, float b) = 0;
		}; 

		//============================
		// RenderSystem
		// Packed Graphic API
		//============================
		typedef shared_ptr<IRenderSystem> IRenderSystem_ptr;
		
	}
}

#endif