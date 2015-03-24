#ifndef __SPACE_RENDERABLE_HPP__
#define __SPACE_RENDERABLE_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"

namespace Space
{

	class Renderable : public Interface 
	{
	private:
		
	public:

	};

	class RenderOperation
	{
	public:
		VertexBuffer* GetVertexBuffer();
		IndexBuffer* GetIndexBuffer();

	};

}

#endif