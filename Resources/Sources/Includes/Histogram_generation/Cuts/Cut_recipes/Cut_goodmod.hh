#ifndef CUT_GOODMOD_H
#define CUT_GOODMOD_H

#include "../SmartHisto_cut.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

namespace Histogram_generation
{

	class Cut_goodmod : public Cut
	{
		public:
			Cut_goodmod(Ntuple_reader*& ntuple_reader_p);
			~Cut_goodmod()
			{
				if(debug)
				{
					std::cout << debug_prompt << "~Cut_goodmod()" << std::endl;
				}
			};
	};

	Cut_goodmod::Cut_goodmod(Ntuple_reader*& ntuple_reader_p)
	{
		auto roc_ = [](const float &lx, const float &ly, const ModuleData &m)
		{
			if (m.det==0) 
			{
				if (std::abs(ly)<3.24&&((m.half==0&&std::abs(lx)<0.8164)||(m.half==1&&std::abs(lx)<0.4082))) 
				{
					int nroc_y = (int)(ly / 0.81 + 4);
					int roc = (m.half) ? ((m.module<0)*8+nroc_y) : ((m.module>0) ? ((lx>0.0) ? nroc_y : 15-nroc_y) : ((lx>0.0) ? 8+nroc_y : 7-nroc_y));
					return roc;
				} else return NOVAL_I;
			} else if (m.det==1) 
			{
				int nrocly = m.module + m.panel;
				if (std::abs(ly)<(nrocly*0.405) && ((!(m.panel==1&&(m.module==1||m.module==4)) && std::abs(lx)<0.8164) || (std::abs(lx)<0.4082))) 
				{
					int rocy = (int)((ly+(nrocly%2)*0.405)/0.81+nrocly/2);
					int roc = ((m.panel==2 && lx<0.0)
					|| (m.panel==1 && (m.module==1||m.module==4))
					|| (m.panel==1 && (m.module==2||m.module==3) && lx>0.0)) ? rocy
					: 2*nrocly -1 - rocy;
					return roc;
				} else return NOVAL_I;
			} else return NOVAL_I;
		};
		this -> cut_function = [&ntuple_reader_p, &roc_]()
		{
			TrajMeasurement traj_field = *(ntuple_reader_p -> get_traj_field_ptr());
			EventData event_field      = *(ntuple_reader_p -> get_event_field_ptr());
			int lad = std::abs(traj_field.mod_on.ladder);
			int sector = (traj_field.mod_on.det) ? NOVAL_I : (traj_field.mod_on.layer==1) ? lad - 1 + (lad==1) - (lad==10) : (traj_field.mod_on.layer==2) ? (lad+1)/2 : (lad+2+(lad>11)*2)/3;
			int roc = roc_(traj_field.lx, traj_field.ly, traj_field.mod_on);
			int tbm = roc > 7;
			// Exclude some known bad modules
			bool goodmod_RunI = 
			traj_field.mod_on.det == 0 ?
			// BPix
				(traj_field.mod_on.layer == 1 ?
					!(traj_field.mod_on.ladder ==  4 && traj_field.mod_on.module == -4 && traj_field.lx > 0.0)
				:
					traj_field.mod_on.layer == 2 ?
						!(traj_field.mod_on.ladder == -8 && traj_field.mod_on.module == 1 && traj_field.lx > 0.0) && 
						!(traj_field.mod_on.ladder == -13 && traj_field.mod_on.module == 3 && traj_field.lx < 0.0)
					:
						traj_field.mod_on.layer == 3 && 
						!(traj_field.mod_on.ladder == 14 && traj_field.mod_on.module == -4 && event_field.run >= 141956 && event_field.run < 160404) && 
						!(event_field.run >= 162713 && ((traj_field.mod_on.ladder == 13 && (traj_field.mod_on.module == -1 || traj_field.mod_on.module == -2 || traj_field.mod_on.module == -3)) ||
						(traj_field.mod_on.ladder == 12 && (traj_field.mod_on.module == -1 || traj_field.mod_on.module == -2)) ||
						(traj_field.mod_on.ladder == -17 && traj_field.mod_on.module == -3))))
			: traj_field.mod_on.det == 1 && 
			// FPix
				(traj_field.mod_on.disk == -2 ?
					!(traj_field.mod_on.blade == 10 && traj_field.mod_on.panel == 1)
				: 
					traj_field.mod_on.disk == -1 ?
						!(traj_field.mod_on.blade == 6  && traj_field.mod_on.panel == 1)
					:
						traj_field.mod_on.disk == 1 ?
							!(event_field.run == 146644 && (traj_field.mod_on.blade == -1 || traj_field.mod_on.blade == -2 || traj_field.mod_on.blade == -3)) && // Whole ROG out
							!(event_field.run >= 163655 &&  traj_field.mod_on.blade == -5 && traj_field.mod_on.panel == 2 && traj_field.mod_on.module == 2) // 4x2 PLQ problem
						:
							traj_field.mod_on.disk == 2 && 
							!(event_field.run >= 147752 && (traj_field.mod_on.blade == 4 || traj_field.mod_on.blade == 5 || traj_field.mod_on.blade == 6)))
							// Exclude Disk1 1x2 and 2x3 modules from measurement due to bad eta coverage
							&& !(std::abs(traj_field.mod_on.disk) == 1 && (traj_field.mod_on.panel+traj_field.mod_on.module)<4);
			
			bool goodmod_RunII = 
			traj_field.mod_on.det == 0 ? 
				// Large parts (sectors)
				!(traj_field.mod_on.layer<3 && traj_field.mod_on.module<0 && traj_field.mod_on.ladder<0 && sector == 3 && event_field.run >= 250989) && // BmO SEC3 (L1+2 is off, but L3 also excluded) - problem appeared in 3.8T
				!(traj_field.mod_on.layer>0 && traj_field.mod_on.module<0 && traj_field.mod_on.ladder<0 && sector == 7 && event_field.run >= 250989 && event_field.run<=251134) && // FED 30 (BmO SEC7) out for the duration of the Fine delay scan
				// Layer 1 modules
				//!(traj_field.mod_on.layer == 1 && traj_field.mod_on.module == 3 && traj_field.mod_on.ladder == -10) &&       // FED  8 Ch   12 - SOLVED
				!(traj_field.mod_on.layer == 1 && traj_field.mod_on.module == 2 && traj_field.mod_on.ladder == -3 && tbm == 0) &&  // FED  6 Ch    5 - Missing ROC header (firmware update)
				// Layer 2 modules
				//!(traj_field.mod_on.layer == 2 && traj_field.mod_on.module == 4 && traj_field.mod_on.ladder == -14) &&       // FED  9 Ch 4,12 - SOLVED
				!(traj_field.mod_on.layer == 2 && traj_field.mod_on.module == 3 && traj_field.mod_on.ladder == -13 && tbm == 1) && // FED  9 Ch   15 - Too many out-of-syncs (rerouting)
				!(traj_field.mod_on.layer == 2 && traj_field.mod_on.module == -1 && traj_field.mod_on.ladder == 2 && tbm == 1) &&  // FED 23 Ch   19 - Too many out-of-syncs, 6th address level (rerouting)
				!(traj_field.mod_on.layer == 2 && traj_field.mod_on.module == -1 && traj_field.mod_on.ladder == 1) &&          // FED 23 Ch   20 - Missing ROC header (firmware update)
				!(traj_field.mod_on.layer == 2 && traj_field.mod_on.module == 4 && traj_field.mod_on.ladder == -1) &&          // FED  7 Ch   20 - No HV
				!(traj_field.mod_on.layer == 2 && traj_field.mod_on.module == 4 && traj_field.mod_on.ladder == -16) &&         // FED  8 Ch    5 - No HV
				!(traj_field.mod_on.layer == 2 && traj_field.mod_on.module == 1 && traj_field.mod_on.ladder == -5 && tbm == 1) &&  // FED  5 Ch    9 - Unstable amplitude (rerouting)
				!(traj_field.mod_on.layer == 2 && traj_field.mod_on.module == 3 && traj_field.mod_on.ladder == -3 && tbm == 1) &&  // FED  6 Ch    2 - Unstable amplitude (rerouting)
				// Layer 3 modules
				//!(traj_field.mod_on.layer == 3 && traj_field.mod_on.module == 2 && traj_field.mod_on.ladder == -19) &&       // FED  9 Ch   36 - SOLVED
				!(traj_field.mod_on.layer == 3 && traj_field.mod_on.module == 1 && traj_field.mod_on.ladder == 3)              // FED  0 Ch   33 - Missing ROC header (firmware update)
			: // FPix - Exclude Disk1 1x2 and 2x3 modules from measurement due to bad eta coverage
				!(std::abs(traj_field.mod_on.disk) == 1 && (traj_field.mod_on.panel+traj_field.mod_on.module)<4);
			
			bool result = event_field.run < 246908 ? goodmod_RunI : goodmod_RunII;
			return result;
		};
	}

} // Histogram_generation

#endif