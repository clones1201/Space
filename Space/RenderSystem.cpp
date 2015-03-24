#include "Log.h"
#include "Utility.h"
#include "SharedUtility.hpp"
#include "RenderSystem.hpp"
#include "RenderTarget.hpp"
#include "DeviceBuffer.hpp"
#include "DeviceTexture.hpp"
#include "RenderWindow.hpp"

namespace Space
{
	RenderSystem::RenderSystem(){}

	RenderSystem::~RenderSystem(){}

	void RenderSystem::Render(RenderTarget* target, SceneManager* scene)
	{
		mActiveRenderTarget = target;
		Prerender();
		FireOneFrame();
		Postrender();
	}

	void RenderSystem::Prerender()
	{}
	void RenderSystem::Postrender()
	{}
	void RenderSystem::FireOneFrame()
	{}
}