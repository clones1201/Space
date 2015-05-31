#ifndef __SPACE_INPUTLAYOUT_HPP__
#define __SPACE_INPUTLAYOUT_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"

namespace Space
{
	enum class ElementClass : uint8
	{
		PerVertex, PerInstance
	};

	class InputLayout
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

		ElementArray::iterator Begin();
		ElementArray::iterator End();
		ElementArray::const_iterator CBegin() const;
		ElementArray::const_iterator CEnd() const;

		void Insert(ElementArray::iterator pos, InputElement elem);

		uint32 GetVertexStride(uint32 slot) const;
		uint32 GetSize() const;
		uint32 GetPositionCount() const;
		uint32 GetTexCoordCount() const;
		uint32 GetNormalCount() const;
		uint32 GetTangentCount() const;

		virtual ~InputLayout();
	protected:
		ElementArray m_LayoutArray;
		uint32 m_PositionCount = 0;
		uint32 m_TexCoordCount = 0;
		uint32 m_NormalCount = 0;
		uint32 m_TangentCount = 0;

		std::map<uint32, uint32> m_StridePerSlot;

	};
}

#endif