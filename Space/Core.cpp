#include "Core.hpp"
#include "D3D11RenderSystem.hpp"
#include "D3D11RenderTarget.hpp"
#include "D3D11RenderWindow.hpp"

namespace Space
{

	RenderSystem* Core::CreateD3DRenderSystem()
	{
		return D3D11RenderSystem::Create();
	}
	RenderSystem* Core::CreateGLRenderSystem()
	{
		throw std::exception("Not Implemented");
	}
}
