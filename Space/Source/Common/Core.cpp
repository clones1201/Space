#include "Common/Core.hpp"

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
	
	tstring Core::GetAssetsDir() const
	{
		return TEXT("./Assets");
	}

	EventDispatcher* Core::GetEventDispatcher() const
	{
		return m_EventDispatcher.get();
	}
}
