#ifndef HIT_EFFICIENCY_FILL_PARAMETER_H
#define HIT_EFFICIENCY_FILL_PARAMETER_H

//#include "../../SmartHistos/SmartHistos.hh"
#include "../Fill_parameter_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

class Hit_efficiency_fill_parameter : public Fill_parameter
{
	public:
		Hit_efficiency_fill_parameter(Ntuple_reader*& ntuple_reader_p);
		~Hit_efficiency_fill_parameter()
		{
			if(debug)
			{
				std::cout << debug_prompt << "~Hit_efficiency_fill_parameter()" << std::endl;
			}
		};
};

Hit_efficiency_fill_parameter::Hit_efficiency_fill_parameter(Ntuple_reader*& ntuple_reader_p)
{
	this -> name = "HitEfficiency";
	this -> nbin = 2;
	this -> bins = { -0.5, 1.5 };
	this -> fill = [&ntuple_reader_p]()
	{ 
		if(ntuple_reader_p -> get_traj_field_ptr() -> validhit)
		{
			return 1;
		}
		if(ntuple_reader_p -> get_traj_field_ptr() -> missing)
		{
			if(ntuple_reader_p -> get_traj_field_ptr() -> clust_near)
			{
				return 1;
			}
			return 0;
		}
		return -1;
	};
	this -> axis_title = "Hit Efficiency"; 
}

#endif