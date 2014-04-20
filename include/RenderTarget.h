#ifndef __SPACE_RENDER_TARGET_H__
#define __SPACE_RENDER_TARGET_H__

#include "basic.h"
#include "ViewPort.h"

namespace space{
	namespace graphic{
		class IRenderTarget : public virtual Interface{
		public:
			struct FrameStats
			{
				float lastFPS;
				float avgFPS;
				float bestFPS;
				float worstFPS;
				unsigned long bestFrameTime;
				unsigned long worstFrameTime;
				size_t triangleCount;
				size_t batchCount;
			};

			IRenderTarget();
			~IRenderTarget();

			virtual const string& GetName(void) const;

			virtual uint GetWidth(void) const;
			virtual uint GetHeight(void) const;

			virtual void Update(bool swapBuffers = true);

			virtual void SwapBuffers(void){}

			virtual void _BeginUpdate();

			virtual void _EndUpdate();

		protected:
			string name;

			uint mWidth;
			uint mHeight;

			virtual void firePreUpdate(void);

			virtual void firePostUpdate(void);
		};
	}
}


#endif