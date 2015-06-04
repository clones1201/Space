#include "Game.hpp"

namespace Space
{
	SimpleGame::SimpleGame()
	{
		m_pRenderSys.reset(
			Core::GetInstance()->CreateD3DRenderSystem());
		m_pGraphicCommandList.reset(
			m_pRenderSys->CreateCommandList());
		m_pRenderWindow.reset(
			m_pRenderSys->CreateRenderWindow("RenderWindow", 500, 500, false));
		m_pRenderWindow->Initialize();

		m_pRenderTarget.reset(
			m_pRenderSys->CreateRenderTarget(m_pRenderWindow->GetBackBuffer()));

		m_pMaterial.reset(Material::Create(m_pRenderSys.get(), "default"));
		m_pMesh.reset(Mesh::CreateFromFBX(m_pRenderSys.get(), "bunny.fbx"));

		m_pSimpleRenderer.reset(
			MeshMaterialRenderer::Create(m_pRenderSys.get()));
		m_pSimpleRenderer->SetRenderTarget(m_pRenderTarget.get());
		m_pSimpleRenderer->SetMaterial(m_pMaterial.get());

	}

	SimpleGame::~SimpleGame()
	{
	}

	void SimpleGame::Run()
	{
#if SPACE_PLATFORM == SPACE_WIN32
		// Main message loop
		MSG msg = { 0 };
		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
#elif
		while(true)
		{			
#endif
			MainLoop();
		}
	}

	void SimpleGame::MainLoop()
	{
		auto cmdList = std::async(std::launch::async,
			[this]{ return this->RenderOneFrame(); }
		);
		Update();

		m_pRenderSys->ExecuteCommandList(cmdList.get());
		m_pRenderWindow->Present();
	}

	CommandList* SimpleGame::RenderOneFrame()
	{
		m_pGraphicCommandList->ClearRenderTargetView(
			m_pRenderTarget.get(),
			Float4{ 0.12f, 0.15f, 0.55f, 1.0f });

		m_pSimpleRenderer->Render(m_pGraphicCommandList.get());

		m_pGraphicCommandList->Close();
		return m_pGraphicCommandList.get();
	}

	void SimpleGame::Close()
	{
	}

	void SimpleGame::Update()
	{

	}

}

#ifdef SPACE_GAME
using namespace Space;

int main(int argc, char* argv[])
{
	{
		SimpleGame game;
		game.Run();
	}
	_CrtDumpMemoryLeaks();
	return 0;
}
#endif