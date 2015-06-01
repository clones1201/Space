#ifndef __SPACE_CORE_HPP__
#define __SPACE_CORE_HPP__

#include "Prerequisites.hpp"
#include "DeviceBuffer.hpp"
#include "RenderSystem.hpp"
#include "DeviceTexture.hpp"
#include "Renderer.hpp"
#include "ShaderResource.hpp"

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