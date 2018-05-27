#include "RenderSystem/RenderSystem.hpp"


namespace Space
{	
namespace Render {
namespace Details
{
#if SPACE_PLATFORM == SPACE_WIN32
	DeviceImpl<D3D11RenderSystem> DeviceImpl<D3D11RenderSystem>::_Instance;
#endif
}
}
}