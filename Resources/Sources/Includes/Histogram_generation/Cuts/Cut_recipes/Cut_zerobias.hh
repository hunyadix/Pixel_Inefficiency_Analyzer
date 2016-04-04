#ifndef CUT_ZEROBIAS_CUT_H
#define CUT_ZEROBIAS_CUT_H

#include "../SmartHisto_cut.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

namespace Histogram_generation
{

	class Cut_zerobias: public Cut
	{
		public:
			Cut_zerobias(Ntuple_reader*& ntuple_reader_p);
			~Cut_zerobias()
			{
				if(debug)
				{
					std::cout << debug_prompt << "~Cut_zerobias()" << std::endl;
				}
			};
	};

	Cut_zerobias::Cut_zerobias(Ntuple_reader*& ntuple_reader_p)
	{
		this -> name = "cut_zerobias";
		this -> cut_function = [&ntuple_reader_p]()
		{
			bool result = ntuple_reader_p -> get_event_field_ptr() -> trig & 1;
			return result;
		};
	}

} // Histogram_generation

#endif