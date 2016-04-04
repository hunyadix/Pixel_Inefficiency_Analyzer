#ifndef CUT_FEDERR_CUT_H
#define CUT_FEDERR_CUT_H

#include "../SmartHisto_cut.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

namespace Histogram_generation
{

	class Cut_federr : public Cut
	{
		public:
			Cut_federr(Ntuple_reader*& ntuple_reader_p);
			~Cut_federr()
			{
				if(debug)
				{
					std::cout << debug_prompt << "~Cut_federr()" << std::endl;
				}
			};
	};

	Cut_federr::Cut_federr(Ntuple_reader*& ntuple_reader_p)
	{
		this -> cut_function = [&ntuple_reader_p]()
		{
			bool result = ntuple_reader_p -> get_event_field_ptr() -> federrs_size == 0;
			return result;
		};
	}
	
} // Histogram_generation

#endif