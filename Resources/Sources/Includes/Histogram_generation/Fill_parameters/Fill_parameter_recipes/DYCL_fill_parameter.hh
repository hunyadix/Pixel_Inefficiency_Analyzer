#ifndef DYCL_FILL_PARAMETER_H
#define DYCL_FILL_PARAMETER_H

//#include "../../SmartHistos/SmartHistos.hh"
#include "../Fill_parameter_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

class DYCL_fill_parameter : public Fill_parameter
{
	private:
		double deg;
	public:
		DYCL_fill_parameter(Ntuple_reader*& ntuple_reader_p);
		~DYCL_fill_parameter()
		{
			if(debug)
			{
				std::cout << debug_prompt << "~DYCL_fill_parameter()" << std::endl;
			}
		};
};

DYCL_fill_parameter::DYCL_fill_parameter(Ntuple_reader*& ntuple_reader_p)
{
	this -> name = "DYCL";
	this -> nbin = 2000; 
	this -> bins = { 0.0, 100000.0 };
	this -> fill = [&ntuple_reader_p]()
	{
		float dy_cl = ntuple_reader_p -> get_traj_field_ptr() -> dy_cl[0] * 10000.0;
		return dy_cl;
	};
	this -> axis_title = "Distance to closest cluster in y direction (#mum)"; 
}

#endif