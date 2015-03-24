#ifndef __SPACE_RENDERSYSTEM_HPP__
#define __SPACE_RENDERSYSTEM_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp" 

namespace Space
{
	class RenderSystem : virtual public Interface
	{
	public:
		void Render(RenderTarget* target, SceneManager* scene);

		virtual ~RenderSystem();
	protected:
		void Prerender();
		void Postrender();
		void FireOneFrame();

		virtual bool _Prerender() = 0;
		virtual void _Postrender() = 0;
		virtual void _FireOneFrame() = 0;

		RenderTarget* mActiveRenderTarget = nullptr;

	public:
		virtual RenderWindow* CreateRenderWindow(std::string const& name, int width, int height, bool isFullScreen) = 0;
		virtual DeviceBuffer* CreateBuffer(BufferType type, ResourceUsage usage, byte const* initialData, size_t lengthInBytes) = 0;

		virtual DeviceTexture1D* CreateTexture1D(int X, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData) = 0;
		virtual DeviceTexture1D* CreateTexture1DArray(int X, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int arraySize, byte const* initialData) = 0;
		virtual DeviceTexture2D* CreateTexture2D(int X, int Y, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData) = 0;
		virtual DeviceTexture2D* CreateTexture2DArray(int X, int Y, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int arraySize, byte const* initialData) = 0;
		virtual DeviceTexture2D* CreateTexture2DFromFile(std::string const& filename);
		virtual DeviceTexture3D* CreateTexture3D(int X, int Y, int Z, DataFormat format, ResourceUsage usage, ResourceBindFlag flag,byte const* initialData) = 0;
		/*virtual DeviceTexture3D* CreateTexture3DArray(int X, int Y, int Z, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int arraySize, byte const* initialData) = 0;*/
		
		RenderSystem();
	};
}

#endif