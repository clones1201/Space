#include "RenderSystem/RenderSystem.hpp"

namespace Space
{

	RenderWindow::RenderWindow(const std::string& name, int32 width, int32 height, bool fullscreen)
	{
		m_pWindow.reset(Window::Create(name, width, height, fullscreen));
	}

}