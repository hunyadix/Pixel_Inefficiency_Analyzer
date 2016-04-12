#ifndef VALIDHIT_POSTFIX_H
#define VALIDHIT_POSTFIX_H

//#include "../../SmartHistos/SmartHistos.hh"
#include "../Postfix_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

class Validhit_postfix : public Postfix
{
	public:
		Validhit_postfix(Ntuple_reader*& ntuple_reader_p);
		~Validhit_postfix() 
		{
			if(debug)
			{
				std::cout << debug_prompt << "~Validhit_postfix()" << std::endl; 
			} 
		};
};

Validhit_postfix::Validhit_postfix(Ntuple_reader*& ntuple_reader_p)
{
	this -> name = "Validhit";
	this -> sel = [&ntuple_reader_p]()
	{
		TrajMeasurement* traj_field = ntuple_reader_p -> get_traj_field_ptr();
		if(traj_field -> validhit == 1)
		{
			return 1;
		}
		if(traj_field -> missing == 1)
		{
			return 0;
		}
		return -1;
	};
	this -> pf = "Missing;Valid";
	this -> leg = "Missing;Valid";
	this -> colz = col3_red_to_blue;
}

#endif