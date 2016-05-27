#ifndef DRCL_SECOND_CLUSTER_FILL_PARAMETER_H
#define DRCL_SECOND_CLUSTER_FILL_PARAMETER_H

//#include "../../SmartHistos/SmartHistos.hh"
#include "../Fill_parameter_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

class DRCL_second_cluster_fill_parameter : public Fill_parameter
{
	private:
		double deg;
	public:
		DRCL_second_cluster_fill_parameter(Ntuple_reader*& ntuple_reader_p);
		~DRCL_second_cluster_fill_parameter()
		{
			if(debug)
			{
				std::cout << debug_prompt << "~DRCL_second_cluster_fill_parameter()" << std::endl;
			}
		};
};

DRCL_second_cluster_fill_parameter::DRCL_second_cluster_fill_parameter(Ntuple_reader*& ntuple_reader_p)
{
	this -> name = "DRCLSecondCluster";
	this -> nbin = 200000;
	this -> bins = { 0.0, 2000000.0 };
	this -> fill = [&ntuple_reader_p]()
	{
		float dx_cl = ntuple_reader_p -> get_traj_field_ptr() -> dx_cl[1] * 10000.0;
		float dy_cl = ntuple_reader_p -> get_traj_field_ptr() -> dy_cl[1] * 10000.0;
		// std::cerr << debug_prompt << "DXCL:" dx_cl << std::endl;
		return sqrt(dx_cl * dx_cl + dy_cl * dy_cl);
	};
	this -> axis_title = "Distance to closest cluster (#mum)"; 
}

#endif