#include "RenderSystem.h"

namespace Space
{	
namespace Render 
{
namespace Details
{
#ifdef SPACE_WIN32
	DeviceImpl<D3D11RenderSystem>* GetDeviceInstance()
	{
		static DeviceImpl<D3D11RenderSystem> _Instance;
		return &_Instance;		
	}
#endif

}
}
}