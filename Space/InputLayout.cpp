#include "Log.h"
#include "Utility.hpp"
#include "InputLayout.hpp"

namespace Space
{

	InputLayout::InputLayout(std::initializer_list<InputElement> list)
		:m_LayoutArray(list){

		for (auto iter = m_LayoutArray.begin();
			iter != m_LayoutArray.end();
			++iter)
		{
			switch (iter->Semantic)
			{
			case ElemSemantic::Position:	{	m_PositionCount++;	}	goto Valid;
			case ElemSemantic::Normal:      {	m_NormalCount++;	}	goto Valid;
			case ElemSemantic::Tangent:		{	m_TangentCount++;	}	goto Valid;
			case ElemSemantic::TexCoord:	{	m_TexCoordCount++;	}	goto Valid;
			case ElemSemantic::Unknown:
			default:
				goto Invalid;
			}

		Valid:
			try
			{
				auto& offset = m_StridePerSlot.at(iter->InputSlot);
				iter->AlignedByteOffset = offset;
				offset = Alignment(offset + GetElementSize(iter->Type));
			}
			catch (std::out_of_range&)
			{
				m_StridePerSlot[iter->InputSlot] = GetElementSize(iter->Type);
				iter->AlignedByteOffset = 0;
			}
		Invalid:
			continue;
		}
	}

	void InputLayout::Insert(ElementArray::iterator pos, InputElement elem)
	{
		m_LayoutArray.insert(pos, elem);
	}

	uint32 InputLayout::GetVertexStride(uint32 slot) const
	{
		return m_StridePerSlot.at(slot);
	}

	uint32 InputLayout::GetSize() const
	{
		return m_LayoutArray.size();
	}

	InputLayout::~InputLayout()
	{
	}
}