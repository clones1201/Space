#include "Log.h"
#include "Utility.hpp"
#include "InputLayout.hpp"
#include "RenderSystem.hpp"

namespace Space
{
	bool InputLayout::AddElem(VertexElemType type, ElemSemantic semantic)
	{
		int32 idx = 0;
		switch (semantic)
		{
		case ES_Position:
			idx = m_PositionCount++;
			break;
		case ES_Normal:
			idx = m_NormalCount++;
			break;
		case ES_TexCoord:
			idx = m_TexCoordCount++;
			break;
		case ES_Tangent:
			idx = m_TangentCount++;
			break;
		case ES_Unknown:
		default:
			return false;
		}
		int32 offset = m_SizeOfVertex;
		m_SizeOfVertex += GetElementSize(type);
		m_LayoutVector.push_back(Description{ type, semantic, idx, offset });
		m_IsCompleted = false;
		return false;
	}
	bool InputLayout::RemoveElem(int32 idx)
	{
		auto target = m_LayoutVector.begin();
		std::advance(target, idx);
		switch (target->semantic)
		{
		case ES_Position:
			m_PositionCount -= 1;
			break;
		case ES_Normal:
			m_NormalCount -= 1;
			break;
		case ES_TexCoord:
			m_TexCoordCount -= 1;
			break;
		case ES_Tangent:
			m_TangentCount -= 1;
			break;
		case ES_Unknown:
		default:
			return false;
		}
		for (auto iter = target; iter != m_LayoutVector.end(); ++iter)
		{
			if (iter->semantic = target->semantic)
				iter->semanticId--;
		}
		m_LayoutVector.erase(target);
		m_IsCompleted = false;
		return true;
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

	int32 InputLayout::GetVertexSize() const
	{
		return m_SizeOfVertex;
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

	void InputLayout::_Complete()
	{
		m_IsCompleted = true;
	}
	
	bool InputLayout::IsCompleted() const
	{
		return m_IsCompleted;
	}

	InputLayout::InputLayout()
	{
	}

	InputLayout::~InputLayout()
	{
	}

	InputLayout* InputLayout::Create(RenderSystem* pRenderSys)
	{
		try
		{
			return pRenderSys->CreateInputLayout();
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}
}