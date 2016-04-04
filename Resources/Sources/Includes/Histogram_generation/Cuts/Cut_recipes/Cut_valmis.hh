#ifndef CUT_VALMIS_H
#define CUT_VALMIS_H

#include "../SmartHisto_cut.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

namespace Histogram_generation
{

	class Cut_valmis : public Cut
	{
		public:
			Cut_valmis(Ntuple_reader*& ntuple_reader_p);
			~Cut_valmis()
			{
				if(debug)
				{
					std::cout << debug_prompt << "~Cut_valmis()" << std::endl;
				}
			};
	};

	Cut_valmis::Cut_valmis(Ntuple_reader*& ntuple_reader_p)
	{
		this -> cut_function = [&ntuple_reader_p]()
		{
			TrajMeasurement& traj_field = *(ntuple_reader_p -> get_traj_field_ptr());
			bool result = traj_field.validhit == 1 || traj_field.missing == 1;
			return result;
		};
	}

} // Histogram_generation

#endif