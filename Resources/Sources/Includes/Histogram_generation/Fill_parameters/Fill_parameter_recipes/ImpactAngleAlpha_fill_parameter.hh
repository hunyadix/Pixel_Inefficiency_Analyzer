#ifndef IMPACTANGLEALPHA_FILL_PARAMETER_H
#define IMPACTANGLEALPHA_FILL_PARAMETER_H

//#include "../../SmartHistos/SmartHistos.hh"
#include "../Fill_parameter_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

class ImpactAngleAlpha_fill_parameter : public Fill_parameter
{
	private:
		float deg;
	public:
		ImpactAngleAlpha_fill_parameter(Ntuple_reader*& ntuple_reader_p);
		~ImpactAngleAlpha_fill_parameter()
		{
			if(debug)
			{
				std::cout << debug_prompt << "~ImpactAngleAlpha_fill_parameter()" << std::endl;
			}
		};
};

ImpactAngleAlpha_fill_parameter::ImpactAngleAlpha_fill_parameter(Ntuple_reader*& ntuple_reader_p)
{
	this -> name = "ImpactAngleAlpha";
	this -> nbin = 90; 
	this -> bins = { 0.0, 90 };
	this -> deg = 180 / 3.141516925;
	this -> fill = [&ntuple_reader_p, this]()
	{
		// float alpha = std::abs(ntuple_reader_p -> get_traj_field_ptr() -> alpha);
		// std::cerr << debug_prompt << "Alpha is " << alpha << std::endl;
		return std::abs(90 - std::abs(ntuple_reader_p -> get_traj_field_ptr() -> alpha * this -> deg));
	};
	this -> axis_title = "alpha (impact angle)"; 
}

#endif
