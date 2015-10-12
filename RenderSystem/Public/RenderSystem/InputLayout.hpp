#ifndef __SPACE_RENDERSYSTEM_INPUTLAYOUT_HPP__
#define __SPACE_RENDERSYSTEM_INPUTLAYOUT_HPP__

#include "RenderSystem/Prerequisites.hpp"
#include "RenderSystem/Shared.hpp"

namespace Space
{
	enum class SPACE_RENDERSYSTEM_API ElementClass : uint8
	{
		PerVertex, PerInstance
	};

	class SPACE_RENDERSYSTEM_API InputLayout
	{
	public:
		struct InputElement
		{
			VertexElemType Type;
			ElemSemantic Semantic;
			uint32 SemanticIdx;
			uint32 InputSlot;
			ElementClass ElemClass;
			uint32 InstanceStep;
			uint32 AlignedByteOffset; //leave this field 0, will be calculated
		};

		typedef std::vector<InputElement> ElementArray;

		InputLayout(std::initializer_list<InputElement> list);
		InputLayout(InputLayout const& other) = default;
		InputLayout& operator=(InputLayout const& other) = default;

		inline ElementArray::iterator Begin()
		{
			return m_LayoutArray.begin();
		}
		inline ElementArray::iterator End()
		{
			return m_LayoutArray.end();
		}
		inline ElementArray::const_iterator CBegin() const
		{
			return m_LayoutArray.cbegin();
		}
		inline ElementArray::const_iterator CEnd() const
		{
			return m_LayoutArray.cend();
		}

		void Insert(ElementArray::iterator pos, InputElement elem);
		uint32 GetVertexStride(uint32 slot) const;
		uint32 GetSize() const;

		inline uint32 GetPositionCount() const
		{
			return m_PositionCount;
		}
		inline uint32 GetTexCoordCount() const
		{
			return m_TexCoordCount;
		}
		inline uint32 GetNormalCount() const
		{
			return m_NormalCount;
		}
		inline uint32 GetTangentCount() const
		{
			return m_TangentCount;
		}

		virtual ~InputLayout();
	protected:
		ElementArray m_LayoutArray;
		uint32 m_PositionCount = 0;
		uint32 m_TexCoordCount = 0;
		uint32 m_NormalCount = 0;
		uint32 m_TangentCount = 0;

		std::map<uint32, uint32> m_StridePerSlot;

	};
	typedef std::shared_ptr<InputLayout> InputLayoutPtr;
}

#endif