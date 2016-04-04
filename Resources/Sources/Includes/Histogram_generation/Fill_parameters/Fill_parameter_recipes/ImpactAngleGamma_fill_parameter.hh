#ifndef IMPACTANGLEGAMMA_FILL_PARAMETER_H
#define IMPACTANGLEGAMMA_FILL_PARAMETER_H

//#include "../../SmartHistos/SmartHistos.hh"
#include "../Fill_parameter_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

class ImpactAngleGamma_fill_parameter : public Fill_parameter
{
	private:
		double deg;
	public:
		ImpactAngleGamma_fill_parameter(Ntuple_reader*& ntuple_reader_p);
		~ImpactAngleGamma_fill_parameter()
		{
			if(debug)
			{
				std::cout << debug_prompt << "~ImpactAngleGamma_fill_parameter()" << std::endl;
			}
		};
};

ImpactAngleGamma_fill_parameter::ImpactAngleGamma_fill_parameter(Ntuple_reader*& ntuple_reader_p)
{
	this -> name = "Gamma";
	this -> nbin = 100; 
	this -> bins = { 0.0, 360.0 };
	this -> deg  = 180 / 3.141516925;
	this -> fill = [&ntuple_reader_p, this]()
	{
		float& alpha = ntuple_reader_p -> get_traj_field_ptr() -> alpha;
		float& beta  = ntuple_reader_p -> get_traj_field_ptr() -> beta;
		std::cerr << "Alpha: " << alpha << std::endl;
		std::cerr << "Beta: " << beta << std::endl;
		std::cerr << "Atan2(tan(alpha), tan(beta)): " << atan2(tan(beta), tan(alpha)) << std::endl;
		std::cerr << "Atan2(tan(alpha), tan(beta)) in degrees: " << atan2(tan(beta), tan(alpha)) * this -> deg << std::endl;
		std::cin.get();
		return std::abs(atan2(tan(beta), tan(alpha)) * this -> deg); 
	};
	this -> axis_title = "gamma (impact angle)"; 
}

#endif