#ifndef CUT_HP_H
#define CUT_HP_H

#include "../SmartHisto_cut.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

namespace Histogram_generation
{

	class Cut_hp : public Cut
	{
		public:
			Cut_hp(Ntuple_reader*& ntuple_reader_p);
			~Cut_hp()
			{
				if(debug)
				{
					std::cout << debug_prompt << "~Cut_hp()" << std::endl;
				}
			};
	};

	// High priority trackek azok, amire jó khinégyzettel lehetett megilleszteni a tracket

	Cut_hp::Cut_hp(Ntuple_reader*& ntuple_reader_p)
	{
		this -> cut_function = [&ntuple_reader_p]()
		{
			bool result = (ntuple_reader_p -> get_traj_field_ptr() -> trk.quality & 4) != 0;
			return result;
		};
	}

} // Histogram_generation

#endif