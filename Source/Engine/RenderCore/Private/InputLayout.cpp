#include "InputLayout.hpp"
#include "Utility.hpp"

namespace Space
{
	namespace Render {

		InputLayout::InputLayout(std::initializer_list<InputElement> list)
			:_LayoutArray(list) {

			for (auto iter = _LayoutArray.begin();
				iter != _LayoutArray.end();
				++iter)
			{
				switch (iter->Semantic)
				{
				case ElemSemantic::Position: {	_PositionCount++;	}	goto Valid;
				case ElemSemantic::Normal: {	_NormalCount++;	}	goto Valid;
				case ElemSemantic::Tangent: {	_TangentCount++;	}	goto Valid;
				case ElemSemantic::TexCoord: {	_TexCoordCount++;	}	goto Valid;
				case ElemSemantic::Unknown:
				default:
					goto Invalid;
				}

			Valid:
				try
				{
					auto& offset = _StridePerSlot.at(iter->InputSlot);
					iter->AlignedByteOffset = (uint32)offset;
					offset = (size_t)(offset + GetElementSize(iter->Type));
				}
				catch (std::out_of_range&)
				{
					_StridePerSlot[iter->InputSlot] = GetElementSize(iter->Type);
					iter->AlignedByteOffset = 0;
				}
			Invalid:
				continue;
			}
		}

		void InputLayout::Insert(ElementArray::iterator pos, InputElement elem)
		{
			try
			{
				auto& offset = _StridePerSlot.at(elem.InputSlot);
				elem.AlignedByteOffset = (uint32)offset;
				offset = Alignment(offset + GetElementSize(elem.Type));
			}
			catch (std::out_of_range&)
			{
				_StridePerSlot[elem.InputSlot] = GetElementSize(elem.Type);
				elem.AlignedByteOffset = 0;
			}
			_LayoutArray.insert(pos, elem);
		}

		size_t InputLayout::GetVertexStride(size_t slot) const
		{
			return _StridePerSlot.at(slot);
		}

		size_t InputLayout::GetSize() const
		{
			return _LayoutArray.size();
		}

		InputLayout::~InputLayout()
		{
		}
	}
}
