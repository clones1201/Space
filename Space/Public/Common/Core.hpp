#ifndef __SPACE_COMMON_CORE_HPP__
#define __SPACE_COMMON_CORE_HPP__

#include "Common/Prerequisites.hpp"
#include "Common/Window.hpp"

namespace Space
{
	class SPACE_COMMON_API Core
	{
	public:
		static Core* GetInstance();
		  
		tstring GetAssetsDir() const;

		EventDispatcher* GetEventDispatcher() const;
	private:
		Core();
	
		static std::unique_ptr<Core> m_Instance;

		std::unique_ptr<EventDispatcher> m_EventDispatcher = nullptr;
	}; 
}


#endif