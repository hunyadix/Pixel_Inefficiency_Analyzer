#ifndef CUT_DZ_H
#define CUT_DZ_H

#include "../SmartHisto_cut.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

namespace Histogram_generation
{

	class Cut_dz : public Cut
	{
		public:
			Cut_dz(Ntuple_reader*& ntuple_reader_p);
			~Cut_dz()
			{
				if(debug)
				{
					std::cout << debug_prompt << "~Cut_dz()" << std::endl;
				}
			};
	};

	Cut_dz::Cut_dz(Ntuple_reader*& ntuple_reader_p)
	{
		this -> cut_function = [&ntuple_reader_p]()
		{
			bool result =
				(ntuple_reader_p -> get_traj_field_ptr() -> mod_on.det == 0 && std::abs(ntuple_reader_p -> get_traj_field_ptr() -> trk.dz) < 0.1) || 
				(ntuple_reader_p -> get_traj_field_ptr() -> mod_on.det == 1 && std::abs(ntuple_reader_p -> get_traj_field_ptr() -> trk.dz) < 0.5);
			return result;
		};
	}

} // Histogram_generation

#endif