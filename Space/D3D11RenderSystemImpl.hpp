#ifndef __SPACE_D3D11RENDERSYSTEM_IMPL_HPP__
#define __SPACE_D3D11RENDERSYSTEM_IMPL_HPP__

#include "D3D11Prerequisites.hpp"
#include "D3D11RenderSystem.hpp"

namespace Space
{
	
	class D3D11RenderSystemImpl : public D3D11RenderSystem
	{
	private:
		D3D11Device mDevice;

		D3D11RenderSystemImpl();
		static D3D11RenderSystemImpl* Create();
		friend class D3D11RenderSystem;
	public:
		~D3D11RenderSystemImpl();

		D3D11Device* GetDevice() const throw();

		virtual RenderWindow* CreateRenderWindow(const std::string& title, int width, int height, bool fullscreen) throw();
		virtual DeviceBuffer* CreateBuffer(BufferType type, ResourceUsage usage, byte const* initialData, size_t lengthInBytes) throw();
		virtual DeviceTexture1D* CreateTexture1D(int X, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData) throw();
		virtual DeviceTexture1D* CreateTexture1DArray(int X, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int arraySize, byte const* initialData) throw();
		virtual DeviceTexture2D* CreateTexture2D(int X, int Y, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData) throw();
		virtual DeviceTexture2D* CreateTexture2DArray(int X, int Y, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int arraySize, byte const* initialData) throw();
		virtual DeviceTexture2D* CreateTexture2DFromFile(std::string const& filename) throw();
		virtual DeviceTexture3D* CreateTexture3D(int X, int Y, int Z, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData) throw();

	};
}

#endif