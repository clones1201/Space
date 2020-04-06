#pragma once

#include "RenderCommon.hpp"
#include "Shared.hpp"

namespace Space {
	namespace Render {

		class RENDERCORE_API InputLayout
		{
		public:
			InputLayout(std::initializer_list<InputElement> list);
			InputLayout(InputLayout const& other) = default;
			InputLayout& operator=(InputLayout const& other) = default;
			InputLayout& operator=(InputLayout && other) = default;

			inline ElementArray::iterator Begin()
			{
				return _LayoutArray.begin();
			}
			inline ElementArray::iterator End()
			{
				return _LayoutArray.end();
			}
			inline ElementArray::const_iterator CBegin() const
			{
				return _LayoutArray.cbegin();
			}
			inline ElementArray::const_iterator CEnd() const
			{
				return _LayoutArray.cend();
			}
			inline InputElement const* Data() const
			{
				return _LayoutArray.data();
			}
			inline size_t Size() const
			{
				return _LayoutArray.size();
			}

			void Insert(ElementArray::iterator pos, InputElement elem);
			size_t GetVertexStride(size_t slot) const;
			size_t GetSize() const;

			inline size_t GetPositionCount() const
			{
				return _PositionCount;
			}
			inline size_t GetTexCoordCount() const
			{
				return _TexCoordCount;
			}
			inline size_t GetNormalCount() const
			{
				return _NormalCount;
			}
			inline size_t GetTangentCount() const
			{
				return _TangentCount;
			}

			virtual ~InputLayout();
		protected:
			ElementArray _LayoutArray;
			size_t _PositionCount = 0;
			size_t _TexCoordCount = 0;
			size_t _NormalCount = 0;
			size_t _TangentCount = 0;

			std::map<size_t, size_t> _StridePerSlot;
		};
		
	}
}