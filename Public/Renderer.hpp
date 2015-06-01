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
		void SetRenderTarget(RenderTarget* target); 
	protected:
		Renderer(RenderSystem* pRenderSys);

		std::unique_ptr<PipelineState> m_pPipelineState = nullptr;
		RenderTarget* m_pRenderTarget = nullptr;
	};

	class MaterialRenderer : public Renderer
	{	
	public:
		void SetMaterial(Material* matertial);
		
	protected:
		MaterialRenderer(RenderSystem* pRenderSys);

		Material* m_pMaterial;
	};

	class MeshMaterialRenderer : public MaterialRenderer
	{
	public:
		static MeshMaterialRenderer* Create(RenderSystem* pRenderSys);

		Mesh* GetMesh() const;
		void SetMesh(Mesh* pMesh);

		void Render(CommandList* list);
	protected:
		MeshMaterialRenderer(RenderSystem* pRenderSys);

		Mesh* m_pMesh = nullptr;
	};


}

#endif