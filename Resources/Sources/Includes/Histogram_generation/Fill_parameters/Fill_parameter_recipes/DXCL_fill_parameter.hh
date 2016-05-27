#ifndef DXCL_FILL_PARAMETER_H
#define DXCL_FILL_PARAMETER_H

//#include "../../SmartHistos/SmartHistos.hh"
#include "../Fill_parameter_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

class DXCL_fill_parameter : public Fill_parameter
{
	private:
		double deg;
	public:
		DXCL_fill_parameter(Ntuple_reader*& ntuple_reader_p);
		~DXCL_fill_parameter()
		{
			if(debug)
			{
				std::cout << debug_prompt << "~DXCL_fill_parameter()" << std::endl;
			}
		};
};

DXCL_fill_parameter::DXCL_fill_parameter(Ntuple_reader*& ntuple_reader_p)
{
	this -> name = "DXCL";
	this -> nbin = 2000; 
	this -> bins = { 0.0, 100000.0 };
	this -> fill = [&ntuple_reader_p]()
	{
		float dx_cl = ntuple_reader_p -> get_traj_field_ptr() -> dx_cl[0] * 10000.0;
		// std::cerr << debug_prompt << "DXCL:" dx_cl << std::endl;
		return dx_cl;
	};
	this -> axis_title = "Distance to closest cluster in x direction (#mu m)"; 
}

#endif