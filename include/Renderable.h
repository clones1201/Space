#ifndef __SPACE_RENDERABLE_H__
#define __SPACE_RENDERABLE_H__

#include "basic.h"

namespace space{

	namespace graphic{
		class Renderable {
		public:
			virtual void GetRenderOperation(RenderOperation& op) const = 0;
			virtual Material_ptr GetMaterial(void) const = 0;
			virtual math::Matrix4 GetWorldTransforms(void) const = 0;
			virtual
		};
	}
}



#endif
