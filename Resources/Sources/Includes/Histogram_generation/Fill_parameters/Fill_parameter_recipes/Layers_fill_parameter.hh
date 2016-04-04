#ifndef LAYERS_FILL_PARAMETER_H
#define LAYERS_FILL_PARAMETER_H

//#include "../../SmartHistos/SmartHistos.hh"
#include "../Fill_parameter_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

class Layers_fill_parameter : public Fill_parameter
{
	public:
		Layers_fill_parameter(Ntuple_reader*& ntuple_reader_p);
		~Layers_fill_parameter()
		{
			if(debug)
			{
				std::cout << debug_prompt << "~Layers_fill_parameter()" << std::endl;
			}
		};
};

Layers_fill_parameter::Layers_fill_parameter(Ntuple_reader*& ntuple_reader_p)
{
	this -> name = "Layers";
	this -> nbin = 3;
	this -> bins = { 0.5, 3.5 };
	this -> fill = [&ntuple_reader_p]()
	{ 
		return ntuple_reader_p -> get_module_field_ptr() -> layer; 
	};
	this -> axis_title = "Layers"; 
}

#endif