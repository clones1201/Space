#include "Log.h"
#include "DeviceTexture.hpp"
#include "RenderSystem.hpp"

namespace Space
{
	DeviceTexture1D* DeviceTexture1D::CreateArray(RenderSystem* pRenderSys, int X, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int arraySize, byte const* initialData)
	{
		return pRenderSys->CreateTexture1DArray(X, format, usage, flag, arraySize, initialData);
	}
	DeviceTexture1D* DeviceTexture1D::Create(RenderSystem* pRenderSys, int X, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData)
	{
		return pRenderSys->CreateTexture1D(X, format, usage, flag, initialData);
	}

	DeviceTexture2D* DeviceTexture2D::CreateArray(RenderSystem* pRenderSys, int X, int Y, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int arraySize, byte const* initialData)
	{
		return pRenderSys->CreateTexture2DArray(X, Y, format, usage, flag, arraySize, initialData);
	}
	DeviceTexture2D* DeviceTexture2D::Create(RenderSystem* pRenderSys, int X, int Y, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData)
	{
		return pRenderSys->CreateTexture2D(X, Y, format, usage, flag, initialData);
	}
	DeviceTexture2D* DeviceTexture2D::CreateFromFile(RenderSystem* pRenderSys, std::string const &filename)
	{
		return pRenderSys->CreateTexture2DFromFile(filename);
	}

	/*DeviceTexture3D* DeviceTexture3D::CreateArray(RenderSystem* pRenderSys, int X, int Y, int Z, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int arraySize, byte const* initialData);*/
	DeviceTexture3D* DeviceTexture3D::Create(RenderSystem* pRenderSys, int X, int Y, int Z, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData)
	{
		return pRenderSys->CreateTexture3D(X, Y, Z, format, usage, flag, initialData);
	}

}