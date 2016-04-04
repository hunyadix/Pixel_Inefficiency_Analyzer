#ifndef IMPACTANGLEBETA_FILL_PARAMETER_H
#define IMPACTANGLEBETA_FILL_PARAMETER_H

//#include "../../SmartHistos/SmartHistos.hh"
#include "../Fill_parameter_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

class ImpactAngleBeta_fill_parameter : public Fill_parameter
{
	private:
		double deg;
	public:
		ImpactAngleBeta_fill_parameter(Ntuple_reader*& ntuple_reader_p);
		~ImpactAngleBeta_fill_parameter()
		{
			if(debug)
			{
				std::cout << debug_prompt << "~ImpactAngleBeta_fill_parameter()" << std::endl;
			}
		};
};

ImpactAngleBeta_fill_parameter::ImpactAngleBeta_fill_parameter(Ntuple_reader*& ntuple_reader_p)
{
	this -> name = "ImpactAngleBeta";
	this -> nbin = 100; 
	this -> bins = { 0.0, 360.0 };
	this -> deg  = 180 / 3.141516925;
	this -> fill = [&ntuple_reader_p, this]()
	{
		// std::cerr << debug_prompt << "Beta is " << std::abs(ntuple_reader_p -> get_traj_field_ptr() -> beta) << std::endl;
		return std::abs(ntuple_reader_p -> get_traj_field_ptr() -> beta * this -> deg); 
	};
	this -> axis_title = "beta (impact angle)"; 
}

#endif