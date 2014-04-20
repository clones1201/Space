#ifndef __SPACE_SCENE_MANAGER_H__
#define __SPACE_SCENE_MANAGER_H__

#include "basic.h"

namespace space{
	namespace graphic{

		class SceneNode : public Uncopyable{

		};

		class SceneManager : public Uncopyable{
		protected:
			SceneNode* root;
		public:
			
		};
	}
}


#endif