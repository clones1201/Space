#ifndef __SPACE_INPUTLAYOUT_HPP__
#define __SPACE_INPUTLAYOUT_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"

namespace Space
{
	class InputLayout
	{
	public:		
		static InputLayout* Create(RenderSystem* pRenderSys);
		virtual void Complete(byte const* pInputSignature, uint32 lengthInBytes) = 0;

		bool AddElem(VertexElemType type, ElemSemantic semantic);
		bool RemoveElem(int32 idx);
		bool IsCompleted() const;

		VertexElemType GetElemTypeByIndex(int32 idx) const;
		ElemSemantic GetSemanticByIndex(int32 idx) const;
		int32 GetOffsetByIndex(int32 idx) const;
		int32 GetSizeInByteByIndex(int32 idx) const;

		int32 GetElemCount() const;
		int32 GetPositionCount() const;
		int32 GetTexCoordCount() const;
		int32 GetNormalCount() const;
		int32 GetTangentCount() const;

		virtual ~InputLayout();
	protected:
		InputLayout();
		void _Complete();

		struct Description
		{
			VertexElemType type;
			ElemSemantic semantic;
			int32 semanticId;
			int32 offset;
		};
		std::vector<Description> m_LayoutVector;
		int32 m_PositionCount = 0;
		int32 m_TexCoordCount = 0;
		int32 m_NormalCount = 0;
		int32 m_TangentCount = 0;

		int32 m_SizeOfVertex = 0;

		bool m_IsCompleted = false;
	};
}

#endif