#ifndef LADDERS_FILL_PARAMETER_H
#define LADDERS_FILL_PARAMETER_H

//#include "../../SmartHistos/SmartHistos.hh"
#include "../Fill_parameter_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

class Ladders_fill_parameter : public Fill_parameter
{
	public:
		Ladders_fill_parameter(Ntuple_reader*& ntuple_reader_p);
		~Ladders_fill_parameter()
		{
			if(debug)
			{
				std::cout << debug_prompt << "~Ladders_fill_parameter()" << std::endl;
			}
		};
};

Ladders_fill_parameter::Ladders_fill_parameter(Ntuple_reader*& ntuple_reader_p)
{
	this -> name = "Ladders";
	this -> nbin = 45; 
	this -> bins = { -22.5, 22.5 };
	this -> fill = [&ntuple_reader_p]()
	{ 
		return ntuple_reader_p -> get_module_field_ptr() -> ladder; 
	};
	this -> axis_title = "Ladders"; 
}

#endif