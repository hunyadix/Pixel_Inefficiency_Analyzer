#ifndef CLUSTROCY_FILL_PARAMETER_H
#define CLUSTROCY_FILL_PARAMETER_H

#include "../Fill_parameter_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"
#include "../../../Console_actor/Console_actor.hh"
#include "../../../ROC_ID_converter/roc_geometry_functions.hh"

#include <string>
#include <functional>

extern bool debug;

class ClustROCy_fill_parameter : public Fill_parameter
{
	public:
		ClustROCy_fill_parameter(Ntuple_reader*& ntuple_reader_p);
		~ClustROCy_fill_parameter()
		{
			if(debug)
			{
				std::cout << debug_prompt << "~ClustROCy_fill_parameter()" << std::endl;
			}
		};
};

//72, -4.5, 4.5, 42, -10.5, 10.5

ClustROCy_fill_parameter::ClustROCy_fill_parameter(Ntuple_reader*& ntuple_reader_p)
{
	this -> name = "ClustROCy"; // ladders
	this -> nbin = 90; 
	this -> bins = { -22.5, 22.5 };
	this -> fill = [&ntuple_reader_p]()
	{
		if(ntuple_reader_p -> get_module_field_ptr() -> det != 0)
		{
			return -9999.0;
		}
		double ladder_fill_value = Barrel_ROC::get_ladder_fill_value_from_cluster_xy(  ntuple_reader_p -> get_cluster_field_ptr() -> x,
			                                                                              ntuple_reader_p -> get_cluster_field_ptr() -> y,
			                                                                            *(ntuple_reader_p -> get_module_field_ptr()));
		return ladder_fill_value; 
	};
	this -> axis_title = "Ladders"; 
}

#endif