#ifndef CUT_NSTRIP_H
#define CUT_NSTRIP_H

#include "../SmartHisto_cut.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

namespace Histogram_generation
{

	class Cut_nstrip : public Cut
	{
		public:
			Cut_nstrip(Ntuple_reader*& ntuple_reader_p);
			~Cut_nstrip()
			{
				if(debug)
				{
					std::cout << debug_prompt << "~Cut_nstrip()" << std::endl;
				}
			};
	};

	Cut_nstrip::Cut_nstrip(Ntuple_reader*& ntuple_reader_p)
	{
		this -> cut_function = [&ntuple_reader_p]()
		{
			bool result = ntuple_reader_p -> get_traj_field_ptr() -> trk.strip > 10;
			return result;
		};
	}

} // Histogram_generation

#endif