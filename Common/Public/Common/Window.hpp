#ifndef __SPACE_COMMON_WINDOWUTILITY_HPP__
#define __SPACE_COMMON_WINDOWUTILITY_HPP__

#include "Common/Prerequisites.hpp"
#include "Common/Basic.hpp"

#include "Common/Event.hpp"

namespace Space
{
	class SPACE_COMMON_API Window : public Interface
	{
	public:
		static Window* Create(const std::string& name, int32 width, int32 height, bool fullscreen);
		static Window* Create(const std::wstring& name, int32 width, int32 height, bool fullscerrn);
		virtual ~Window();

		virtual void Show() = 0;
		
		inline int32 Width() const
		{
			return m_Width;
		}
		inline int32 Height() const
		{
			return m_Height;
		}
		inline bool IsFullscreen() const
		{
			return m_Fullscreen;
		}
				
	protected: 
		Window(const std::wstring& name, int32 width, int32 height, bool fullscreen);
		 
		EventDispatcher* m_EventDispatcher = nullptr;
		std::wstring m_Name;
		int32 m_Width, m_Height;
		bool m_Fullscreen;
		//std::function<void(*)> Resize;
	};

	typedef std::shared_ptr<Window> WindowPtr;
}

#endif