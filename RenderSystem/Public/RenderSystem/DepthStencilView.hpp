#ifndef __SPACE_RENDERSYSTEM_DEPTHSTENCIL_HPP__
#define __SPACE_RENDERSYSTEM_DEPTHSTENCIL_HPP__

#include "RenderSystem/Prerequisites.hpp"

namespace Space
{
	class SPACE_RENDERSYSTEM_API DepthStencilView : private Interface
	{
	public:
		static DepthStencilView* Create(
			RenderSystem* pRenderSys, DeviceTexture2D* pTexture);
		virtual ~DepthStencilView();
	};

	typedef std::shared_ptr<DepthStencilView> DepthStencilViewPtr;
}

#endif