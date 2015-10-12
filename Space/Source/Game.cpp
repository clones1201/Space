#include "Space/Game.hpp"

#include "RenderSystem/D3D11RenderSystem.hpp"

namespace Space
{
	SimpleGame::SimpleGame()
	{
		m_pRenderSys.reset(
			D3D11RenderSystem::Create());
		m_pFrontCommandList.reset(
			m_pRenderSys->CreateCommandList());
		m_pBackCommandList.reset(
			m_pRenderSys->CreateCommandList());

		m_pRenderWindow.reset(
			m_pRenderSys->CreateRenderWindow("RenderWindow", 500, 500, false));
		m_pRenderWindow->Initialize();

		m_pRenderTarget.reset(
			m_pRenderSys->CreateRenderTarget(m_pRenderWindow->GetBackBuffer()));

		m_pMaterial.reset(Material::Create(m_pRenderSys.get(), TEXT("simplecolor")));
		m_pMesh.reset(Mesh::CreateFromFBX(m_pRenderSys.get(), TEXT("bunny.fbx")));

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
		m_CmdList = std::async(std::launch::async,
			[this] { return this->RenderOneFrame(); }
		);
		Update();
		
		if(m_RenderAction.valid())
			m_RenderAction.get();
		
		CommandList* pList = m_CmdList.get();
		std::swap(m_pFrontCommandList,m_pBackCommandList);

		m_RenderAction = std::async(std::launch::async, 
			[this,pList] { return this->ExecuteRender(pList); });
	}

	int SimpleGame::ExecuteRender(CommandList* pList){		
		m_pRenderSys->ExecuteCommandList(pList);
		m_pRenderWindow->Present();
		return 1;
	}

	CommandList* SimpleGame::RenderOneFrame()
	{
		RenderTarget* targets[] = 
		{
			m_pSimpleRenderer->GetRenderTarget()
		};
		m_pFrontCommandList->SetRenderTargets(
			targets,1,nullptr
			);

		m_pFrontCommandList->ClearRenderTargetView(
			m_pRenderTarget.get(),
			Float4{ 0.12f, 0.15f, 0.55f, 1.0f });

		m_pSimpleRenderer->Render(m_pFrontCommandList.get(),m_pMesh.get());

		m_pFrontCommandList->Close();
		return m_pFrontCommandList.get();
	}

	void SimpleGame::Close()
	{
	}

	void SimpleGame::Update()
	{ 
		Float4x4 world;
		Float4x4 view;
		Float4x4 project;

		static uint count = 0;
		count = (count + 1) % 360;
		Matrix mWorld = MatrixTranslation(0.0f, 0.0f, 0.0f);
		Vector lookAt = VectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		Vector eye = VectorSet(
			10.0f,// * std::sin(count / 360.0f),
			0.0f,
			10.0f,// * std::cos(count / 360.0f), 
			1.0f);
		Matrix mView = MatrixLookToLH(
			eye, 
			lookAt - eye,
			VectorSet(0.0f, 1.0f, 0.0f, 1.0f));
		Matrix mProject = MatrixPerspectiveFovLH(
			Pi / 3.0f,
			m_pRenderTarget->GetWidth() / (float)m_pRenderTarget->GetHeight(),
			0.1f,1000.0f);
		
		StoreFloat4x4(&world, mWorld);
		StoreFloat4x4(&view, mView);
		StoreFloat4x4(&project, mProject);

		m_pMaterial->SetGameTime(123.111f);
		m_pMaterial->SetWorld(world);
		m_pMaterial->SetView(view);
		m_pMaterial->SetProjection(project);

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