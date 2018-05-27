#ifndef __RENDERSYSTEM_PIPELINESTATE_MANAGER_HPP__
#define __RENDERSYSTEM_PIPELINESTATE_MANAGER_HPP__

#pragma once
#include "Common/Basic.hpp"
#include "Prerequisites.hpp"
#include "Rendering.hpp"

namespace Space
{
namespace Render
{
	class PipelineStateManager : public Uncopyable
	{
	public:
		static PipelineStateManager& Instance()
		{
			return _instance;
		}

		std::shared_ptr<PipelineState> CreatePipelineState(PipelineStateDescriptor const& desc);		
		std::shared_ptr<PipelineState> CreateOpaqueDefault();

	private:
		static PipelineStateManager _instance;

		std::map<PipelineStateDescriptor, std::weak_ptr<PipelineState>> m_object_map;
	};
	
}
}

#endif