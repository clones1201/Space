#include "Log.h"
#include "Utility.h"
#include "InputLayout.hpp"

namespace Space
{
	bool InputLayout::AddElem(VertexElemType type, ElemSemantic semantic)
	{
		switch (semantic)
		{
		case ES_Position:
			m_PositionCount += 1;
			break;
		case ES_Normal:
			m_NormalCount += 1;
			break;
		case ES_Texcoord:
			m_TexCoordCount += 1;
			break;
		case ES_Tangent:
			m_TangentCount += 1;
			break;
		case ES_Unknown:
		default:
			return false;
		}
		int32 offset = m_SizeOfVertex;
		m_SizeOfVertex += GetElementSize(type);
		m_LayoutVector.push_back(Description{ type, semantic, offset });
	}
	bool InputLayout::RemoveElem(int32 idx)
	{
		auto target = m_LayoutVector.begin();
		switch (target->type)
		{
		case ES_Position:
			m_PositionCount -= 1;
			break;
		case ES_Normal:
			m_NormalCount -= 1;
			break;
		case ES_Texcoord:
			m_TexCoordCount -= 1;
			break;
		case ES_Tangent:
			m_TangentCount -= 1;
			break;
		case ES_Unknown:
		default:
			return false;
		}
		std::advance(target, idx);
		m_LayoutVector.erase(target);
	}

	VertexElemType InputLayout::GetElemTypeByIndex(int32 idx) const
	{
		return (VertexElemType)m_LayoutVector.at(idx).type;		
	}
	ElemSemantic InputLayout::GetSemanticByIndex(int32 idx) const
	{
		return (ElemSemantic)m_LayoutVector.at(idx).semantic;
	}
	
	int32 InputLayout::GetOffsetByIndex(int32 idx) const
	{
		return m_LayoutVector.at(idx).offset;
	}
	int32 InputLayout::GetSizeInByteByIndex(int32 idx) const
	{
		return GetElementSize(m_LayoutVector.at(idx).type);
	}

	int32 InputLayout::GetElemCount() const
	{
		return m_LayoutVector.size();
	}
	int32 InputLayout::GetPositionCount() const
	{
		return m_PositionCount;
	}
	int32 InputLayout::GetTexCoordCount() const
	{
		return m_TexCoordCount;
	}
	int32 InputLayout::GetNormalCount() const
	{
		return m_NormalCount;
	}

	InputLayout::InputLayout()
	{
	}

	InputLayout::~InputLayout()
	{
	}
}