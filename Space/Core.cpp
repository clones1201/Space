#include "Core.hpp"
#include "D3D11RenderSystem.hpp"
#include "D3D11RenderTarget.hpp"
#include "D3D11RenderWindow.hpp"

namespace Space
{
	std::unique_ptr<Core> Core::m_Instance;

	std::once_flag g_CoreCreationOnceFlag;
	Core* Core::GetInstance()
	{
		std::call_once(g_CoreCreationOnceFlag,
			[&]{
			m_Instance.reset(new Core);
		});
		return m_Instance.get();
	}

	Core::Core()
	{
		m_EventDispatcher.reset(new EventDispatcher);
	}


	RenderSystem* Core::CreateD3DRenderSystem()
	{
		return D3D11RenderSystem::Create();
	}
	RenderSystem* Core::CreateGLRenderSystem()
	{
		throw std::exception("Not Implemented");
	}
	
	EventDispatcher* Core::GetEventDispatcher() const
	{
		return m_EventDispatcher.get();
	}
}
