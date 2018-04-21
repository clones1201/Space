#include "Common/Log.h"

#include "RenderSystem/RenderSystem.hpp"

#include "RenderSystem/Material.hpp" 
#include "RenderSystem/Renderer.hpp"
#include "RenderSystem/RenderTarget.hpp"

namespace Space
{
	namespace Render {

		Renderer::Renderer()
		{
		}

		void Renderer::Render()
		{

			/*CommandList* pList = m_CmdList.get();
			std::swap(m_pFrontCommandList,m_pBackCommandList);

			m_RenderAction = std::async(std::launch::async,
			[this,pList] { return this->ExecuteRender(pList); });*/
		}

		void Renderer::Update()
		{
			/*Matrix mProject = MatrixPerspectiveFovLH(
			Pi / 3.0f,
			m_pRenderTarget->GetWidth() / (float)m_pRenderTarget->GetHeight(),
			0.1f,1000.0f);

			StoreFloat4x4(&world, mWorld);
			StoreFloat4x4(&view, mView);
			StoreFloat4x4(&project, mProject);

			m_pMaterial->SetGameTime(123.111f);
			m_pMaterial->SetWorld(world);
			m_pMaterial->SetView(view);
			m_pMaterial->SetProjection(project);*/

		}
	}
}