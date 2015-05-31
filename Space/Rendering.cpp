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

	void PipelineState::SetSampleMask(uint32 mask)
	{
		m_SampleMask = mask;
	}

	void PipelineState::SetStencilRef(uint32 ref)
	{
		m_StencilRef = ref;
	}

	void PipelineState::SetBlendFactor(Float4 factor)
	{
		m_BlendFactor = factor;
	}

}