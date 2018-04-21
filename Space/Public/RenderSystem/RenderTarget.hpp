#pragma once

#include "RenderSystem/Prerequisites.hpp"

namespace Space
{
	namespace Render
	{
		namespace Details
		{
			template<class RenderSystem>
			class RenderTargetImpl : public RenderSystem::RenderTarget
			{
			public:
				typedef typename std::remove_reference<typename RenderSystem>::type RenderSystem;
				typedef typename std::remove_reference<typename RenderSystem::Device>::type Device;
				typedef typename std::remove_reference<typename RenderSystem::RenderTarget>::type RenderTarget;
				typedef typename std::remove_reference<typename RenderSystem::DeviceTexture2D>::type DeviceTexture2D;

				RenderTargetImpl(Device* device, DeviceTexture2D const* pBackBuffer) :
					RenderTarget(device, pBackBuffer)
				{}

				~RenderTargetImpl() {}
			};

			template<class RenderSystem>
			class RenderWindowImpl : public RenderSystem::RenderWindow
			{
			public:
				typedef typename std::remove_reference<typename RenderSystem>::type RenderSystem;
				typedef typename std::remove_reference<typename RenderSystem::Device>::type Device;
				typedef typename std::remove_reference<typename RenderSystem::RenderWindow>::type RenderWindow;
				typedef typename std::remove_reference<typename RenderSystem::DeviceTexture2D>::type DeviceTexture2D;
				typedef Texture2D BackBufferType;

				DeviceTexture2D* GetBackBuffer() const
				{
					return RenderWindow::GetBackBuffer();
				}
				inline void Present()
				{
					RenderWindow::Present();
				}
				void Resize(int32 width, int32 height)
				{
					RenderWindow::Resize(width, height);
				}
				void Show()
				{
					RenderWindow::Show();
				}
				RenderWindowImpl(Device* device,
					const std::string& name, int32 width, int32 height, bool fullscreen) :
					RenderWindow(device, name, width, height, fullscreen)
				{}

				~RenderWindowImpl() {}
			};
		
			template<class RenderSystem>
			class DepthStencilViewImpl : public RenderSystem::DepthStencilView
			{
			public:
				typedef typename std::remove_reference<typename RenderSystem::DepthStencilView>::type DepthStencilView;
				DepthStencilViewImpl(Device* device, typename RenderSystem::DeviceTexture2D const* buffer) :
					DepthStencilView(device, buffer)
				{}
			};
		}
	}
}