#ifndef CUT_GOODROC_H
#define CUT_GOODROC_H

#include "../SmartHisto_cut.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

namespace Histogram_generation
{

	class Cut_goodroc : public Cut
	{
		public:
			Cut_goodroc(Ntuple_reader*& ntuple_reader_p);
			~Cut_goodroc()
			{
				if(debug)
				{
					std::cout << debug_prompt << "~Cut_goodroc()" << std::endl;
				}
			};
	};

	Cut_goodroc::Cut_goodroc(Ntuple_reader*& ntuple_reader_p)
	{
		this -> cut_function = [&ntuple_reader_p]()
		{
			bool result = true;
			// bool result = (ntuple_reader -> get_event_field_ptr() -> run == 1) ? 1: goodroc_list[p1_rocid][p2_rocid][p3_rocid];
			return result;
		};
	}

} // Histogram_generation

#endif