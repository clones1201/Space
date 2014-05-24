#ifndef __SPACE_MOVABLE_H__
#define __SPACE_MOVABLE_H__

#include "basic.h"
#include "SceneManager.h"

namespace space{
	namespace graphic{
		class Movable : public Object{
		public:
			virtual BBox GetBoundingBox() = 0;
			//virtual Spere GetBoundingSphere() = 0;

			//light,material,visible,shadow
			//should be attach to a SceneNode
			//moving the node is moving the object
		protected:
			//SceneNode* node;
		};
	}
}

#endif