#ifndef CUT_PIXHIT_H
#define CUT_PIXHIT_H

#include "../SmartHisto_cut.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

namespace Histogram_generation
{

	class Cut_pixhit : public Cut
	{
		public:
			Cut_pixhit(Ntuple_reader*& ntuple_reader_p);
			~Cut_pixhit()
			{
				if(debug)
				{
					std::cout << debug_prompt << "~Cut_pixhit()" << std::endl;
				}
			};
	};

	Cut_pixhit::Cut_pixhit(Ntuple_reader*& ntuple_reader_p)
	{
		this -> cut_function = [&ntuple_reader_p]()
		{
			TrajMeasurement traj_field = *(ntuple_reader_p -> get_traj_field_ptr());
			bool result = traj_field.mod_on.det==0 ?
			(// BPix
			traj_field.mod_on.layer==1 ?
			(traj_field.trk.validbpix[1]>0 && traj_field.trk.validbpix[2]>0) // L2 + L3
			|| (traj_field.trk.validbpix[1]>0 && traj_field.trk.validfpix[0]>0) // L2 + D1
			|| (traj_field.trk.validfpix[0]>0 && traj_field.trk.validfpix[1]>0) // D1 + D2
			: traj_field.mod_on.layer==2 ?
			(traj_field.trk.validbpix[0]>0 && traj_field.trk.validbpix[2]>0) // L1 + L3
			|| (traj_field.trk.validbpix[0]>0 && traj_field.trk.validfpix[0]>0) // L1 + D1
			: traj_field.mod_on.layer==3 &&
			traj_field.trk.validbpix[0]>0 && traj_field.trk.validbpix[1]>0) // L1 + L2
			: traj_field.mod_on.det==1 &&
			(// FPix
			std::abs(traj_field.mod_on.disk)==1 ?
			(traj_field.trk.validbpix[0]>0 && traj_field.trk.validfpix[1]>0) // L1 + D2
			|| (traj_field.trk.validbpix[1]>0 && traj_field.trk.validfpix[1]>0) // or L2 + D2
			: std::abs(traj_field.mod_on.disk)==2 &&
			traj_field.trk.validbpix[0]>0 && traj_field.trk.validfpix[0]>0); // L1 + D1
			return result;
		};
	}

} // Histogram_generation

#endif