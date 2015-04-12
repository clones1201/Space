#include "Log.h"
#include "DeviceTexture.hpp"
#include "RenderSystem.hpp"

namespace Space
{
	DeviceTexture1D::DeviceTexture1D(
		int32 X,
		DataFormat format,
		ResourceUsage usage,
		ResourceBindFlag flag,
		int32 arraySize):
		m_Width(X),
		m_Type(arraySize > 1 ? TT_Texture1DArray : TT_Texture1D),
		m_Usage(usage),
		m_Flag(flag)
	{
	}

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

	int32 DeviceTexture1D::GetArraySize() const
	{
		return (m_ArraySize);
	}

	TextureType DeviceTexture1D::GetType() const
	{
		return (m_Type);
	}

	DataFormat DeviceTexture1D::GetFormat() const
	{
		return (m_Format);
	}

	int32 DeviceTexture1D::GetWidth() const
	{
		return (m_Width);
	}

	DeviceTexture2D::DeviceTexture2D(
		int32 X, int32 Y,
		DataFormat format,
		ResourceUsage usage,
		ResourceBindFlag flag,
		int32 arraySize) :
		m_Width(X), m_Height(Y),
		m_Type(arraySize > 1 ? TT_Texture2DArray : TT_Texture2D),
		m_Usage(usage),
		m_Flag(flag)
	{
	}

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
		RenderSystem* pRenderSys, std::string const &filename)
	{
		return pRenderSys->CreateTexture2DFromFile(filename);
	}

	int32 DeviceTexture2D::GetArraySize() const
	{
		return (m_ArraySize);
	}

	TextureType DeviceTexture2D::GetType() const
	{
		return (m_Type);
	}

	DataFormat DeviceTexture2D::GetFormat() const
	{
		return (m_Format);
	}

	int32 DeviceTexture2D::GetWidth() const
	{
		return (m_Width);
	}
	
	int32 DeviceTexture2D::GetHeight() const
	{
		return (m_Height);
	}

	DeviceTexture3D::DeviceTexture3D(
		int32 X, int32 Y, int32 Z,
		DataFormat format,
		ResourceUsage usage,
		ResourceBindFlag flag
		) :
		m_Width(X), m_Height(Y), m_Depth(Z),
		m_Type(TT_Texture3D),
		m_Format(format),
		m_Usage(usage),
		m_Flag(flag)
	{
	}

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
	 
	TextureType DeviceTexture3D::GetType() const
	{
		return (m_Type);
	}

	DataFormat DeviceTexture3D::GetFormat() const
	{
		return (m_Format);
	}

	int32 DeviceTexture3D::GetWidth() const
	{
		return (m_Width);
	}
	
	int32 DeviceTexture3D::GetHeight() const
	{
		return (m_Height);
	}

	int32 DeviceTexture3D::GetDepth() const
	{
		return (m_Depth);
	}

}