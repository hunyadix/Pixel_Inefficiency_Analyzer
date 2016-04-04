#ifndef TRAJROCY_FILL_PARAMETER_H
#define TRAJROCY_FILL_PARAMETER_H

#include "../Fill_parameter_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"
#include "../../../Console_actor/Console_actor.hh"
#include "../../../ROC_ID_converter/roc_geometry_functions.hh"

#include <string>
#include <functional>

extern bool debug;

class TrajROCy_fill_parameter : public Fill_parameter
{
	public:
		TrajROCy_fill_parameter(Ntuple_reader*& ntuple_reader_p);
		~TrajROCy_fill_parameter()
		{
			if(debug)
			{
				std::cout << debug_prompt << "~TrajROCy_fill_parameter()" << std::endl;
			}
		};
};

//72, -4.5, 4.5, 42, -10.5, 10.5

TrajROCy_fill_parameter::TrajROCy_fill_parameter(Ntuple_reader*& ntuple_reader_p)
{
	this -> name = "TrajROCy"; 	// ladders
	this -> nbin = 90;
	this -> bins = { -22.5, 22.5 };
	this -> fill = [&ntuple_reader_p]()
	{
		if(ntuple_reader_p -> get_module_field_ptr() -> det != 0)
		{
			return -9999.0;
		}
		if(ntuple_reader_p -> get_traj_field_ptr() -> validhit == 0 && ntuple_reader_p -> get_traj_field_ptr() -> missing == 0)
		{
			return -9999.0;
		}
		double ladder_fill_value = Barrel_ROC::get_ladder_fill_value_from_local_x(ntuple_reader_p -> get_traj_field_ptr() -> lx, *(ntuple_reader_p -> get_module_field_ptr()));
		return ladder_fill_value; 
	};
	this -> axis_title = "Ladders"; 
}

#endif