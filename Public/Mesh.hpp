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
		inline IndexBuffer* GetIndexBuffer() const
		{
			return m_pIndexBuffer.get();
		}
		inline VertexBuffer* GetVertexBuffer() const
		{
			return m_pVertexBuffer.get();
		}
		inline InputLayout* GetInputLayout() const
		{
			return m_pInputLayout.get();
		}
		inline uint32 GetNumPrimitives() const
		{
			return m_NumPrimitives;
		}
	private:
		MeshPart();

		VertexBufferPtr m_pVertexBuffer;
		IndexBufferPtr m_pIndexBuffer;

		InputLayoutPtr m_pInputLayout;
		
		float m_Size;
		Float3 m_Center;
		uint32 m_NumPrimitives;
		
		friend class Mesh;
	};
	typedef std::shared_ptr<MeshPart> MeshPartPtr;

	class Mesh : public Uncopyable , public Object
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

		typedef std::list<MeshPartPtr> MeshPartContainer;
		typedef MeshPartContainer::iterator MeshPartIterator;
		typedef MeshPartContainer::const_iterator MeshPartConstIterator;

		inline MeshPartIterator Begin()
		{
			return m_PartCollection.begin();
		}
		inline MeshPartConstIterator CBegin() const
		{
			return m_PartCollection.cbegin();
		}
		inline MeshPartIterator End()
		{
			return m_PartCollection.end();
		}
		inline MeshPartConstIterator CEnd() const
		{
			return m_PartCollection.cend();
		}
	protected:
		Mesh();

		MeshPartContainer m_PartCollection;
	};

	typedef std::shared_ptr<Mesh> MeshPtr;
}

#endif