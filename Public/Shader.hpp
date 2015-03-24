#ifndef __SPACE_SHADER_HPP__
#define __SPACE_SHADER_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"


namespace Space
{
	class VertexShader : virtual public Interface
	{
	public:
		static VertexShader* LoadSourceFromFile();
		static VertexShader* LoadSourceFromMemory();
		static VertexShader* LoadBinaryFromFile();
		static VertexShader* LoadBinaryFromMemory();

		virtual bool Compile() = 0;
				
		void SetShaderResource(std::vector<TypeTrait<ShaderResource>::Ptr>& resources);

		virtual ~VertexShader();
	private:		
		VertexShader();
		
		bool isInMemory;
		bool isBinaryAvailable;
	};

	class GeometryShader : virtual public Interface
	{
	public:

		virtual ~GeometryShader();
	protected:
		GeometryShader();
	};

	class PixelShader : virtual public Interface
	{
	public:

		virtual ~PixelShader();
	protected:
		PixelShader();
	};

}

#endif