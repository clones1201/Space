#ifndef __SPACE_CORE_HPP__
#define __SPACE_CORE_HPP__

#include "Prerequisites.hpp"

namespace Space
{
	class Core
	{
	public:
		static RenderSystem* CreateD3DRenderSystem();
		static RenderSystem* CreateGLRenderSystem();


	};
}


#endif