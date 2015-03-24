#ifndef __SPACE_DEPTHSTENCIL_HPP__
#define __SPACE_DEPTHSTENCIL_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"

namespace Space
{

	class DepthStencilView : virtual public Interface
	{
	protected:
		DepthStencilView();

		virtual ~DepthStencilView();
	public:

		friend class DeviceTexture;
	};
}

#endif