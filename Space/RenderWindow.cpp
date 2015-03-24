#include "Window.hpp"
#include "RenderWindow.hpp"

namespace Space
{

	RenderWindow::RenderWindow(const std::string& name, int width, int height, bool fullscreen)
	{
		m_pWindow.reset(Window::Create(name, width, height, fullscreen));
	}

}