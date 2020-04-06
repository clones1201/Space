#pragma once

#include "RenderCommon.hpp"
#include "DeviceResource.hpp"


namespace Space
{
	namespace Render
	{
		struct PipelineStateDescriptor : ComparableBits<PipelineStateDescriptor>
		{
			BlendStateDescriptor BlendState;
			RasterizerStateDescriptor RasterizerState;
			DepthStencilStateDescriptor DepthStencilState;
		};

		namespace Details
		{
			template<class RenderSystem>
			class CommandListImpl : public RenderSystem::CommandList
			{
			public:
				typedef typename std::remove_reference<typename RenderSystem::Device>::type Device;
				typedef typename std::remove_reference<typename RenderSystem::CommandList>::type CommandList;

				CommandListImpl(Device* device):
					CommandList(device)
				{}

				void DrawPrimitives(VertexBuffer const& vertices, IndexBuffer const& indices, uint startIndex, uint numPrimitives)
				{
					CommandList::SetVertexBuffers(0,
					{vertices.GetBuffer()}, {(int32)vertices.GetStride()}, {(int32)vertices.GetOffset()});
					CommandList::SetIndexBuffer(
					indices.GetBuffer(), indices.GetFormat(), indices.GetOffset());
					CommandList::DrawIndexed(startIndex, numPrimitives);
				}
				
				~CommandListImpl(){}
			};

			template<class RenderSystem>
			class PipelineStateImpl : public RenderSystem::PipelineState
			{
			public:
				typedef typename std::remove_reference<typename RenderSystem>::type RenderSystem;
				typedef typename std::remove_reference<typename RenderSystem::Device>::type Device;
				typedef typename std::remove_reference<typename RenderSystem::PipelineState>::type PipelineState;

				PipelineStateImpl(Device* device):
					PipelineState(device)
				{}

				~PipelineStateImpl(){}
				
			private:

			};
		}
	}
}