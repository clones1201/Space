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


	class Pass : virtual public Object<Pass>
	{
	public:
		void Apply();
	private:
		Shader* vertex;
		Shader* pixel;
	};

	class Technique : virtual public Object<Technique>
	{
	public:
		int GetPassCount() const;
		Pass* GetPassByIndex(int index)const;
	private:
		std::vector<Pass::Ptr> Passes;
	};

	class MaterialSelector : virtual public Object<Material>
	{

	};

	class Material : virtual public Object<Material>
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