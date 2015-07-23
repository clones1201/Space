#ifndef __SPACE_RENDERER_HPP__
#define __SPACE_RENDERER_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"
#include "Rendering.hpp"

namespace Space
{
	class Renderer : virtual public Uncopyable
	{
	public:
		inline void SetRenderTarget(RenderTarget* target)
		{
			m_pRenderTarget = target;
		}

		inline RenderTarget* GetRenderTarget() const
		{
			return m_pRenderTarget;
		}
	protected:
		Renderer(RenderSystem* pRenderSys);

		std::unique_ptr<PipelineState> m_pPipelineState = nullptr;
		RenderTarget* m_pRenderTarget = nullptr;
	};

	typedef std::shared_ptr<Renderer> RendererPtr;

	class MaterialRenderer : public Renderer
	{	
	public:
		void SetMaterial(Material* matertial);
		
	protected:
		MaterialRenderer(RenderSystem* pRenderSys);

		Material* m_pMaterial;
	};

	typedef std::shared_ptr<MaterialRenderer> MaterialRendererPtr;

	class MeshMaterialRenderer : public MaterialRenderer
	{
	public:
		static MeshMaterialRenderer* Create(RenderSystem* pRenderSys);
		
		void Render(CommandList* list, Mesh* pMesh);
	protected:
		MeshMaterialRenderer(RenderSystem* pRenderSys);

		Mesh* m_pMesh = nullptr;
	};
	typedef std::shared_ptr<MeshMaterialRenderer> MeshMaterialRendererPtr;

}

#endif