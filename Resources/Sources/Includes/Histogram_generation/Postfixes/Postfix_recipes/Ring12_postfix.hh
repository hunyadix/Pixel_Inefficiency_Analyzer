// Thu Mar 31 09:04:38 2016 version bugged, use newer!
// Mon Apr  4 12:45:58 2016 fixed

#ifndef RING12_POSTFIX_H
#define RING12_POSTFIX_H

//#include "../../SmartHistos/SmartHistos.hh"
#include "../Postfix_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

class Ring12_postfix : public Postfix
{
	public:
		Ring12_postfix(Ntuple_reader*& ntuple_reader_p);
		~Ring12_postfix() 
		{
			if(debug)
			{
				std::cout << debug_prompt << "~Ring12_postfix()" << std::endl; 
			} 
		};
};

Ring12_postfix::Ring12_postfix(Ntuple_reader*& ntuple_reader_p)
{
	this -> name = "Ring12";
	this -> sel = [&ntuple_reader_p]()
	{
		if(ntuple_reader_p -> get_module_field_ptr() -> det == 1)
		{
			return -1;
		}
		if(std::abs(ntuple_reader_p -> get_module_field_ptr() -> module) > 2)
		{
			return 1;
		}
		return 0;
	};
	this -> pf = "Ring12;Ring34";
	this -> leg = "Ring 1, 2;Ring 3, 4";
	this -> colz = "600,820,";
}

#endif