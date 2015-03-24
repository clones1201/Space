#ifndef __SPACE_GAME_HPP__
#define __SPACE_GAME_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"

namespace Space
{
	class Game
	{
	public:
		std::function<void()> Update = []()->void{};
		std::function<void()> Initial = []()->void{};
		void Run();
		void SetRenderSystem(RenderSystem* renderer);
		void SetRenderTarget(RenderTarget* target);
		//void SetScene(SceneManager* scene);
	private:
		RenderSystem* renderer;
		RenderTarget* currentTarget;
		//SceneManager* scene;
	};
}

#endif