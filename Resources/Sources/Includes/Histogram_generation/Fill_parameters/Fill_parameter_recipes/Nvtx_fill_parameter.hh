#ifndef NVTX_FILL_PARAMETER_H
#define NVTX_FILL_PARAMETER_H

#include "../Fill_parameter_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"
#include "../../../Console_actor/Console_actor.hh"
#include "../../../ROC_ID_converter/roc_geometry_functions.hh"

#include <string>
#include <functional>

extern bool debug;

class Nvtx_fill_parameter : public Fill_parameter
{
	public:
		Nvtx_fill_parameter(Ntuple_reader*& ntuple_reader_p);
		~Nvtx_fill_parameter()
		{
			if(debug)
			{
				std::cout << debug_prompt << "~Nvtx_fill_parameter()" << std::endl;
			}
		};
};

//72, -4.5, 4.5, 42, -10.5, 10.5

Nvtx_fill_parameter::Nvtx_fill_parameter(Ntuple_reader*& ntuple_reader_p)
{
	this -> name = "Nvtx"; // modules
	this -> nbin = 100; 
	this -> bins = { 0, 100 };
	this -> fill = [&ntuple_reader_p]()
	{
		double nvtx_fill_value = ntuple_reader_p -> get_event_field_ptr() -> nvtx;
		return nvtx_fill_value; 
	};
	this -> axis_title = "Nvtx";
}

#endif