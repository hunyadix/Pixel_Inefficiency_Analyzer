#ifndef MODULES_POSTFIX_H
#define MODULES_POSTFIX_H

//#include "../../SmartHistos/SmartHistos.hh"
#include "../Postfix_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

class Modules_postfix : public Postfix
{
	public:
		Modules_postfix(Ntuple_reader*& ntuple_reader_p);
		~Modules_postfix() 
		{
			if(debug)
			{
				std::cout << debug_prompt << "~Modules_postfix()" << std::endl; 
			} 
		};
};

Modules_postfix::Modules_postfix(Ntuple_reader*& ntuple_reader_p)
{
	this -> name = "Modules";
	this -> sel = [&ntuple_reader_p]()
	{
		if(ntuple_reader_p -> get_module_field_ptr() -> det == 1)
		{
			return -1;
		}
		return std::abs(ntuple_reader_p -> get_module_field_ptr() -> module) - 1;
	};
	this -> pf = "Module[1to4];";
	this -> leg = "M[1to4]";
	this -> colz = col4_cyan_to_red;
}

#endif