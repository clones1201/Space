#pragma once 
#include "Common/Common.hpp"

#include "Prerequisites.hpp"

#if SPACE_PLATFORM == SPACE_WIN32
#include "D3D11RenderSystem/D3D11RenderSystem.hpp"
#endif

namespace Space
{
	namespace Render {

		namespace Details {
			template<class RenderSystem>
			class SPACE_RENDERSYSTEM_API DeviceImpl : public RenderSystem::Device
			{
			public:
				virtual ~DeviceImpl()
				{
				}

				static DeviceImpl* GetInstance()
				{
					return &_Instance;
				}
			private:
				static DeviceImpl _Instance;

				DeviceImpl()
				{
				}
			};
		}
	}
}