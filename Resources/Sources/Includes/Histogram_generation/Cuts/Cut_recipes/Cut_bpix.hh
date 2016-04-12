#ifndef CUT_BPIX_H
#define CUT_BPIX_H

#include "../SmartHisto_cut.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

namespace Histogram_generation
{

	class Cut_bpix : public Cut
	{
		public:
			Cut_bpix(Ntuple_reader*& ntuple_reader_p);
			~Cut_bpix()
			{
				if(debug)
				{
					std::cout << debug_prompt << "~Cut_bpix()" << std::endl;
				}
			};
	};

	Cut_bpix::Cut_bpix(Ntuple_reader*& ntuple_reader_p)
	{
		this -> cut_function = [&ntuple_reader_p]() 
		{
			bool result = (ntuple_reader_p -> get_module_field_ptr() -> det == 0);
			return result;
		};
	}

} // Histogram_generation

#endif