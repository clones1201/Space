#include "Log.h"
#include "RenderTarget.hpp"
#include "RenderSystem.hpp"
#include "Rendering.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "Renderer.hpp"

namespace Space
{
	Renderer::Renderer(RenderSystem* pRenderSys)
	{
		m_pPipelineState.reset(
			pRenderSys->CreatePipelineState());

		m_pPipelineState->SetPrimitiveTopology(PrimitiveTopology::TriangleList);
		m_pPipelineState->SetInputLayout(,);
	}

	MaterialRenderer::MaterialRenderer(RenderSystem* pRenderSys)
		:Renderer(pRenderSys)
	{
	}

	void MaterialRenderer::SetMaterial(Material* material)
	{
		m_pMaterial = material;

		m_pPipelineState->SetShaders(m_pMaterial->GetShader());
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
				m_pRenderTarget->GetWidth(), m_pRenderTarget->GetHeight(),
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
			m_pPipelineState->SetShaders(m_pMaterial->GetShader());

			list->SetPipelineState(m_pPipelineState.get());

			list->DrawIndexed(0, pPart->GetNumPrimitives());
		}
	}

}