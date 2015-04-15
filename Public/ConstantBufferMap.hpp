#ifndef __SPACE_CONSTANTBUFFER_MAP_HPP__
#define __SPACE_CONSTANTBUFFER_MAP_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"

#include "ShaderCommon.hpp"

namespace Space
{

	class ConstantBufferMap
	{
	public:
		
		static ConstantBufferMap* Create(RenderSystem* pRenderSys,std::string const& name);

		void AddVariable(std::string const& name, uint32 size, uint32 offset);
		ShaderVariableDesc GetVariableDescByIndex(uint32 index) const;
		ShaderVariableDesc GetVariableDescByName(std::string const& name) const;
		bool SetVariableValue(std::string const& name, byte const* pData);
		uint32 GetVariablesCount() const;

		void Complete();
		void Update();

		std::string GetName() const;
		bool IsComplete() const;
		uint32 GetBufferSize() const;

		~ConstantBufferMap();
	protected:
		ConstantBufferMap(RenderSystem* pRenderSys, std::string const& name);

		std::unique_ptr<ConstantBuffer> m_pBuffer;

		std::unordered_map<Name, ShaderVariableDesc> m_vLayout;
		std::string m_Name;
		uint32 m_Size = 0;
		std::vector<byte> m_pRaw;

		bool m_IsComplete = false;

		RenderSystem* m_pRenderSystem = nullptr;
	};

}

#endif