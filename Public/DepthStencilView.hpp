#ifndef __SPACE_DEPTHSTENCIL_HPP__
#define __SPACE_DEPTHSTENCIL_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"

namespace Space
{

	class DepthStencilView : virtual public Interface
	{
	public:
		static DepthStencilView* Create(
			RenderSystem* pRenderSys, DeviceTexture2D* pTexture);
		virtual ~DepthStencilView();
	};
}

#endif