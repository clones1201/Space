#include "Common/Log.h"
#include "RenderSystem/RenderSystem.hpp"

#include "Space/Material.hpp"

namespace Space
{

	StaticBoolParameter::~StaticBoolParameter()
	{}

	StaticBoolParameter::StaticBoolParameter(
		StaticParameterSet* pSet, std::string const& name, uint8 bit)
		:StaticBoolParameter(pSet, Name(name), bit)
	{
	}

	StaticBoolParameter::StaticBoolParameter(
		StaticParameterSet* pSet, std::wstring const& name, uint8 bit)
		: StaticBoolParameter(pSet, Name(name), bit)
	{
	}

	StaticBoolParameter::StaticBoolParameter(
		StaticParameterSet* pSet, Name const& name, uint8 bit)
		: pSet(pSet), m_Name(name), m_Bit(bit)
	{
	}
	
	bool StaticBoolParameter::GetValue() const
	{
		return pSet->_GetBoolValueFromBits(m_Bit);
	}
	void StaticBoolParameter::SetValue(bool value)
	{
		pSet->_SetBoolValueToBits(m_Bit, value);
	}


	//Definitions of class StaticMaskParameter

	StaticMaskParameter::StaticMaskParameter(
		StaticParameterSet* pSet, std::string const& name,
		uint8 bitR, uint8 bitG, uint8 bitB, uint8 bitA)
		:StaticMaskParameter(pSet, Name(name), bitR, bitG, bitB, bitA)
	{
	}

	StaticMaskParameter::StaticMaskParameter(
		StaticParameterSet* pSet, std::wstring const& name,
		uint8 bitR, uint8 bitG, uint8 bitB, uint8 bitA)
		: StaticMaskParameter(pSet, Name(name), bitR, bitG, bitB, bitA)
	{
	}

	StaticMaskParameter::StaticMaskParameter(
		StaticParameterSet* pSet, Name const& name,
		uint8 bitR, uint8 bitG, uint8 bitB, uint8 bitA)
		: pSet(pSet), m_Name(name), m_BitR(bitR), m_BitG(bitG), m_BitB(bitB), m_BitA(bitA)
	{
	}

	StaticMaskParameter::~StaticMaskParameter()
	{
	}

	bool StaticMaskParameter::GetMaskR() const
	{
		return pSet->_GetBoolValueFromBits(m_BitR);
	}
	bool StaticMaskParameter::GetMaskG() const
	{
		return pSet->_GetBoolValueFromBits(m_BitG);
	}
	bool StaticMaskParameter::GetMaskB() const
	{
		return pSet->_GetBoolValueFromBits(m_BitB);
	}
	bool StaticMaskParameter::GetMaskA() const
	{
		return pSet->_GetBoolValueFromBits(m_BitA);
	}

	inline void StaticMaskParameter::SetMaskR(bool value)
	{
		pSet->_SetBoolValueToBits(m_BitR, value);
	}
	inline void StaticMaskParameter::SetMaskG(bool value)
	{
		pSet->_SetBoolValueToBits(m_BitG, value);
	}
	inline void StaticMaskParameter::SetMaskB(bool value)
	{
		pSet->_SetBoolValueToBits(m_BitB, value);
	}
	inline void StaticMaskParameter::SetMaskA(bool value)
	{
		pSet->_SetBoolValueToBits(m_BitA, value);
	}

	// Definitions of class StaticParameterSet
	StaticParameterSet::StaticParameterSet(){}

	std::vector<StaticParameterSet> StaticParameterSet::GetAllStaticParameterSet()
	{
		std::vector<StaticParameterSet> ret;
		int count = (2 << this->GetSwitchCount()) * (4 << this->GetMaskCount());
		ret.reserve(count);
		for (auto i = 0; i < count; ++i)
		{
			StaticParameterSet newOne = *this;
			newOne.m_Bits = i;
			ret.push_back(newOne);
		}
		return std::move(ret);
	}

	void StaticParameterSet::Clear()
	{
		m_SwitchParameters.clear();
		m_MaskParameters.clear();
		m_Bits = 0;
		m_BitCount = 0;
	}

	void StaticParameterSet::AddSwitch(Name const& name,
		bool defaultValue)
	{
		//assert(m_BitCount + 1 < 32);
		if (m_BitCount + 1 >= 32) throw std::exception("Static Parameter Set is Full.");
		StaticBoolParameter newParam = StaticBoolParameter(
			this, name, m_BitCount);
		m_BitCount++;
		newParam.SetValue(defaultValue);
		m_SwitchParameters.insert(
			std::pair<Name, StaticBoolParameter>(name, newParam));
	}
	void StaticParameterSet::AddMask(Name const& name,
		bool maskR, bool maskG,
		bool maskB, bool maskA)
	{
		//assert(m_BitCount + 4 < 32);
		if (m_BitCount + 4 >= 32) throw std::exception("Static Parameter Set is Full.");
		StaticMaskParameter newParam = StaticMaskParameter(
			this, name, m_BitCount, m_BitCount + 1, m_BitCount + 2, m_BitCount + 3);
		m_BitCount += 4;
		newParam.SetMaskR(maskR);
		newParam.SetMaskG(maskG);
		newParam.SetMaskB(maskB);
		newParam.SetMaskA(maskA);
		m_MaskParameters.insert(
			std::pair<Name, StaticMaskParameter>(name, newParam));
	}

	// Definitions of class Material
	static char* g_DomainStringArrayA[] = {
		"Surface",
		"PostProcess"
	};

	static wchar_t* g_DomainStringArrayW[] = {
		L"Surface",
		L"PostProcess"
	};
	static char* g_BlendModeStringArrayA[] = {
		"Opaque",
		"Masked",
		"Translaucent",
		"Additive"
	};
	static wchar_t* g_BlendModeStringArrayW[] = {
		L"Opaque",
		L"Masked",
		L"Translaucent",
		L"Additive"
	};

	inline char* GetMaterialDomainStringA(MaterialDomain domain)
	{
		return g_DomainStringArrayA[(int32)domain];
	}

	inline wchar_t* GetMaterialDomainStringW(MaterialDomain domain)
	{
		return g_DomainStringArrayW[(int32)domain];
	}

	inline MaterialDomain GetMaterialDomainByString(std::string const& name)
	{
		for (int32 i = 0; i < (int32)MaterialDomain::Max; ++i)
		{
			if (g_DomainStringArrayA[i] == name)
			{
				return (MaterialDomain)i;
			}
		}
		return MaterialDomain::Surface;
	}
	inline MaterialDomain GetMaterialDomainByString(std::wstring const& name)
	{
		for (int32 i = 0; i < (int32)MaterialDomain::Max; ++i)
		{
			if (g_DomainStringArrayW[i] == name)
			{
				return (MaterialDomain)i;
			}
		}
		return MaterialDomain::Surface;
	}

	inline MaterialBlendMode GetMaterialBlendModeByString(std::string const& name)
	{
		for (int32 i = 0; i < (int32)MaterialBlendMode::Max; ++i)
		{
			if (g_BlendModeStringArrayA[i] == name)
			{
				return (MaterialBlendMode)i;
			}
		}
		return MaterialBlendMode::Opaque;
	}
	inline MaterialBlendMode GetMaterialBlendModeByString(std::wstring const& name)
	{
		for (int32 i = 0; i < (int32)MaterialBlendMode::Max; ++i)
		{
			if (g_BlendModeStringArrayW[i] == name)
			{
				return (MaterialBlendMode)i;
			}
		}
		return MaterialBlendMode::Opaque;
	}

	

	// pre-defined constant buffers
	struct CommonVariables
	{
		// modelview * view
		Float4x4 _MV;
		// Projection | CameraPosition, Time
		Float4x4 _PCT;
	};

	StaticParameterSet const & Material::GetStaticParameterSet() const
	{ 
		return m_ParameterSet;
	}
	StaticParameterSet & Material::GetStaticParameterSet()
	{ 
		return m_ParameterSet;
	}
	Material* Material::Create(RenderSystem* pRenderSys, tstring const& name)
	{
		TRY_CATCH_LOG(
			return new Material(pRenderSys, name),
			return nullptr);
	}

	using namespace rapidjson;

	Material::Material(RenderSystem* pRenderSys, tstring const& name)
	{

		CommonVariables variables;
		variables._MV._11 = 1.0f;
		variables._MV._22 = 1.0f;
		variables._MV._33 = 1.0f;

		variables._PCT._11 = 1.0f;
		variables._PCT._22 = 1.0f;
		variables._PCT._33 = 1.0f;
		variables._PCT._44 = 0.0f;

		m_CommonVariablesBuffer.reset(ConstantBuffer::Create(pRenderSys,
			(byte const*)&variables, sizeof(CommonVariables)));
		
		tstring materialPath = Core::GetInstance()->GetAssetsDir() + TEXT("/Material/") + name + TEXT("/");
		tstring DescriptionPath = materialPath + TEXT("ContentDesc.json");
		std::fstream tempFile(wstr2str(DescriptionPath), std::ios_base::in | std::ios_base::binary);
		if (!tempFile.is_open())
		{
			throw std::exception("Missing Shader File");
		}
		std::string json(
			(std::istreambuf_iterator<char>(tempFile)),
			(std::istreambuf_iterator<char>()));

		Document dc;
		dc.Parse(json.c_str());

		Value& nodeMaterial = dc["Material"];
		if (nodeMaterial.IsNull())
		{
			throw std::exception("No Material");
		}

		// Get Name of Material
		Value& nodeName = nodeMaterial["Name"];
		if (nodeName.IsNull())
		{
			throw std::exception("No Material Name");
		}
		m_Name = Name(nodeName.GetString());

		// Get Blend Mode of Material
		Value& nodeBlendMode = nodeMaterial["BlendMode"];
		if (nodeBlendMode.IsNull())
		{
			Log("default blend mode:%s\n", nodeBlendMode.GetString());
		}
		else
		{
			m_BlendMode = GetMaterialBlendModeByString(nodeBlendMode.GetString());
		}

		// Get Domain of Material
		Value& nodeDomain = nodeMaterial["Domain"];
		if (nodeDomain.IsNull())
		{
			Log("default blend mode:%s\n", nodeBlendMode.GetString());
		}
		else
		{
			m_Domain = GetMaterialDomainByString(nodeDomain.GetString());
		}

		// Get Static Parameter Set
		Value& nodeStaticParameters = nodeMaterial["StaticParameters"];
		if (nodeStaticParameters.IsNull() || !nodeStaticParameters.IsArray())
		{
			throw std::exception("Static Parameters are missing");
		}
		m_ParameterSet.Clear();
		m_DefaultParameterSet.Clear();
		for (auto iter = nodeStaticParameters.Begin();
			iter != nodeStaticParameters.End(); ++iter)
		{
			Value& nodeParam = *iter;
			Value& nodeName = nodeParam["Name"];
			Value& nodeType = nodeParam["Type"];

			Name name = Name(nodeName.GetString());
			if (nodeType.GetString() == std::string("Switch"))
			{
				m_DefaultParameterSet.AddSwitch(name);

				Value& nodeDefaultValue = nodeParam["DefaultValue"];
				m_DefaultParameterSet.GetSwitchByName(name)
					.SetValue(
					!nodeDefaultValue.IsNull() && nodeDefaultValue.IsBool() ?
					nodeDefaultValue.GetBool() :
					false);
			}
			else if (nodeType.GetString() == std::string("Mask"))
			{
				m_DefaultParameterSet.AddMask(name);
				Value& nodeValue = nodeParam["DefaultValue"];
				if (!nodeValue.IsArray())
				{
					continue;
				}

				auto iterR = nodeValue.Begin();
				auto iterG = iterR + 1;
				auto iterB = iterG + 1;
				auto iterA = iterB + 1;

				m_DefaultParameterSet.GetMaskByName(name)
					.SetMaskR(
					iterR->IsBool() ?
					iterR->GetBool() :
					false);
				m_DefaultParameterSet.GetMaskByName(name)
					.SetMaskG(
					iterG->IsBool() ?
					iterG->GetBool() :
					false);
				m_DefaultParameterSet.GetMaskByName(name)
					.SetMaskB(
					iterB->IsBool() ?
					iterB->GetBool() :
					false);
				m_DefaultParameterSet.GetMaskByName(name)
					.SetMaskA(
					iterA->IsBool() ?
					iterA->GetBool() :
					false);
			}
		}
		m_ParameterSet = m_DefaultParameterSet;
		// Get All Shaders in Material
		Value& nodeShaders = nodeMaterial["Shaders"];
		if (nodeShaders.IsNull() || !nodeShaders.IsArray())
		{
			throw std::exception("Material doesn't have any shaders in it.");
		}
		for (auto iter = nodeShaders.Begin();
			iter != nodeShaders.End(); ++iter)
		{
			Value& nodeShader = *iter;
			Value& nodeShaderName = nodeShader["Name"];

			Shader* shader = Shader::Create(pRenderSys,
				materialPath + toTs(nodeShaderName.GetString()));

			shader->GetVertexShader()->SetConstantBuffer(0, m_CommonVariablesBuffer.get());
			shader->GetPixelShader()->SetConstantBuffer(0, m_CommonVariablesBuffer.get());

			m_Shaders.insert(std::pair<Name, std::unique_ptr<Shader>>(
				Name(nodeShaderName.GetString()),
				std::unique_ptr<Shader>(shader)));

			Value& nodeParamSet = nodeShader["ParameterSet"];
			if (nodeParamSet.IsArray())
			{
				for (auto iter = nodeParamSet.Begin();
					iter != nodeParamSet.End(); ++iter)
				{
					StaticParameterSet paramSet = m_DefaultParameterSet;
					paramSet._SetBits(iter->GetUint());
					m_ShaderMap.insert(
						std::pair<StaticParameterSet, Shader*>(
						paramSet, shader));
				}
			}
		}
		SelectShader();
	}

	Material::~Material()
	{}

	void Material::SelectShader()
	{
		m_CurrentShader = m_ShaderMap.at(m_ParameterSet);
	}

	Name Material::GetName() const
	{
		return m_Name;
	}

	MaterialDomain Material::GetDomain() const
	{
		return m_Domain;
	}

	MaterialBlendMode Material::GetBlendMode() const
	{
		return m_BlendMode;
	}

	Shader* Material::GetShader()
	{
		Matrix world = LoadFloat4x4(&m_World);
		Matrix view = LoadFloat4x4(&m_View);
		Matrix projection = MatrixTranspose(LoadFloat4x4(&m_Projection));

		Matrix worldView = MatrixTranspose(world * view);
		//projection.r[3].m128_f32[3] = m_Time;

		CommonVariables variables;
		StoreFloat4x4(&variables._MV, worldView);
		StoreFloat4x4(&variables._PCT, projection);
		m_CommonVariablesBuffer->Update(0, sizeof(CommonVariables),
			(byte const*)&variables);
		m_CommonVariablesBuffer->UpdateToDevice();

		SelectShader();

		return m_CurrentShader;
	}

}