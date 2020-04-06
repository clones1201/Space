#ifndef __SPACE_COMMON_CORE_HPP__
#define __SPACE_COMMON_CORE_HPP__

#include "Prerequisites.hpp"

namespace Space
{
	class SPACE_COMMON_API Core
	{
	public:
		static Core* GetInstance();
		  
		tstring GetAssetsDir() const;
	private:
		Core();
	
		static std::unique_ptr<Core> m_Instance;
	};

}


#endif