#ifndef __SPACE_GAME_HPP__
#define __SPACE_GAME_HPP__

#include "Space/Space.hpp"

namespace Space
{
	class SimpleGame
	{
	public:
		SimpleGame();
		~SimpleGame();

		void Run();
		void Close();
	private: 
		void MainLoop();
		void Update();
		CommandList* RenderOneFrame();
		int ExecuteRender(CommandList* pList);

		std::unique_ptr<RenderSystem> m_pRenderSys = nullptr;
		std::unique_ptr<RenderWindow> m_pRenderWindow = nullptr;
		std::unique_ptr<RenderTarget> m_pRenderTarget = nullptr;
		std::unique_ptr<CommandList> m_pFrontCommandList = nullptr;
		std::unique_ptr<CommandList> m_pBackCommandList = nullptr;
		std::unique_ptr<MeshMaterialRenderer> m_pSimpleRenderer = nullptr;
		std::unique_ptr<Mesh> m_pMesh = nullptr;
		std::unique_ptr<Material> m_pMaterial = nullptr;

		std::atomic<int> m_CloseFlag = 0;
		std::thread m_RenderThread;
		std::mutex m_Mutex;
		std::unique_lock<std::mutex> m_Lock;
		std::condition_variable m_Fence;

		std::future<CommandList*> m_CmdList;
		std::future<int> m_RenderAction;
	};
}

#endif