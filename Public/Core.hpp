#ifndef __SPACE_CORE_HPP__
#define __SPACE_CORE_HPP__

#include "Prerequisites.hpp"
#include "Event.hpp"
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
		static Core* GetInstance();

		RenderSystem* CreateD3DRenderSystem();
		RenderSystem* CreateGLRenderSystem();
		
		EventDispatcher* GetEventDispatcher() const;
	private:
		Core();
	
		static std::unique_ptr<Core> m_Instance;

		std::unique_ptr<EventDispatcher> m_EventDispatcher = nullptr;
	};
}


#endif