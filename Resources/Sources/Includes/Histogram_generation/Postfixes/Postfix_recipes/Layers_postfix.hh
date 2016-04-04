#ifndef LAYERS_POSTFIX_H
#define LAYERS_POSTFIX_H

//#include "../../SmartHistos/SmartHistos.hh"
#include "../Postfix_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

class Layers_postfix : public Postfix
{
	public:
		Layers_postfix(Ntuple_reader*& ntuple_reader_p);
		~Layers_postfix() 
		{
			if(debug)
			{
				std::cout << debug_prompt << "~Layers_postfix()" << std::endl; 
			} 
		};
};

Layers_postfix::Layers_postfix(Ntuple_reader*& ntuple_reader_p)
{
	this -> name = "Layers";
	this -> sel = [&ntuple_reader_p]()
	{
		if(ntuple_reader_p -> get_module_field_ptr() -> det == 1)
		{
			return -1;
		}
		return ntuple_reader_p -> get_module_field_ptr() -> layer - 1;
	};
	this -> pf = "Layer[1to3]";
	this -> leg = "L[1to3]";
	this -> colz = col3_red_to_blue;
}

#endif