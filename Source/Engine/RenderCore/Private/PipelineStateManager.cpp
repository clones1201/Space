#include "RenderSystem/PipelineStateManager.hpp"

namespace Space
{
namespace Render
{
	PipelineStateManager PipelineStateManager::_instance;

	std::shared_ptr<PipelineState> PipelineStateManager::CreatePipelineState(
		PipelineStateDescriptor const& desc)
	{
		std::shared_ptr<PipelineState> ret = nullptr;
		auto found = m_object_map.find(desc);
		if (found != m_object_map.end())
		{
			ret = found->second.lock();
			if (ret)
				return ret;
		}
		ret = std::make_shared<PipelineState>(Device::GetInstance());
		ret->SetBlendState(desc.BlendState);
		ret->SetRasterizerState(desc.RasterizerState);
		ret->SetDepthStencilState(desc.DepthStencilState);
		
		m_object_map.insert_or_assign(desc, ret);
		return ret;
	}
	
	std::shared_ptr<PipelineState> PipelineStateManager::CreateOpaqueDefault()
	{
		PipelineStateDescriptor desc;
		return CreatePipelineState(desc);
	}

}
}