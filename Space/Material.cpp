#include "Log.h"
#include "Shader.hpp"
#include "ShaderReflection.hpp"
#include "Material.hpp"

namespace Space
{
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
		std::fstream tempFile(name + ".ps.cso", std::ios_base::in| std::ios_base::binary);
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
			return new Material(pRenderSys,name),
			return nullptr);
	}

	using namespace rapidjson;

	Material::Material(RenderSystem* pRenderSys, std::string const& name)
	{
		std::fstream tempFile(GetAssetsPath() + "Material/"+ name + "/ContentDesc.json", std::ios_base::in | std::ios_base::binary);
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

		Value& nodeName = nodeMaterial["Name"];
		if (nodeName.IsNull())
		{
			throw std::exception("No Material Name");
		}
		m_Name = Name(nodeName.GetString());

		Value& nodeShaders = nodeMaterial["Shaders"];



	}

	Material::~Material()
	{}

}