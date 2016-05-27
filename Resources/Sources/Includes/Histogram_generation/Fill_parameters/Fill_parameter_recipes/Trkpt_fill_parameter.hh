#ifndef TRKPT_FILL_PARAMETER_H
#define TRKPT_FILL_PARAMETER_H

//#include "../../SmartHistos/SmartHistos.hh"
#include "../Fill_parameter_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

class Trkpt_fill_parameter : public Fill_parameter
{
	public:
		Trkpt_fill_parameter(Ntuple_reader*& ntuple_reader_p);
		~Trkpt_fill_parameter()
		{
			if(debug)
			{
				std::cout << debug_prompt << "~Trkpt_fill_parameter()" << std::endl;
			}
		};
};

Trkpt_fill_parameter::Trkpt_fill_parameter(Ntuple_reader*& ntuple_reader_p)
{
	this -> name = "Trkpt";
	this -> nbin = 9; 
	this -> bins = {0, 0.2, 0.4, 0.6, 0.8, 1, 1.5, 2, 5, 20};
	this -> fill = [&ntuple_reader_p]()
	{
		return ntuple_reader_p -> get_traj_field_ptr() -> trk.pt;
	};
	this -> axis_title = "Trackp_{T} (GeV/c)"; 
}

#endif