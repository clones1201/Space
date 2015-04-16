#ifndef __SPACE_RENDERSYSTEM_HPP__
#define __SPACE_RENDERSYSTEM_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"

namespace Space
{
	class RenderSystem : virtual public Interface
	{
	public:
		virtual ~RenderSystem();
	protected:
		RenderSystem();		 

	public:
		virtual RenderWindow* CreateRenderWindow(std::string const& name, int32 width, int32 height, bool isFullScreen) = 0;
		virtual DeviceBuffer* CreateBuffer(BufferType type, ResourceUsage usage, byte const* initialData, uint32 lengthInBytes) = 0;

		virtual DeviceTexture1D* CreateTexture1D(int32 X, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData) = 0;
		virtual DeviceTexture1D* CreateTexture1DArray(int32 X, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int32 arraySize, byte const* initialData) = 0;
		virtual DeviceTexture2D* CreateTexture2D(int32 X, int32 Y, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData) = 0;
		virtual DeviceTexture2D* CreateTexture2DArray(int32 X, int32 Y, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int32 arraySize, byte const* initialData) = 0;
		virtual DeviceTexture2D* CreateTexture2DFromFile(std::string const& filename, DataFormat format, ResourceUsage usage, ResourceBindFlag flag) = 0;
		virtual DeviceTexture3D* CreateTexture3D(int32 X, int32 Y, int32 Z, DataFormat format, ResourceUsage usage, ResourceBindFlag flag,byte const* initialData) = 0;
		/*virtual DeviceTexture3D* CreateTexture3DArray(int32 X, int32 Y, int32 Z, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int32 arraySize, byte const* initialData) = 0;*/

		virtual VertexShader* LoadVertexShaderFromMemory(byte const* byteCodes, uint32 sizeInBytes) = 0;
		virtual PixelShader* LoadPixelShaderFromMemory(byte const* byteCodes, uint32 sizeInBytes) = 0;
		
		virtual DepthStencilView* CreateDepthStencilView(DeviceTexture2D* pTexture) = 0;
		virtual RenderTarget* CreateRenderTarget(DeviceTexture2D* pTexture) = 0;
	};
}

#endif