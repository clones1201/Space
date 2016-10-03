#include "Common/Log.h"

#include "RenderSystem/RenderSystem.hpp"

#include "Space/Material.hpp"
#include "Space/Mesh.hpp"
#include "Space/Renderer.hpp"

namespace Space
{
	Renderer::Renderer(RenderSystem* pRenderSys)
	{
	}

	MaterialRenderer::MaterialRenderer(RenderSystem* pRenderSys)
		:Renderer(pRenderSys)
	{
}

	void MaterialRenderer::SetMaterial(Material* material)
	{
		m_pMaterial = material;

		//m_pPipelineState->SetShaders(m_pMaterial->GetShader());
	}

	MeshMaterialRenderer* MeshMaterialRenderer::Create(RenderSystem* pRenderSys)
	{
		TRY_CATCH_LOG(
			return new MeshMaterialRenderer(pRenderSys),
			return nullptr;
		);
	}

	MeshMaterialRenderer::MeshMaterialRenderer(RenderSystem* pRenderSys)
		:MaterialRenderer(pRenderSys){
	}

	void MeshMaterialRenderer::Render(CommandList* list, Mesh* pMesh)
	{
		ViewPort ports[] =
		{
			ViewPort{
				0.0f, 0.0f,
				(float)m_pRenderTarget->GetWidth(), (float)m_pRenderTarget->GetHeight(),
				0.0f, 1.0f
			}
		};
		list->SetViewPorts(ports, 1);

		for (auto part = pMesh->CBegin();
			part != pMesh->CEnd(); ++part)
		{
			MeshPart* pPart = (*part).get();

			list->SetIndexBuffer(pPart->GetIndexBuffer());

			VertexBuffer* buffers[] = {
				pPart->GetVertexBuffer()
			};
			list->SetVertexBuffers(0, 1, buffers);
			m_pPipelineState->SetShaders(m_pMaterial->GetShader(list));

			list->SetPipelineState(m_pPipelineState.get());

			list->DrawIndexed(0, pPart->GetNumPrimitives());
		}
	}

}