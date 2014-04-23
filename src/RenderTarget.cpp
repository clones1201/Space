#include "RenderTarget.h"

namespace space{
	namespace graphic{

		RenderTarget::RenderTarget(){
		}

		RenderTarget::~RenderTarget(){
		}

		const String& RenderTarget::GetName(void) const{
			return name;
		}

		uint RenderTarget::GetWidth() const{
			return mWidth;
		}

		uint RenderTarget::GetHeight() const{
			return mHeight;
		}

		void RenderTarget::Update(bool swapBuffers){
			firePreUpdate();
			_BeginUpdate();

			_EndUpdate();
			firePostUpdate();
		}

		void RenderTarget::_BeginUpdate(){

		}

		void RenderTarget::_EndUpdate(){

		}

		void RenderTarget::firePreUpdate(){

		}

		void RenderTarget::firePostUpdate(){

		}

	
	}
}