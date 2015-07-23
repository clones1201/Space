#ifndef __SPACE_SHADER_HPP__
#define __SPACE_SHADER_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"
#include "ShaderCommon.hpp"
#include "ShaderReflection.hpp"
#include "ShaderResourcePool.hpp"

namespace Space
{
	class ShaderBase
	{
	public:
		virtual ~ShaderBase();

		void ClearAllSlots();

		void SetConstantBuffer(uint slots, ConstantBuffer* buffer);
		void SetShaderResource(uint slots, ShaderResource* resource);

		// get the max number of the slot
		inline int32 GetConstantBufferCount() const
		{
			assert(m_MaxCBSlot >= 0);
			return m_MaxCBSlot + 1;
		}
		// get the max number of the slot
		inline int32 GetShaderResourceCount() const
		{
			assert(m_MaxSRSlot >= 0);
			return m_MaxSRSlot + 1;
		}

		ConstantBuffer* GetConstantBuffer(uint slot) const;
		ShaderResource* GetShaderResource(uint slot) const;

		inline byte const* GetByteCodes() const
		{
			return m_ByteCodes.data();
		}
		inline uint32 GetSizeInBytes() const
		{
			return m_ByteCodes.size();
		}
	protected:
		ShaderBase();

		std::vector<byte> m_ByteCodes;

		typedef std::map<uint, ConstantBuffer*> ConstantBufferTable;
		typedef std::map<uint, ShaderResource*> ShaderResourceTable;
		ConstantBufferTable m_ConstantBufferTable;
		ShaderResourceTable m_ShaderResourceTable;

		int32 m_MaxCBSlot = 0;
		int32 m_MaxSRSlot = 0;
	};

	class VertexShader : virtual public Interface, public ShaderBase
	{  
	public:
		static VertexShader* LoadBinaryFromMemory(RenderSystem* pRenderSys, byte const* byteCodes, uint32 sizeInBytes);
				 
		virtual ~VertexShader();
	protected:		
		VertexShader();  
	};
	typedef std::shared_ptr<VertexShader> VertexShaderPtr;
	/*class GeometryShader : virtual public Interface
	{
	public:

		virtual ~GeometryShader();
	protected:
		GeometryShader();
	};*/

	class PixelShader : virtual public Interface, public ShaderBase
	{
	public:
		static PixelShader* LoadBinaryFromMemory(RenderSystem* pRenderSys, byte const* byteCodes, uint32 sizeInBytes);
				 
		virtual ~PixelShader();
	protected:
		PixelShader(); 
	};
	typedef std::shared_ptr<PixelShader> PixelShaderPtr;
}

#endif