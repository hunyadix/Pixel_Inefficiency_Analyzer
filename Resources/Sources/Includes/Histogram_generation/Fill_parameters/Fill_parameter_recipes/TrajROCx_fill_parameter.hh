#ifndef TRAJROCX_FILL_PARAMETER_H
#define TRAJROCX_FILL_PARAMETER_H

#include "../Fill_parameter_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"
#include "../../../Console_actor/Console_actor.hh"
#include "../../../ROC_ID_converter/roc_geometry_functions.hh"

#include <string>
#include <functional>

extern bool debug;

class TrajROCx_fill_parameter : public Fill_parameter
{
	public:
		TrajROCx_fill_parameter( Ntuple_reader*& ntuple_reader_p );
		~TrajROCx_fill_parameter()
		{
			if( debug )
			{
				std :: cout << debug_prompt << "~TrajROCx_fill_parameter()" << std :: endl;
			}
		};
};

//72, -4.5, 4.5, 42, -10.5, 10.5

TrajROCx_fill_parameter :: TrajROCx_fill_parameter( Ntuple_reader*& ntuple_reader_p )
{
	this -> name = "TrajROCx"; // modules
	this -> nbin = 72; 
	this -> bins = { -4.5, 4.5 };
	this -> fill = [&ntuple_reader_p]()
	{
		if( ntuple_reader_p -> get_module_field_ptr() -> det != 0 )
		{
			return -9999.0;
		}
		if( ntuple_reader_p -> get_traj_field_ptr() -> validhit == 0 && ntuple_reader_p -> get_traj_field_ptr() -> missing == 0 )
		{
			return -9999.0;
		}
		double module_fill_value = Barrel_ROC :: get_module_fill_value_from_local_y( ntuple_reader_p -> get_traj_field_ptr() -> ly, *(ntuple_reader_p -> get_module_field_ptr()) );
		return module_fill_value; 
	};
	this -> axis_title = "Modules"; 
}

#endif