#ifndef CUT_PT_H
#define CUT_PT_H

#include "../SmartHisto_cut.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

namespace Histogram_generation
{

	class Cut_pt : public Cut
	{
		public:
			Cut_pt(Ntuple_reader*& ntuple_reader_p);
			~Cut_pt()
			{
				if(debug)
				{
					std::cout << debug_prompt << "~Cut_pt()" << std::endl;
				}
			};
	};

	Cut_pt::Cut_pt(Ntuple_reader*& ntuple_reader_p)
	{
		this -> cut_function = [&ntuple_reader_p]() 
		{
			bool result = ntuple_reader_p -> get_traj_field_ptr() -> trk.pt > 1.0;
			return result;
		};
	}

} // Histogram_generation

#endif