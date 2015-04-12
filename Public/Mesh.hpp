#ifndef __SPACE_MESH_HPP__
#define __SPACE_MESH_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"

namespace Space
{

	class Mesh
	{
	public:
		static Mesh* Create(RenderSystem* pRenderSys);


	protected:
		TypeTrait<VertexBuffer>::Ptr m_pVertexBuffer;
		TypeTrait<IndexBuffer>::Ptr m_pIndexBuffer;

		TypeTrait<InputLayout>::Ptr m_pInputLayout;

	};

}

#endif