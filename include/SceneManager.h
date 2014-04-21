#ifndef __SPACE_SCENE_MANAGER_H__
#define __SPACE_SCENE_MANAGER_H__

#include "basic.h"
#include "Camera.h"

namespace space{
	namespace graphic{

		class SceneNode : public Uncopyable{

		};

		class SceneManager : public Uncopyable{
		protected:
			SceneNode* root;
			Camera camera;
		public:
			
		};
	}
}


#endif