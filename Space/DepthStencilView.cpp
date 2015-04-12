#include "Log.h"
#include "DeviceTexture.hpp"
#include "DepthStencilView.hpp"
#include "RenderSystem.hpp"

namespace Space
{
	DepthStencilView* DepthStencilView::Create(RenderSystem* pRenderSys, DeviceTexture2D* pTexture)
	{
		auto format = pTexture->GetFormat();
		switch (format)
		{
		default:
			return nullptr;
		case DF_D24_UNORM_S8_UINT:
		case DF_D32_FLOAT:
			break;
		}
		pRenderSys->CreateDepthStencilView(pTexture);
	}

	DepthStencilView::~DepthStencilView(){}
}