#ifndef __SPACE_SHADER_HPP__
#define __SPACE_SHADER_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"
#include "ShaderCommon.hpp"
#include "ShaderReflection.hpp"
#include "ShaderResourcePool.hpp"

namespace Space
{
	class VertexShader : virtual public Interface
	{  
	public:
		static VertexShader* LoadBinaryFromMemory(RenderSystem* pRenderSys, byte const* byteCodes, uint32 sizeInBytes);
		byte const* GetByteCodes() const;
		uint32 GetSizeInBytes() const;
		
		virtual void Apply() = 0;

		virtual ~VertexShader();
	protected:		
		VertexShader(); 

		byte const* m_ByteCodes = nullptr;
		uint32 m_SizeInBytes = 0;
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
		byte const* GetByteCodes() const;
		uint32 GetSizeInBytes() const;
		
		virtual void Apply() = 0;

		virtual ~PixelShader();
	protected:
		PixelShader();

		byte const* m_ByteCodes = nullptr;
		uint32 m_SizeInBytes = 0;
	};

}

#endif