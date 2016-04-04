#ifndef IMPACTANGLEY_FILL_PARAMETER_H
#define IMPACTANGLEY_FILL_PARAMETER_H

//#include "../../SmartHistos/SmartHistos.hh"
#include "../Fill_parameter_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

class ImpactAngleY_fill_parameter : public Fill_parameter
{
	public:
		ImpactAngleY_fill_parameter(Ntuple_reader*& ntuple_reader_p);
		~ImpactAngleY_fill_parameter()
		{
			if(debug)
			{
				std::cout << debug_prompt << "~ImpactAngleY_fill_parameter()" << std::endl;
			}
		};
};

ImpactAngleY_fill_parameter::ImpactAngleY_fill_parameter(Ntuple_reader*& ntuple_reader_p)
{
	this -> name = "ImpactAngleY";
	this -> nbin = 45; 
	this -> bins = { -22.5, 22.5 };
	this -> fill = [&ntuple_reader_p]()
	{ 
		return ntuple_reader_p -> get_module_field_ptr() -> ladder; 
	};
	this -> axis_title = "Ladders"; 
}

#endif