#include "RenderSystem/RenderSystem.hpp"

namespace Space
{
	RenderSystem::RenderSystem(){}

	RenderSystem::~RenderSystem(){}


	
	RenderTarget::RenderTarget(uint32 width, uint32 height)
		:m_Width(width),m_Height(height)
	{
	}
	RenderTarget::~RenderTarget()
	{
	}
	

	RenderWindow::RenderWindow(const std::string& name, int32 width, int32 height, bool fullscreen)
	{
		m_pWindow.reset(Window::Create(name, width, height, fullscreen));
	}
}