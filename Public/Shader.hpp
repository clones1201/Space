#ifndef __SPACE_SHADER_HPP__
#define __SPACE_SHADER_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"

#include "ShaderCommon.hpp"
#include "ShaderReflection.hpp"

namespace Space
{
	class VertexShader : virtual public Interface
	{  
	public:
		static VertexShader* LoadBinaryFromMemory(RenderSystem* pRenderSys, byte const* byteCodes, uint32 sizeInBytes);
		
		ShaderResourcePool* ResourcePool = nullptr;

		std::vector<uint> ShaderResourceTable;
		std::vector<uint> SamplerTable; 

		virtual void Apply() = 0;

		virtual ~VertexShader();
	protected:		
		VertexShader(); 

		bool m_IsBinaryAvailable; 
	};

	/*class GeometryShader : virtual public Interface
	{
	public:

		virtual ~GeometryShader();
	protected:
		GeometryShader();
	};*/

	class PixelShader : virtual public Interface
	{
	public:
		static PixelShader* LoadBinaryFromMemory(RenderSystem* pRenderSys, byte const* byteCodes, uint32 sizeInBytes);

		ShaderResourcePool* ResourcePool = nullptr;

		std::vector<uint> ShaderResourceTable;
		std::vector<uint> SamplerTable;

		virtual void Apply() = 0;

		virtual ~PixelShader();
	protected:
		PixelShader();
	};


	class Shader : virtual public Interface
	{
	protected:
		std::unique_ptr<VertexShader> m_pVS = nullptr;
		std::unique_ptr<PixelShader> m_pPS = nullptr;
		

	public: 



	};
}

#endif