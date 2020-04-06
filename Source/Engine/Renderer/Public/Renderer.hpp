#pragma once

#include "Prerequisites.hpp"

namespace Space
{
namespace Render {

	class RenderJob
	{
	public:
		std::shared_ptr<PipelineState> RenderStates;

		/* Shaders
		 */
		std::shared_ptr<VertexShader> VS;
//		std::shared_ptr<GeometryShader> GS;
//		std::shared_ptr<HullShader> HS;
//		std::shared_ptr<DomainShader> DS;
		std::shared_ptr<PixelShader> PS;


	};

	class Renderer
	{
	public:
		void Render();
		void Update();
			
		Renderer();
	private:
	};
}
}
