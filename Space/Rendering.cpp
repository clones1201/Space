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

}