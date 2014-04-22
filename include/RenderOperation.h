#ifndef __SPACE_RENDEROPERATION_H__
#define __SPACE_RENDEROPERATION_H__

#include "basic.h"

namespace space{
	namespace graphic{
		class RenderOperation : virtual Uncopyable{
		protected:
			//VertexData* vertexData;
			//VertexIndices* vertexIndices;

			bool isIndexed;
		};
	}
}

#endif