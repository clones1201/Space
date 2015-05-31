#include "Log.h"
#include "RenderSystem.hpp"

#include "Rendering.hpp"

namespace Space
{	
	CommandList* CommandList::Create(RenderSystem* pRenderSys)
	{
		return pRenderSys->CreateCommandList();
	}

	CommandList::~CommandList()
	{}
	
	void CommandList::Reset()
	{
		_Reset();
	}

	void CommandList::Close()
	{
		_Close();
	}

	void CommandList::SetPrimitiveTopology()
	{
		_SetPrimitiveTopology();
	}

	void CommandList::SetIndexBuffer(IndexBuffer* buffer)
	{
		_SetIndexBuffer();
	}
	void CommandList::SetVertexBuffers(uint startSlot, uint numBuffers, VertexBuffer** buffer)
	{
		_SetVertexBuffer();
	}
	void CommandList::SetViewPorts(ViewPort** pViewPorts,uint numViewPorts)
	{
		_SetViewPorts();
	}
	void CommandList::SetPipelineState(PipelineState* state)
	{
		_SetPipelineState(state);
	}
	void CommandList::SetRenderTargets(RenderTarget** targets, uint32 numTargets)
	{
		_SetRenderTargets(targets, numTargets);
	}
	void CommandList::DrawIndexed()
	{

	}
	void CommandList::DrawInstanced()
	{

	}
	void CommandList::DrawIndexedInstanced()
	{

	}

	PipelineState* PipelineState::Create(RenderSystem* pRenderSys)
	{
		return pRenderSys->CreatePipelineState();
	}

	PipelineState::PipelineState()
	{}

	PipelineState::~PipelineState()
	{}
	
	void PipelineState::ClearAllState()
	{
		m_BlendStateDesc = BlendDesc();
		m_RasterizerDesc = RasterizerDesc();
		m_DepthStencilDesc = DepthStencilDesc();
		m_SampleDesc = SampleDesc();

		_ClearAllState();
	}

	void PipelineState::SetBlendState(BlendDesc desc)
	{
		m_BlendStateDesc = desc;
		_SetBlendState();
	}
	void PipelineState::SetRasterizerState(RasterizerDesc desc)
	{
		m_RasterizerDesc = desc;
		_SetRasterizerState();
	}
	void PipelineState::SetDepthStencilState(DepthStencilDesc desc)
	{
		m_DepthStencilDesc = desc;
		_SetDepthStencilState();
	}
	void PipelineState::SetSample(SampleDesc desc)
	{
		m_SampleDesc = desc;
		_SetSample();
	}

}