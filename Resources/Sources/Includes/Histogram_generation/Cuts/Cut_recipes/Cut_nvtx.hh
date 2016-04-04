#ifndef CUT_NVTX_CUT_H
#define CUT_NVTX_CUT_H

#include "../SmartHisto_cut.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

namespace Histogram_generation
{
	class Cut_nvtx: public Cut
	{
		public:
			Cut_nvtx(Ntuple_reader*& ntuple_reader_p);
			~Cut_nvtx()
			{
				if(debug)
				{
					std::cout << debug_prompt << "~Cut_nvtx()" << std::endl;
				}
			};
	};

	Cut_nvtx::Cut_nvtx(Ntuple_reader*& ntuple_reader_p)
	{
		this -> name = "cut_nvtx";
		this -> cut_function = [&ntuple_reader_p]()
		{
			bool result = ntuple_reader_p -> get_event_field_ptr() -> nvtx >= 1;
			return result;
		};
	}

} // Histogram_generation

#endif