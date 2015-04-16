#ifndef __SPACE_EFFECTS_HPP__
#define __SPACE_EFFECTS_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"
#include "SharedUtility.hpp"

namespace Space
{
	struct StaticBoolParameter //: public Archiveable
	{
		Name parameterName;
		bool value : 1;
		StaticBoolParameter();
		StaticBoolParameter(std::string const& name);
		StaticBoolParameter(std::wstring const& name);
		StaticBoolParameter(const Name& name);

	//	virtual std::ostream& Write(std::ostream& archiver) const;
	//	virtual std::istream& Read(std::istream& archiver);
		bool operator == (const StaticBoolParameter& param) const;
		bool operator != (const StaticBoolParameter& param) const;
	};

	struct StaticComponentMaskParameter : public Archiveable
	{
		Name parameterName;
		bool maskR : 1, maskG : 1, maskB : 1, maskA : 1;
		StaticComponentMaskParameter();
		StaticComponentMaskParameter(std::string const& name);
		StaticComponentMaskParameter(std::wstring const& name);
		StaticComponentMaskParameter(Name const& name);
		
	//	virtual std::ostream& Write(std::ostream& archiver) const;
	//	virtual std::istream& Read(std::istream& archiver);
		bool operator == (const StaticComponentMaskParameter& param) const;
		bool operator != (const StaticComponentMaskParameter& param) const;
	};

	struct StaticParameterSet
	{
		std::vector<StaticBoolParameter> SwitchParameters;
		std::vector<StaticComponentMaskParameter> ComponentMaskParameters;

		std::vector<StaticParameterSet> GetAllStaticParameterSet();
		bool operator == (const StaticParameterSet& param) const;
		bool operator != (const StaticParameterSet& param) const;

	//	virtual std::ostream& Write(std::ostream& archiver) const;
	//	virtual std::istream& Read(std::istream& archiver);
	};



}

#endif