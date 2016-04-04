#ifndef CUT_LX_FID_H
#define CUT_LX_FID_H

#include "../SmartHisto_cut.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

namespace Histogram_generation
{

	class Cut_lx_fid : public Cut
	{
		public:
			Cut_lx_fid(Ntuple_reader*& ntuple_reader_p);
			~Cut_lx_fid()
			{
				if(debug)
				{
					std::cout << debug_prompt << "~Cut_lx_fid()" << std::endl;
				}
			};
	};

	Cut_lx_fid::Cut_lx_fid(Ntuple_reader*& ntuple_reader_p)
	{
		this -> cut_function = [&ntuple_reader_p]()
		{
			TrajMeasurement traj_field = *(ntuple_reader_p -> get_traj_field_ptr());
			bool edge_lx = traj_field.mod_on.det==0 ? /* BPix */ (traj_field.mod_on.half==0 ? std::abs(traj_field.lx)<0.65: traj_field.mod_on.half==1 && traj_field.lx>-0.3 && traj_field.lx<0.25): traj_field.mod_on.det==1 &&(/* FPix */traj_field.mod_on.panel==1 ?(/* Panel 1 */traj_field.mod_on.module== 1 ? traj_field.lx>-0.15 && traj_field.lx<0.3: traj_field.mod_on.module==2 ? (std::abs(traj_field.mod_on.disk)==1  ? traj_field.lx>-0.55 && traj_field.lx<0.6 :std::abs(traj_field.mod_on.disk)==2 && traj_field.lx>-0.6 && traj_field.lx<0.3): traj_field.mod_on.module==3 ? (std::abs(traj_field.mod_on.disk)==1  ? traj_field.lx> 0.3 && traj_field.lx<0.6 :std::abs(traj_field.mod_on.disk)==2 && traj_field.lx>-0.6 && traj_field.lx<0.5): traj_field.mod_on.module==4 && traj_field.lx>-0.3 && traj_field.lx<0.15): traj_field.mod_on.panel==2 &&(/* Panel 2 */traj_field.mod_on.module==1 ? traj_field.lx>-0.55 && traj_field.lx<0.6: traj_field.mod_on.module==2 ? (std::abs(traj_field.mod_on.disk)==1  ? std::abs(traj_field.lx)<0.55 :std::abs(traj_field.mod_on.disk)==2 && traj_field.lx>-0.6 && traj_field.lx<0.55): traj_field.mod_on.module==3 && std::abs(traj_field.lx)<0.55));/* ROC Edge cut - LX */int rocedge_lx = !((traj_field.mod_on.half==0||(traj_field.mod_on.det==1&&!(traj_field.mod_on.panel==1&&(traj_field.mod_on.module==1||traj_field.mod_on.module==4))))&& std::abs(traj_field.lx)<0.06);
			bool result = edge_lx && rocedge_lx;
			return result;
		};
	}

} // Histogram_generation

#endif