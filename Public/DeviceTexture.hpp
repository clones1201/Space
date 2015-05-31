#ifndef __SPACE_DEVICETEXTURE_HPP__
#define __SPACE_DEVICETEXTURE_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"

namespace Space
{
	class DeviceTextureBase
	{
	public:
		virtual ~DeviceTextureBase();
	protected:
		DeviceTextureBase();

		TextureType m_Type = TextureType::None;
		DataFormat m_Format = DataFormat::UNKNOWN;
		ResourceUsage m_Usage = ResourceUsage::Default;
		ResourceBindFlag m_Flag = ResourceBindFlag::None;
	};

	class DeviceTexture1D : virtual public Interface, public DeviceTextureBase
	{
	public:
		virtual ~DeviceTexture1D();

		static DeviceTexture1D* CreateArray(
			RenderSystem* pRenderSys,
			int32 X,
			DataFormat format,
			ResourceUsage usage,
			ResourceBindFlag flag,
			int32 arraySize,
			byte const* initialData);
		static DeviceTexture1D* Create(
			RenderSystem* pRenderSys,
			int32 X,
			DataFormat format,
			ResourceUsage usage,
			ResourceBindFlag flag,
			byte const* initialData);
		
		virtual byte* Lock() = 0;
		virtual void Unlock() = 0;

		int32 GetArraySize() const;
		TextureType GetType() const;
		DataFormat GetFormat() const;
		int32 GetWidth() const;
	protected:
		DeviceTexture1D(int32 X, 
			DataFormat format, 
			ResourceUsage usage, 
			ResourceBindFlag flag,
			int32 arraySize);
		int32 m_Width = 0;
		int32 m_ArraySize = 0;
	};

	class DeviceTexture2D : virtual public Interface, public DeviceTextureBase
	{
	public:
		virtual ~DeviceTexture2D();

		static DeviceTexture2D* CreateArray(
			RenderSystem* pRenderSys,
			int32 X, int32 Y,
			DataFormat format,
			ResourceUsage usage,
			ResourceBindFlag flag,
			int32 arraySize,
			byte const* initialData);
		static DeviceTexture2D* Create(
			RenderSystem* pRenderSys,
			int32 X, int32 Y,
			DataFormat format,
			ResourceUsage usage,
			ResourceBindFlag flag,
			byte const* initialData);
		static DeviceTexture2D* CreateFromFile(
			RenderSystem* pRenderSys, std::string const &filename, 
			DataFormat format, ResourceUsage usage, ResourceBindFlag flag);
		 
		virtual byte* Lock() = 0;
		virtual void Unlock() = 0;

		int32 GetArraySize() const;
		TextureType GetType() const;
		DataFormat GetFormat() const;
		int32 GetWidth() const;
		int32 GetHeight() const;

	protected:
		DeviceTexture2D(int32 X, int32 Y,
			DataFormat format,
			ResourceUsage usage,
			ResourceBindFlag flag,
			int32 arraySize);
		int32 m_Width = 0, m_Height = 0; 
		int32 m_ArraySize = 0;
	};

	class DeviceTexture3D : virtual public Interface, public DeviceTextureBase
	{
	public:
		virtual ~DeviceTexture3D();

		static DeviceTexture3D* Create(
			RenderSystem* pRenderSys,
			int32 X, int32 Y, int32 Z,
			DataFormat format,
			ResourceUsage usage,
			ResourceBindFlag flag,
			byte const* initialData);
		
		virtual byte* Lock() = 0;
		virtual void Unlock() = 0;

		int32 GetArraySize() const;
		TextureType GetType() const;
		DataFormat GetFormat() const;
		int32 GetWidth() const;
		int32 GetHeight() const;
		int32 GetDepth() const;

	protected:
		DeviceTexture3D(int32 X, int32 Y, int32 Z,
			DataFormat format,
			ResourceUsage usage,
			ResourceBindFlag flag);
		int32 m_Width = 0, m_Height = 0, m_Depth = 0; 
	};

}

#endif