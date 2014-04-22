#ifndef __SPACE_RENDERWINDOW_H__
#define __SPACE_RENDERWINDOW_H__

#include "Core.h"
#include "RenderTarget.h"

namespace space{
	namespace graphic{
		class IRenderWindow : virtual public IRenderTarget{
		public:
			virtual void Create(const String & name, uint width, uint height, bool fullScreen) = 0;
			virtual void _Initialize(bool fullScreen) = 0;

			virtual void SetFullScreen(bool fullScreen, uint width, uint height){}
			virtual bool isFullScreen(void) const{ return mIsFullScreen; }

			virtual void Resize(uint width, uint height) = 0;
			virtual void Reposition(int left, int top) = 0;

		protected:
			bool mIsFullScreen;
			bool mIsPrimary;
			bool mAutoDeactivatedOnFocusChange;
			int mLeft;
			int mTop;

			friend class Core;
		};
	}
}

#endif