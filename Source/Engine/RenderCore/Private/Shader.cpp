#include <sstream>

#include "RenderSystem.h"
#include "Shader.h"
#include "Utility.hpp"

namespace Space
{
	namespace Render
	{
		namespace Details
		{
		}

		std::string ShaderMacro::GetString() const
		{
			std::ostringstream ss;
			for(auto& key_value : _macros)
			{
				ss << "#define" << key_value.first << " " << key_value.second << std::endl;
			}
			return ss.str();
		}

		void ShaderMacro::Undef(std::string const& name)
		{
			_macros.erase(name);
		}
		void ShaderMacro::DefineFloat(std::string const& name, float value)
		{
			_macros.insert_or_assign(name, StringFormatA("(%.5f)", value));
		}
		void ShaderMacro::DefineInt(std::string const& name, int value)
		{
			_macros.insert_or_assign(name, StringFormatA("(%d)", value));
		}
		void ShaderMacro::DefineBool(std::string const& name, bool value)
		{
			_macros.insert_or_assign(name, value ? "TRUE" : "FALSE");
		}
	}
}
