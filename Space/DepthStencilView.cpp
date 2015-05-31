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
		case DataFormat::D24_UNORM_S8_UINT:
		case DataFormat::D32_FLOAT:
			break;
		}
		return pRenderSys->CreateDepthStencilView(pTexture);
	}

	DepthStencilView::~DepthStencilView(){}
}