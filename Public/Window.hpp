#ifndef __SPACE_WINDOWUTILITY_HPP__
#define __SPACE_WINDOWUTILITY_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"

namespace Space
{
	class Window : public Uncopyable
	{
	private:
		class Impl;
		std::unique_ptr<Impl> impl;

		Window(const std::string& winName, int32 width, int32 height, bool fullscreen);
	public:
		static Window* Create(const std::string& winName, int32 width, int32 height, bool fullscreen);
		~Window();

#if SPACE_PLATFORM == SPACE_WIN32
		class Win32Window;
#endif
		void Show();
		int32 Width() const;
		int32 Height() const;
		bool IsFullscreen() const;

	protected:
		//std::function<void(*)> Resize;
	};
}

#endif