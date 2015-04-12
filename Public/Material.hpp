#ifndef __SPACE_MATERIAL_HPP__
#define __SPACE_MATERIAL_HPP__

#include "Prerequisites.hpp"
#include "TemplateTrait.hpp"
#include "Basic.hpp"
#include "Shader.hpp"

namespace Space
{
	class MaterialParameter
	{

	};

	typedef std::vector<MaterialParameter> MaterialParameterSet;


	class Pass
	{
	public:
		void Apply();
	private:
		Shader* vertex;
		Shader* pixel;
	};

	class Technique
	{
	public:
		int32 GetPassCount() const;
		Pass* GetPassByIndex(int32 index)const;
	private:
		std::vector<TypeTrait<Pass>::Ptr> Passes;
	};

	class MaterialSelector
	{

	};

	class Material
	{
	public:

	private:
		std::map<MaterialParameterSet, Technique> shaderMap;
	};

	class BasicMaterial : virtual public Material
	{
	public:


	};

	class CustomMaterial : public Material
	{

	};

}

#endif