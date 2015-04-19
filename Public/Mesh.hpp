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
		
		float m_Size;
		Float3 m_Center;
		
		friend class Mesh;
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

		MeshPart* CreatePart();
		void RemovePart(MeshPart* pPart);
	protected:
		
		std::list<TypeTrait<MeshPart>::Ptr> m_PartCollection;
	};

}

#endif