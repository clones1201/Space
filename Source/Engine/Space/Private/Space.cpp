#include "Space.hpp"
#include "RenderSystem.hpp"
#include "DeviceResource.hpp"
#include "RenderTarget.hpp"
// #include "Utility.hpp"
// #include "Renderer.hpp"

namespace Space
{
	/*
	struct PerFrameData
	{
		Float4x4 View;
		Float4x4 Projection;
	} ALIGNED_ALLOCATED;

	struct PerObjectData
	{
		Float4x4 World;
	} ALIGNED_ALLOCATED;
	*/
	using namespace Render;

	SimpleGame::SimpleGame()
	{
		// renderer = std::make_unique<Render::Renderer>();

		auto device = Render::Device::GetInstance();

		m_pFrontCommandList = std::make_unique<Render::CommandList>(device);
		m_pBackCommandList = std::make_unique<Render::CommandList>(device);

		m_pRenderWindow = std::make_unique<Render::RenderWindow>(
			device,
			"RenderWindow", 500, 500, false);

		auto pBackBuffer = m_pRenderWindow->GetBackBuffer();
		m_pRenderTarget = std::make_unique<Render::RenderTarget>(
			device,
			pBackBuffer);
		_Width = pBackBuffer->GetWidth();
		_Height = pBackBuffer->GetHeight();
		m_pDepthBuffer = std::make_unique<Render::Texture2D>(device, _Width , _Height, 1,
		                                                     DataFormat::D24_UNORM_S8_UINT, ResourceUsage::Default, ResourceBindFlag::DepthStencil);
		m_pDepthStencilView = std::make_unique<Render::DepthStencilView>(device, m_pDepthBuffer.get());

		m_pRenderWindow->Show();

		// PipelineStateManager::Instance().CreateOpaqueDefault();

		// m_pPipelineState.reset(new Render::PipelineState(device));

		// std::ifstream shaderfile("./Assets/Material/default/Shader0.hlsl");
		// auto shaderCode = std::make_shared<std::string>((std::istreambuf_iterator<char>(shaderfile)),
			// std::istreambuf_iterator<char>());

		// auto macros = std::make_shared<Render::ShaderMacro>();
		// macros->DefineBool("TRUE", true);
		// macros->DefineBool("FALSE", false);
		// macros->DefineFloat("MACRO_TEST_FLOAT", 3.1415926f);
		// macros->DefineFloat("MACRO_TEST_INT", 42);

		// Render::ShaderSource vs_source = 
		// {
			// "Assets/Material/default/Shader0.hlsl",
			// "5_0",
			// "MainVS",
			// macros,
			// shaderCode
		// };
		
		// Render::ShaderSource ps_source =
		// {
			// "Assets/Material/default/Shader0.hlsl",
			// "5_0",
			// "MainPS",
			// macros,
			// shaderCode
		// };

		// _vs.reset(new Render::VertexShader(vs_source));
		// _ps.reset(new Render::PixelShader(ps_source));
		// _mesh.reset(Space::Mesh::CreateCube(1.0f, 1.0f, 1.0f));

		// Render::InputLayout const* layout = (*_mesh->Begin())->GetInputLayout();
		// m_pPipelineState->SetInputLayout(layout->Data(),layout->Size());

		// _PerFrameData.reset(new Render::ConstantBuffer(sizeof(PerFrameData), nullptr));
		// _PerObjectData.reset(new Render::ConstantBuffer(sizeof(PerObjectData), nullptr));
		
		// Matrix projection = MatrixPerspectiveFovLH(Pi / 4.0f, 1.0f, 0.1f, 100.0f);
		// projection = MatrixTranspose(projection);
		// _PerFrameData->Update(m_pFrontCommandList.get(), sizeof(Float4x4), sizeof(Float4x4),
			// reinterpret_cast<byte const*>(&projection));
		
		// m_pFrontCommandList->GetContext()->Flush();
		// std::vector<D3D11RenderTarget*> targets =
		// {
			// m_pRenderTarget.get()
		// };
		// m_pFrontCommandList->SetRenderTargets(
			// targets, m_pDepthStencilView.get()
		// );
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
			Logic();
			Update();
			Render();
		}
	}

	void SimpleGame::Render()
	{
		m_pFrontCommandList->ClearRenderTargetView(
			m_pRenderTarget.get(),
			Float4{ 0.12f, 0.15f, 0.55f, 1.0f });
		m_pFrontCommandList->ClearDepth(m_pDepthStencilView.get(), 1.0);
		m_pFrontCommandList->ClearStencil(m_pDepthStencilView.get(), 0);

		m_pFrontCommandList->SetPipelineState(m_pPipelineState.get());
		
		Render::ViewPort viewports[] = {
			{0.0, 0.0, (float)_Width, (float)_Height, 0.0, 1.0 }
		};
		m_pFrontCommandList->SetViewPorts(viewports, 1);
		
		// m_pFrontCommandList->SetVertexShader(_vs.get());
		// m_pFrontCommandList->SetPixelShader(_ps.get());
		// ID3D11Buffer* buffers[] = {
			// _PerFrameData->GetBuffer()->GetRawPtr().p, _PerObjectData->GetBuffer()->GetRawPtr().p
		// };
		// m_pFrontCommandList->GetContext()->VSSetConstantBuffers(0, 2, buffers);
		// m_pFrontCommandList->SetVertexShader();
		// m_pSimpleRenderer->Render(m_pFrontCommandList.get(),m_pMesh.get());
		// for(auto part = _mesh->Begin(); part != _mesh->End(); ++part)
		// {
			// m_pFrontCommandList->DrawPrimitives(
				// *(*part)->GetVertexBuffer(),
				// *(*part)->GetIndexBuffer(),
				// 0, (*part)->GetNumPrimitives()
			// );
		// }

		m_pFrontCommandList->Close();
		
		auto device = Render::Device::GetInstance();
		device->GetImmediateContext()->ExecuteCommandList(m_pFrontCommandList->GetList(), false);

		m_pRenderWindow->Present();
	}

	void SimpleGame::Update()
	{
		// PerFrameData perFrameData;
		// PerObjectData perObjectData;

		static uint count = 0;
		count = (count + 1) % 360;
		Matrix mWorld = MatrixTranslation(0.0f, 0.0f, 0.0f);
		Vector lookAt = VectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		Vector eye = VectorSet(
			50.0f * std::sin(2 * Pi * (count / 360.0f)),
			0.0f,
			50.0f * std::cos(2 * Pi * (count / 360.0f)), 
			1.0f);
		Matrix mView = MatrixLookAtLH(
			eye,
			lookAt,
			VectorSet(0.0f, 1.0f, 0.0f, 1.0f));

		mWorld = MatrixTranspose(mWorld);
		mView = MatrixTranspose(mView);

		// StoreFloat4x4(&perObjectData.World, mWorld);
		// StoreFloat4x4(&perFrameData.View, mView);

		// _PerFrameData->Update(m_pFrontCommandList.get(), 0, sizeof(Float4x4),
			// reinterpret_cast<byte const*>(&perFrameData.View));
		// _PerObjectData->Update(m_pFrontCommandList.get(), 0, sizeof(Float4x4),
			// reinterpret_cast<byte const*>(&perObjectData.World));

		// _PerFrameData->UpdateToDevice(m_pFrontCommandList.get());
		// _PerObjectData->UpdateToDevice(m_pFrontCommandList.get());
	}

	void SimpleGame::Logic()
	{
		
	}

	void SimpleGame::Close()
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