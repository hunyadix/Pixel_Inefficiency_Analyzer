#ifndef CUT_D0_H
#define CUT_D0_H

#include "../SmartHisto_cut.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

namespace Histogram_generation
{
	class Cut_d0 : public Cut
	{
		public:
			Cut_d0(Ntuple_reader*& ntuple_reader_p);
			~Cut_d0()
			{
				if(debug)
				{
					std::cout << debug_prompt << "~Cut_d0()" << std::endl;
				}
			};
	};

	Cut_d0::Cut_d0(Ntuple_reader*& ntuple_reader_p)
	{
		this -> cut_function = [&ntuple_reader_p]()
		{
			bool result;
			TrajMeasurement traj_field = *(ntuple_reader_p -> get_traj_field_ptr());
			if(traj_field.mod_on.det == 0)
			{
				if(traj_field.mod_on.layer == 1)
				{
					result = std::abs(traj_field.trk.d0) < 0.01;
				}
				else
				{
					result = std::abs(traj_field.trk.d0) < 0.02;
				}
			}
			else
			{
				result = traj_field.mod_on.det == 1 && std::abs(traj_field.trk.d0) < 0.05;
			}
			return result;
		};
	}

} // Histogram_generation

#endif