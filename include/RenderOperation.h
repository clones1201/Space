#ifndef __SPACE_RENDEROPERATION_H__
#define __SPACE_RENDEROPERATION_H__

#include "basic.h"

namespace space{
	namespace graphic{

		typedef enum _PrimitiveType{
			SP_POINTS = 0x0000, SP_LINES = 0x0001,
			SP_TRIANGLES = 0x0004, SP_TRIANGLES_STRIP = 0x0005, SP_TRIANGLES_FAN = 0x0006
		}PrimitiveType;

		class RenderOperation : virtual Uncopyable{
		protected:
			//VertexData* vertexData;
			//VertexIndices* vertexIndices;

			bool isIndexed;
		};
	}
}

#endif