#ifndef __SPACE_MESH_HPP__
#define __SPACE_MESH_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"

namespace Space
{

	class MeshPart : public Uncopyable
	{
	public:

	private:
		TypeTrait<VertexBuffer>::Ptr m_pVertexBuffer;
		TypeTrait<IndexBuffer>::Ptr m_pIndexBuffer;

		TypeTrait<InputLayout>::Ptr m_pInputLayout;


	};

	class Mesh : public Uncopyable
	{
	public:
		static Mesh* CreateFromFBX(
			RenderSystem* pRenderSys,
			std::wstring const& filename);

		static Mesh* CreateFromFBX(
			RenderSystem* pRenderSys,
			std::string const& filename
			);
	protected:
		
	};

}

#endif