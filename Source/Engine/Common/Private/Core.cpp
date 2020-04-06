#include "Core.hpp"

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
	}
	
	tstring Core::GetAssetsDir() const
	{
		return TEXT("./Assets");
	}
}
