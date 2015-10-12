#ifndef __SPACE_RENDERSYSTEM_SHADERCOMMON_HPP__
#define __SPACE_RENDERSYSTEM_SHADERCOMMON_HPP__

#include "RenderSystem/Prerequisites.hpp"

namespace Space
{
	enum class SPACE_RENDERSYSTEM_API ShaderStage : uint8
	{
		SS_VertexShader = 0,
		SS_GeometryShader = 1,
		SS_PixelShader = 2,
		SS_Num = 3
	}; 
}

#endif