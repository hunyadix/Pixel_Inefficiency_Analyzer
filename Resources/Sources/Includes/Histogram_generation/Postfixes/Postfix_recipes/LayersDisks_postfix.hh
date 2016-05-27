#ifndef LAYERSDISKS_POSTFIX_H
#define LAYERSDISKS_POSTFIX_H

//#include "../../SmartHistos/SmartHistos.hh"
#include "../Postfix_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

NINCS MEGIRVA!

// class LayersDisks_postfix : public Postfix
// {
// 	public:
// 		LayersDisks_postfix(Ntuple_reader*& ntuple_reader_p);
// 		~LayersDisks_postfix() 
// 		{
// 			if(debug)
// 			{
// 				std::cout << debug_prompt << "~LayersDisks_postfix()" << std::endl; 
// 			} 
// 		};
// };

// LayersDisks_postfix::LayersDisks_postfix(Ntuple_reader*& ntuple_reader_p)
// {
// 	this -> name = "LayersDisks";
// 	this -> sel = [&ntuple_reader_p]()
// 	{
// 		if(ntuple_reader_p -> get_module_field_ptr() -> det == 1)
// 		{
// 			return disk + 5 + (disk < 0);
// 		}
// 		return ntuple_reader_p -> get_module_field_ptr() -> layer - 1;
// 	};
// 	this -> pf = "Layer[1to3]";
// 	this -> leg = "L[1to3]";
// 	this -> colz = col3_red_to_blue;
// }

#endif