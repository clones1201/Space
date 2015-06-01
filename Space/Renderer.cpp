#include "Log.h"
#include "RenderSystem.hpp"
#include "Rendering.hpp"
#include "Renderer.hpp"

namespace Space
{
	Renderer::Renderer(RenderSystem* pRenderSys)
	{
		m_pPipelineState.reset(
			pRenderSys->CreatePipelineState());
	}

	void Renderer::SetRenderTarget(RenderTarget* target)
	{
		m_pRenderTarget = target;
	}

	MaterialRenderer::MaterialRenderer(RenderSystem* pRenderSys)
		:Renderer(pRenderSys)
	{
	}

	void MaterialRenderer::SetMaterial(Material* material)
	{
		m_pMaterial = material;
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

	void MeshMaterialRenderer::SetMesh(Mesh* pMesh)
	{
		m_pMesh = nullptr;
	}

	void MeshMaterialRenderer::Render(CommandList* list)
	{
		
	}

}