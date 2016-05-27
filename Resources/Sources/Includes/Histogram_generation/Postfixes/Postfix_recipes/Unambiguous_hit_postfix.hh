#ifndef UNAMBIGUOUS_HIT_POSTFIX_H
#define UNAMBIGUOUS_HIT_POSTFIX_H

//#include "../../SmartHistos/SmartHistos.hh"
#include "../Postfix_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

class Unambiguous_hit_postfix : public Postfix
{
	public:
		Unambiguous_hit_postfix(Ntuple_reader*& ntuple_reader_p);
		~Unambiguous_hit_postfix() 
		{
			if(debug)
			{
				std::cout << debug_prompt << "~Unambiguous_hit_postfix()" << std::endl; 
			} 
		};
};

Unambiguous_hit_postfix::Unambiguous_hit_postfix(Ntuple_reader*& ntuple_reader_p)
{
	this -> name = "UnambiguousHit";
	this -> sel = [&ntuple_reader_p]()
	{
		if(ntuple_reader_p -> get_module_field_ptr() -> det == 1)
		{
			return -1;
		}
		TrajMeasurement* traj_field = ntuple_reader_p -> get_traj_field_ptr();
		float dx_cl_0 = traj_field -> dx_cl[0] * 10000.0;
		float dy_cl_0 = traj_field -> dy_cl[0] * 10000.0;
		float dx_cl_1 = traj_field -> dx_cl[1] * 10000.0;
		float dy_cl_1 = traj_field -> dy_cl[1] * 10000.0;
		float dr_cl_0 = sqrt(dx_cl_0 * dx_cl_0 + dy_cl_0 * dy_cl_0);
		float dr_cl_1 = sqrt(dx_cl_1 * dx_cl_1 + dy_cl_1 * dy_cl_1);
		// std::cerr << "dr_cl_0: " << dr_cl_0 << std::endl; 
		// std::cerr << "dr_cl_1: " << dr_cl_1 << std::endl;
		// std::cin.get(); 
		if(dr_cl_0 < 500 && 500 < dr_cl_1 )
		{
			return 0;
		}
		return 1;
	};
	this -> pf = "Unambiguous_hit;Ambiguous_hit";
	this -> leg = "Unambiguous_hit;Ambiguous_hit";
	this -> colz = col3_red_to_blue;
}

#endif