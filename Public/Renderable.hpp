#ifndef __SPACE_RENDERABLE_HPP__
#define __SPACE_RENDERABLE_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"

namespace Space
{
	class InputLayout
	{

	};

	class Mesh : public Uncopyable
	{
	protected:
		std::unique_ptr<VertexBuffer> m_pVertexBuffer;
		std::unique_ptr<IndexBuffer> m_pIndexBuffer;
		InputLayout m_InputLayout;

		RenderSystem* m_pRenderSys;

		Mesh(RenderSystem* pRenderSys);
	public:
		static Mesh* CreateFromFile(RenderSystem* pRenderSys, std::string const& filename);

		virtual ~Mesh();
		VertexBuffer const* GetVertexBuffer() const;
		IndexBuffer const* GetIndexBuffer() const;

	};
}

#endif