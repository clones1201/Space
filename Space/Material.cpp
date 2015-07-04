#include "Log.h"
#include "RenderSystem.hpp"
#include "DeviceBuffer.hpp"
#include "Shader.hpp"
#include "ShaderReflection.hpp"
#include "Material.hpp"

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

	Shader* Shader::Create(RenderSystem* pRenderSys, std::wstring const& name)
	{
		return Create(pRenderSys, wstr2str(name));
	}

	Shader* Shader::Create(RenderSystem* pRenderSys, std::string const& name)
	{
		TRY_CATCH_LOG(
			return new Shader(pRenderSys, name),
			return nullptr);
	}

	Shader::Shader(RenderSystem* pRenderSys, std::string const& name)
	{
		std::fstream tempFile(name + ".ps.cso", std::ios_base::in | std::ios_base::binary);
		if (!tempFile.is_open())
		{
			throw std::exception("Missing Shader File");
		}
		std::vector<char> psCode(
			(std::istreambuf_iterator<char>(tempFile)),
			(std::istreambuf_iterator<char>()));

		tempFile.close();
		tempFile.open(name + ".vs.cso", std::ios_base::in | std::ios_base::binary);
		if (!tempFile.is_open())
		{
			throw std::exception("Missing Shader File");
		}
		std::vector<char> vsCode(
			(std::istreambuf_iterator<char>(tempFile)),
			(std::istreambuf_iterator<char>()));

		m_pVS.reset(
			VertexShader::LoadBinaryFromMemory(
			pRenderSys, (byte*)vsCode.data(), vsCode.size()));
		m_pPS.reset(
			PixelShader::LoadBinaryFromMemory(
			pRenderSys, (byte*)psCode.data(), psCode.size()));

		m_pVSReflect.reset(ShaderReflection::CreateD3D(m_pVS.get()));
		m_pPSReflect.reset(ShaderReflection::CreateD3D(m_pPS.get()));
	}

	Shader::~Shader()
	{}

	VertexShader* Shader::GetVertexShader() const
	{
		return m_pVS.get();
	}

	PixelShader* Shader::GetPixelShader() const
	{
		return m_pPS.get();
	}

	// pre-defined constant buffers
	struct CommonVariables
	{
		// modelview * view
		Float4x3 _MV;
		// Projection | CameraPosition, Time
		Float4x4 _PCT;
	};

	Material* Material::Create(RenderSystem* pRenderSys, std::wstring const& name)
	{
		return Create(pRenderSys, wstr2str(name));
	}
	Material* Material::Create(RenderSystem* pRenderSys, std::string const& name)
	{
		TRY_CATCH_LOG(
			return new Material(pRenderSys, name),
			return nullptr);
	}

	using namespace rapidjson;

	Material::Material(RenderSystem* pRenderSys, std::string const& name)
	{

		CommonVariables variables;
		variables._MV._11 = 1.0f;
		variables._MV._22 = 1.0f;
		variables._MV._33 = 1.0f;

		variables._PCT._11 = 1.0f;
		variables._PCT._22 = 1.0f;
		variables._PCT._33 = 1.0f;
		variables._PCT._44 = 0.0f;

		g_CommonVariablesBuffer.reset(ConstantBuffer::Create(pRenderSys,
			(byte const*)&variables, sizeof(CommonVariables)));


		std::string materialPath = GetAssetsPath() + "Material/" + name + "/";
		std::string DescriptionPath = materialPath + "ContentDesc.json";
		std::fstream tempFile(DescriptionPath, std::ios_base::in | std::ios_base::binary);
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
				materialPath + nodeShaderName.GetString());

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

	void Material::SetWorld(Float4x4 world)
	{
		g_World = world;
	}
	void Material::SetView(Float4x4 view)
	{
		g_View = view;
	}
	void Material::SetProjection(Float4x4 projection)
	{
		g_Projection = g_Projection;
	}
	void Material::SetGameTime(float time)
	{
		g_Time = time;
	}

	Shader* Material::GetShader()
	{
		Matrix world = LoadFloat4x4(&g_World);
		Matrix view = LoadFloat4x4(&g_View);
		Matrix projection = LoadFloat4x4(&g_Projection);

		Matrix worldView = world * view;
		projection.r[3].m128_f32[3] = g_Time;

		CommonVariables variables;
		StoreFloat4x3(&variables._MV, worldView);
		StoreFloat4x4(&variables._PCT, projection);
		g_CommonVariablesBuffer->Update(0, sizeof(CommonVariables),
			(byte const*)&variables);
		g_CommonVariablesBuffer->UpdateToDevice();

		SelectShader();

		return m_CurrentShader;
	}

}