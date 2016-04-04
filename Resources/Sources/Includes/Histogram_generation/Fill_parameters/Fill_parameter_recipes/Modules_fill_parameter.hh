#ifndef MODULES_FILL_PARAMETER_H
#define MODULES_FILL_PARAMETER_H

//#include "../../SmartHistos/SmartHistos.hh"
#include "../Fill_parameter_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

class Modules_fill_parameter : public Fill_parameter
{
	public:
		Modules_fill_parameter(Ntuple_reader*& ntuple_reader_p);
		~Modules_fill_parameter()
		{
			if(debug)
			{
				std::cout << debug_prompt << "~Modules_fill_parameter()" << std::endl;
			}
		};
};

Modules_fill_parameter::Modules_fill_parameter(Ntuple_reader*& ntuple_reader_p)
{
	this -> name = "Modules";
	this -> nbin = 9; 
	this -> bins = { -4.5, 4.5 };
	this -> fill = [&ntuple_reader_p]()
	{ 
		return ntuple_reader_p -> get_module_field_ptr() -> module;
	};
	this -> axis_title = "Modules"; 
}

#endif