#pragma once

#include "RenderCommon.hpp"
#include "Shared.hpp"

//#include "ShaderReflection.hpp"

namespace Space
{
namespace Render
{
	enum RENDERCORE_API ShaderClass : uint8
	{
		SC_VertexShader,
		SC_GeometryShader,
		SC_HullShader,
		SC_DomainShader,
		SC_PixelShader,
		SC_ComputeShader
	};

	class ShaderSource;
	class ShaderMacro;

	namespace Details
	{
		template<ShaderClass shaderClass>
		class ShaderImpl
		{
		public:				
			static ShaderClass GetClass()
			{
				return shaderClass;
			}
		};

		template<class RenderSystem>
		class VertexShaderImpl : public ShaderImpl<SC_VertexShader>, public RenderSystem::VertexShader
		{
		public:				
			VertexShaderImpl(ShaderSource const& source)
				:RenderSystem::VertexShader(
					Device::GetInstance(),
					RenderSystem::VertexShader::Compile(source)
				)
			{}

			VertexShaderImpl(std::vector<byte> byteCodes)
				: RenderSystem::VertexShader(
					Device::GetInstance(),
					byteCodes)
			{}
		};

		template<class RenderSystem>
		class PixelShaderImpl : public ShaderImpl<SC_PixelShader>, public RenderSystem::PixelShader
		{
		public:
			PixelShaderImpl(ShaderSource const& source)
				:RenderSystem::PixelShader(
					Device::GetInstance(),
					RenderSystem::PixelShader::Compile(source)
				)
			{}

			PixelShaderImpl(std::vector<byte> byteCodes)
				: RenderSystem::PixelShader(
					Device::GetInstance(),
					byteCodes)
			{}
		};
	}

	class ShaderSource
	{
	public:
		std::string FileName;
		std::string Profile;
		std::string Entry;
		std::shared_ptr<ShaderMacro> Macros;
		std::shared_ptr<std::string> StringSource;
		std::vector<byte> ByteCodes;
	};

	class ShaderMacro
	{
	public:
		std::unordered_map<std::string, std::string> _macros;
	
		std::string GetString() const;
		void Undef(std::string const& name);
		void DefineFloat(std::string const& name, float value);
		void DefineInt(std::string const& name, int value);
		void DefineBool(std::string const& name, bool value);
	};


	// class Shader
	// {
	// public:
		// std::shared_ptr<VertexShader> vs;
		// std::shared_ptr<PixelShader> ps;			
	// };
}
}