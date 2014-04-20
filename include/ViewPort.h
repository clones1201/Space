#ifndef __SPACE_VIEWPORT_H__
#define __SPACE_VIEWPORT_H__

#include "basic.h"

namespace space{

	namespace graphic{

		class ViewPort : public Interface{
		public:

		protected:
			Rect rect;
		};
	}
}

#endif