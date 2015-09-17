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

		std::fstream signatureFile("StandardInputSignature.cso", std::ios_base::in | std::ios_base::binary);
		if (!signatureFile.is_open())
		{
			throw std::exception("Missing Input Signature File");
		}
		std::vector<char> signatureCode(
			(std::istreambuf_iterator<char>(signatureFile)),
			(std::istreambuf_iterator<char>()));

		std::unique_ptr<InputLayout> pInputLayout(new InputLayout{
			{ VertexElemType::Float3, ElemSemantic::Position, 0, 0, ElementClass::PerVertex, 0 },
			{ VertexElemType::Float3, ElemSemantic::Normal, 0, 0, ElementClass::PerVertex, 0 },
			{ VertexElemType::Float3, ElemSemantic::Tangent, 0, 0, ElementClass::PerVertex, 0 },
			{ VertexElemType::Float2, ElemSemantic::TexCoord, 0, 0, ElementClass::PerVertex, 0 },
		});

		m_pPipelineState->SetInputLayout(pInputLayout.get(),(byte const*) signatureCode.data(), signatureCode.size());
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
			m_pPipelineState->SetShaders(m_pMaterial->GetShader());

			list->SetPipelineState(m_pPipelineState.get());

			list->DrawIndexed(0, pPart->GetNumPrimitives());
		}
	}

}