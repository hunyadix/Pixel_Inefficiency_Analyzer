#ifndef CUT_HITSEP_H
#define CUT_HITSEP_H

#include "../SmartHisto_cut.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

namespace Histogram_generation
{

	class Cut_hitsep : public Cut
	{
		public:
			Cut_hitsep(Ntuple_reader*& ntuple_reader_p);
			~Cut_hitsep()
			{
				if(debug)
				{
					std::cout << debug_prompt << "~Cut_hitsep()" << std::endl;
				}
			};
	};

	Cut_hitsep::Cut_hitsep(Ntuple_reader*& ntuple_reader_p)
	{
		this -> cut_function = [&ntuple_reader_p]()
		{
			bool result = ntuple_reader_p -> get_traj_field_ptr() -> hit_near == 0;
			return result;
		};
	}

} // Histogram_generation

#endif