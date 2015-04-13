#ifndef __SPACE_WINDOWUTILITY_HPP__
#define __SPACE_WINDOWUTILITY_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"

namespace Space
{
	class Window : public Interface
	{
	public:
		static Window* Create(const std::string& name, int32 width, int32 height, bool fullscreen);
		static Window* Create(const std::wstring& name, int32 width, int32 height, bool fullscerrn);
		virtual ~Window();

		virtual void Show() = 0;
		
		int32 Width() const;
		int32 Height() const;
		bool IsFullscreen() const;

	protected: 
		Window(const std::wstring& name, int32 width, int32 height, bool fullscreen);

		std::wstring m_Name;
		int32 m_Width, m_Height;
		bool m_Fullscreen;
		//std::function<void(*)> Resize;
	};
}

#endif