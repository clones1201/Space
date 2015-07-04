#include "Log.h"
#include "DeviceTexture.hpp"
#include "RenderSystem.hpp"

namespace Space
{
	DeviceTextureBase::DeviceTextureBase()
	{}

	DeviceTextureBase::~DeviceTextureBase()
	{}

	DeviceTexture1D::DeviceTexture1D(
		int32 X,
		DataFormat format,
		ResourceUsage usage,
		ResourceBindFlag flag,
		int32 arraySize) :
		m_Width(X), m_ArraySize(arraySize)
	{
		m_Type = arraySize > 1 ? TextureType::Texture1DArray : TextureType::Texture1D;
		m_Format = format;
		m_Usage = usage;
		m_Flag = flag;
	}
	DeviceTexture1D::~DeviceTexture1D()
	{}

	DeviceTexture1D* DeviceTexture1D::CreateArray(
		RenderSystem* pRenderSys,
		int32 X,
		DataFormat format,
		ResourceUsage usage,
		ResourceBindFlag flag,
		int32 arraySize,
		byte const* initialData)
	{
		return pRenderSys->CreateTexture1DArray(X, format, usage, flag, arraySize, initialData);
	}
	DeviceTexture1D* DeviceTexture1D::Create(
		RenderSystem* pRenderSys,
		int32 X,
		DataFormat format,
		ResourceUsage usage,
		ResourceBindFlag flag,
		byte const* initialData)
	{
		return pRenderSys->CreateTexture1D(X, format, usage, flag, initialData);
	}
	
	DeviceTexture2D::DeviceTexture2D(
		int32 X, int32 Y,
		DataFormat format,
		ResourceUsage usage,
		ResourceBindFlag flag,
		int32 arraySize) :
		m_Width(X), m_Height(Y), m_ArraySize(arraySize)
	{
		m_Type = arraySize > 1 ? TextureType::Texture2DArray : TextureType::Texture2D;
		m_Format = format;
		m_Usage = usage;
		m_Flag = flag;
	}
	DeviceTexture2D::~DeviceTexture2D()
	{}

	DeviceTexture2D* DeviceTexture2D::CreateArray(
		RenderSystem* pRenderSys,
		int32 X, int32 Y,
		DataFormat format,
		ResourceUsage usage,
		ResourceBindFlag flag,
		int32 arraySize,
		byte const* initialData)
	{
		return pRenderSys->CreateTexture2DArray(X, Y, format, usage, flag, arraySize, initialData);
	}
	DeviceTexture2D* DeviceTexture2D::Create(
		RenderSystem* pRenderSys,
		int32 X, int32 Y,
		DataFormat format,
		ResourceUsage usage,
		ResourceBindFlag flag,
		byte const* initialData)
	{
		return pRenderSys->CreateTexture2D(X, Y, format, usage, flag, initialData);
	}
	DeviceTexture2D* DeviceTexture2D::CreateFromFile(
		RenderSystem* pRenderSys, std::string const &filename, DataFormat format, ResourceUsage usage, ResourceBindFlag flag)
	{
		return pRenderSys->CreateTexture2DFromFile(filename, format, usage, flag);
	}

	DeviceTexture3D::DeviceTexture3D(
		int32 X, int32 Y, int32 Z,
		DataFormat format,
		ResourceUsage usage,
		ResourceBindFlag flag
		) :
		m_Width(X), m_Height(Y), m_Depth(Z)
	{
		m_Type = TextureType::Texture3D;
		m_Format = format;
		m_Usage = usage;
		m_Flag = flag;
	}
	DeviceTexture3D::~DeviceTexture3D()
	{}

	/*DeviceTexture3D* DeviceTexture3D::CreateArray(RenderSystem* pRenderSys, int32 X, int32 Y, int32 Z, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int32 arraySize, byte const* initialData);*/
	DeviceTexture3D* DeviceTexture3D::Create(
		RenderSystem* pRenderSys,
		int32 X, int32 Y, int32 Z,
		DataFormat format,
		ResourceUsage usage,
		ResourceBindFlag flag,
		byte const* initialData)
	{
		return pRenderSys->CreateTexture3D(X, Y, Z, format, usage, flag, initialData);
	}
}