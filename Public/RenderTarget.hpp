#ifndef __SPACE_RENDERTARGET_HPP__
#define __SPACE_RENDERTARGET_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"

namespace Space
{
	class SPACE_API RenderTarget : virtual public Interface
	{
	public:		
		virtual ~RenderTarget();

	protected:
	};

	typedef std::shared_ptr<RenderTarget> RenderTargetPtr;
}

#endif 