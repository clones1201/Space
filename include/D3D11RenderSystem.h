#ifndef __SPACE_D3D11_RENDERSYSTEM_H__
#define __SPACE_D3D11_RENDERSYSTEM_H__

#include "RenderSystem.h"
#include "D3D11Prerequisites.h"

namespace space{
	namespace graphic{

		class D3D11Device{
		private:
			ID3D11Device *pD3D11Device;
			ID3D11DeviceContext *pImmediateContext;
		public:
			D3D11Device(ID3D11Device * device);
			D3D11Device();
			~D3D11Device();

			inline ID3D11Device * operator->(){
				return pD3D11Device;
			}

			ID3D11Device * Get(){
				return pD3D11Device;
			}

			bool isNull() const;

			ID3D11DeviceContext* GetImmediateContext(){
				return pImmediateContext;
			}

			void Release(){
				SAFE_RELEASE(pD3D11Device);
				SAFE_RELEASE(pImmediateContext);
			}

		};

		class D3D11RenderSystem : public RenderSystem{
		
		public:
			D3D11RenderSystem(HINSTANCE inst);
			~D3D11RenderSystem();

			virtual RenderWindow* _createRenderWindow(const String &name, unsigned int width, unsigned int height,
				bool fullScreen);

			//virtual void AttachRenderTarget(IRenderTarget &rt) = 0;
			//virtual void DetachRenderTarget(const string &name) = 0;

			virtual void InitRenderSystem();

			virtual RenderWindow* _Initialize(bool autoCreateWindow);

			virtual void _Render();

			virtual void _BeginScene();

			virtual void _EndScene();

			virtual void ShutDown(void);

			virtual void SetAmbientLight(float r, float g, float b);
		private:
			HINSTANCE mhInstance;
			D3D11Device mDevice;
		};
	}
}

#endif