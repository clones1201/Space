#ifndef __SPACE_MESH_HPP__
#define __SPACE_MESH_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"

#include "DeviceBuffer.hpp"
#include "InputLayout.hpp"

namespace Space
{

	class MeshPart : public Uncopyable
	{
	public:
		inline float GetSize() const
		{
			return m_Size;
		}
		inline Float3 GetCenter() const
		{
			return m_Center;
		}
	private:
		VertexBufferPtr m_pVertexBuffer;
		IndexBufferPtr m_pIndexBuffer;

		InputLayoutPtr m_pInputLayout;
		
		float m_Size;
		Float3 m_Center;
		
		friend class Mesh;
	};
	typedef std::shared_ptr<MeshPart> MeshPartPtr;

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
		
		std::list<MeshPartPtr> m_PartCollection;
	};

	typedef std::shared_ptr<Mesh> MeshPtr;
}

#endif