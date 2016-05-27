#ifndef LOCALPHI_FILL_PARAMETER_H
#define LOCALPHI_FILL_PARAMETER_H

//#include "../../SmartHistos/SmartHistos.hh"
#include "../Fill_parameter_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

class LocalPhi_fill_parameter : public Fill_parameter
{
	private:
		float deg;
	public:
		LocalPhi_fill_parameter(Ntuple_reader*& ntuple_reader_p);
		~LocalPhi_fill_parameter()
		{
			if(debug)
			{
				std::cout << debug_prompt << "~LocalPhi_fill_parameter()" << std::endl;
			}
		};
};

LocalPhi_fill_parameter::LocalPhi_fill_parameter(Ntuple_reader*& ntuple_reader_p)
{
	this -> deg = 180.0 / 3.141516925;
	this -> name = "LocalPhi";
	this -> nbin = 180; 
	this -> bins = { 0.0, 180.0 };
	this -> fill = [&ntuple_reader_p, this]()
	{
		float alpha = std::abs(ntuple_reader_p -> get_traj_field_ptr() -> alpha);
		float beta  = std::abs(ntuple_reader_p -> get_traj_field_ptr() -> beta);
		float phi   = 90.0 - deg * atan2(tan(beta), tan(alpha));
		if(phi < 0)
		{
			phi += 360.0;
		}
// Reflect the values that are greater than 180 degree:
// 1: get the distance to 180 degree.
// 2: transform by substracting this distance from 180 degree
		if(180 < phi)
		{
			// phi = 180 - (phi - 180);
			phi = 360 - phi;
		}
		return phi;
	};
	this -> axis_title = "Local #phi (degree)"; 
}

#endif
