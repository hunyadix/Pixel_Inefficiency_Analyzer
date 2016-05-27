#ifndef LAYERSDISKS_FILL_PARAMETER_H
#define LAYERSDISKS_FILL_PARAMETER_H

//#include "../../SmartHistos/SmartHistos.hh"
#include "../Fill_parameter_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

class LayersDisks_fill_parameter : public Fill_parameter
{
	public:
		LayersDisks_fill_parameter(Ntuple_reader*& ntuple_reader_p);
		~LayersDisks_fill_parameter()
		{
			if(debug)
			{
				std::cout << debug_prompt << "~LayersDisks_fill_parameter()" << std::endl;
			}
		};
};

LayersDisks_fill_parameter::LayersDisks_fill_parameter(Ntuple_reader*& ntuple_reader_p)
{
	this -> name = "LayersDisks";
	this -> nbin = 3;
	this -> bins = { 0.5, 7.5 };
	this -> fill = [&ntuple_reader_p]()
	{
		auto module_ptr = ntuple_reader_p -> get_module_field_ptr();
		auto disk = module_ptr -> disk;
		if(module_ptr -> det == 0)
		{
			return module_ptr -> layer;
		}

// Disk variable values:
// -2, -1, 1, 2
// Test:
// -2 => -2 + 5 + 1 = 4
// -1 => -1 + 5 + 1 = 5
//  1 =>  1 + 5 + 0 = 6
//  2 =>  2 + 5 + 0 = 7

		return disk + 5 + (disk < 0);
		// return ntuple_reader_p -> get_module_field_ptr() -> layer; 
	};
	this -> axis_title = "Layers, disks"; 
}

#endif