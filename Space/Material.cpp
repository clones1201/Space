#include "Log.h"
#include "Shader.hpp"
#include "ShaderReflection.hpp"
#include "Material.hpp"

namespace Space
{
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
		for (int32 i = 0; i < MD_Max; ++i)
		{
			if (g_DomainStringArrayA[i] == name)
			{
				return (MaterialDomain)i;
			}
		}
	}
	inline MaterialDomain GetMaterialDomainByString(std::wstring const& name)
	{
		for (int32 i = 0; i < MD_Max; ++i)
		{
			if (g_DomainStringArrayW[i] == name)
			{
				return (MaterialDomain)i;
			}
		}
	}

	inline MaterialBlendMode GetMaterialBlendModeByString(std::string const& name)
	{
		for (int32 i = 0; i < MD_Max; ++i)
		{
			if (g_BlendModeStringArrayA[i] == name)
			{
				return (MaterialBlendMode)i;
			}
		}
	}
	inline MaterialBlendMode GetMaterialBlendModeByString(std::wstring const& name)
	{
		for (int32 i = 0; i < MD_Max; ++i)
		{
			if (g_BlendModeStringArrayW[i] == name)
			{
				return (MaterialBlendMode)i;
			}
		}
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
		std::fstream tempFile(GetAssetsPath() + "Material/" + name + "/ContentDesc.json", std::ios_base::in | std::ios_base::binary);
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
			m_Domain = GetMaterialDomainByString(nodeBlendMode.GetString());
		}

		// Get All Shaders in Material
		Value& nodeShaders = nodeMaterial["Shaders"];
		if (nodeShaders.IsNull() || !nodeShaders.IsArray())
		{
			throw std::exception("Material doesn't have any shaders in it.");
		}
		for (auto iter = nodeShaders.Begin(); iter != nodeShaders.End(); ++iter)
		{
			Value& nodeShader = *iter;
			Value& nodeShaderName = nodeShader["Name"];
			
			Value& nodeSourceFileName = nodeShader["SourceFile"];
			Shader* shader = Shader::Create(pRenderSys,
				nodeSourceFileName.GetString());

			m_Shaders.insert( std::pair<Name,std::shared_ptr<Shader>>(
					Name(nodeShaderName.GetString()),
					std::shared_ptr<Shader>(shader)));
		}

		Value& nodeStaticParameters = nodeMaterial["StaticParameters"];
		if (nodeStaticParameters.IsNull() || !nodeStaticParameters.IsArray())
		{
			throw std::exception("Static Parameters are missing");
		}
		m_ParameterSet.Clear();
		m_DefaultParameterSet.Clear();
		for (auto iter = nodeStaticParameters.Begin(); iter != nodeStaticParameters.End(); ++iter)
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
					nodeDefaultValue.GetBool():
					false);
			}
			else if (nodeType.GetString() == std::string("Mask"))
			{
				m_DefaultParameterSet.AddMask(name);
				Value& nodeValueR = nodeParam["DefaultValueR"];
				Value& nodeValueG = nodeParam["DefaultValueG"];
				Value& nodeValueB = nodeParam["DefaultValueB"];
				Value& nodeValueA = nodeParam["DefaultValueA"];

				m_DefaultParameterSet.GetMaskByName(name)
					.SetMaskR(
					!nodeValueR.IsNull() && nodeValueR.IsBool() ?
					nodeValueR.GetBool() :
					false);
				m_DefaultParameterSet.GetMaskByName(name)
					.SetMaskG(
					!nodeValueG.IsNull() && nodeValueG.IsBool() ?
					nodeValueG.GetBool() :
					false);
				m_DefaultParameterSet.GetMaskByName(name)
					.SetMaskB(
					!nodeValueB.IsNull() && nodeValueB.IsBool() ?
					nodeValueB.GetBool() :
					false);
				m_DefaultParameterSet.GetMaskByName(name)
					.SetMaskA(
					!nodeValueA.IsNull() && nodeValueA.IsBool() ?
					nodeValueA.GetBool() :
					false);
			}
		}
		m_ParameterSet = m_DefaultParameterSet;

		// Get Shader Map (Static Parameters Set => Shader)
		Value& nodeShaderMap = nodeMaterial["ShaderMap"];
		if (nodeShaderMap.IsNull() || !nodeShaderMap.IsArray())
		{
			throw std::exception("Material doesn't have a Shader Map in it.");
		}
		for (auto iter = nodeShaderMap.Begin(); iter != nodeShaders.End(); ++iter)
		{
			Value& nodeShaderMapItem = *iter;
			Value& nodeParameterSet = nodeShaderMapItem["ParameterSet"];
			if (nodeParameterSet.IsNull() || !nodeParameterSet.IsArray())
			{
				Log("Parameter Set is missing\n");
				continue;
			}
			for (auto iter = nodeParameterSet.Begin(); iter != nodeParameterSet.End(); ++iter)
			{
				StaticParameterSet paramSet = m_DefaultParameterSet;

			}
		}
	}

	Material::~Material()
	{}

}