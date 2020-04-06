#ifndef __SPACE_HEADER_HPP__
#define __SPACE_HEADER_HPP__

#include "Prerequisites.hpp"

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
		void Render();
		void Logic();
		void Update();

		//std::unique_ptr<Render::Renderer> renderer;
		// int ExecuteRender(Render::CommandList* pList);
		std::unique_ptr<Render::RenderWindow> m_pRenderWindow = nullptr;
		std::unique_ptr<Render::RenderTarget> m_pRenderTarget = nullptr;
		uint32 _Width = 0, _Height = 0;
		std::unique_ptr<Render::Texture2D> m_pDepthBuffer = nullptr;
		std::unique_ptr<Render::DepthStencilView> m_pDepthStencilView = nullptr;
		std::unique_ptr<Render::CommandList> m_pFrontCommandList = nullptr;
		std::unique_ptr<Render::CommandList> m_pBackCommandList = nullptr;
	
		std::unique_ptr<Render::PipelineState> m_pPipelineState = nullptr;
		// std::unique_ptr<Render::VertexShader> _vs;
		// std::unique_ptr<Render::PixelShader> _ps;
		//std::unique_ptr<Space::Mesh> _mesh = nullptr;

		//std::unique_ptr<Render::ConstantBuffer> _PerFrameData = nullptr;
		//std::unique_ptr<Render::ConstantBuffer> _PerObjectData = nullptr;

		std::atomic<int> m_CloseFlag = 0;
		std::thread m_RenderThread;
		std::mutex m_Mutex;
		std::unique_lock<std::mutex> m_Lock;
		std::condition_variable m_Fence;

		std::future<Render::CommandList*> m_CmdList;
		std::future<int> m_RenderAction;
	};
}

#endif