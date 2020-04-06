#ifndef __SPACE_MESH_HPP__
#define __SPACE_MESH_HPP__

#include "Space/Prerequisites.hpp"
#include <RenderSystem/InputLayout.hpp>

namespace Space
{

	class MeshPart : private Uncopyable
	{
	public:
		inline Float3 GetSize() const
		{
			return m_Size;
		}
		inline Float3 GetCenter() const
		{
			return m_Center;
		}
		inline Render::IndexBuffer* GetIndexBuffer() const
		{
			return m_pIndexBuffer.get();
		}
		inline Render::VertexBuffer* GetVertexBuffer() const
		{
			return m_pVertexBuffer.get();
		}
		inline Render::InputLayout* GetInputLayout() const
		{
			return m_pInputLayout.get();
		}
		inline uint32 GetNumPrimitives() const
		{
			return m_NumPrimitives;
		}

		bool Valid() const
		{
			return m_Loaded;
		}

	private:
		MeshPart();

		std::shared_ptr<Render::VertexBuffer> m_pVertexBuffer;
		std::shared_ptr<Render::IndexBuffer> m_pIndexBuffer;

		std::shared_ptr<Render::InputLayout> m_pInputLayout;
		
		Float3 m_Size = Float3(0.0f, 0.0f, 0.0f);
		Float3 m_Center = Float3(0.0f, 0.0f, 0.0f);
		uint32 m_NumPrimitives = 0;
		bool m_Loaded = false;
		
		friend class Mesh;
	};
	typedef std::shared_ptr<MeshPart> MeshPartPtr;

	class Mesh : public Uncopyable , public Object
	{
	public:
		static Mesh* CreateCube(float width, float height, float depth);
		static Mesh* CreateSphere(float radius, int32_t segments);
		static Mesh* CreateCone(float radius, float height, int32_t segments);
		static Mesh* CreateQuad(float width, float height);
		static Mesh* CreateFromFBX(			
			tstring const& filename);
		
		MeshPart* CreatePart();
		void RemovePart(MeshPart* pPart);

		typedef std::list<MeshPartPtr> MeshPartContainer;
		typedef MeshPartContainer::iterator MeshPartIterator;
		typedef MeshPartContainer::const_iterator MeshPartConstIterator;

		inline const tchar* GetFileName() const
		{
			return m_FileName.c_str();
		}

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

		bool Valid() const {
			return m_LoadAction.valid() && m_LoadAction.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
		}

		//void Render(CommandList* pCmdList);
	protected:
		Mesh();
		Mesh(const tstring& filename);

		void _LoadAsync();
		void _RealLoad();

		std::future<void> m_LoadAction;

		MeshPartContainer m_PartCollection;

		tstring m_FileName;
	};

	typedef std::shared_ptr<Mesh> MeshPtr;
}

#endif