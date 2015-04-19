#ifndef __SPACE_SHADERCOMMON_HPP__
#define __SPACE_SHADERCOMMON_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"

namespace Space
{
	typedef enum _ShaderStage
	{
		SS_VertexShader = 0,
		SS_GeometryShader = 1,
		SS_PixelShader = 2,
		SS_Num = 3
	}ShaderStage; 
}

#endif