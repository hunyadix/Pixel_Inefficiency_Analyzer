#ifndef CUT_LY_FID_H
#define CUT_LY_FID_H

#include "../SmartHisto_cut.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

namespace Histogram_generation
{

	class Cut_ly_fid : public Cut
	{
		public:
			Cut_ly_fid(Ntuple_reader*& ntuple_reader_p);
			~Cut_ly_fid()
			{
				if(debug)
				{
					std::cout << debug_prompt << "~Cut_ly_fid()" << std::endl;
				}
			};
	};

	Cut_ly_fid::Cut_ly_fid(Ntuple_reader*& ntuple_reader_p)
	{
		this -> cut_function = [&ntuple_reader_p]()
		{
			TrajMeasurement traj_field = *(ntuple_reader_p -> get_traj_field_ptr());
			bool edge_ly = traj_field.mod_on.det==0 ? std::abs(traj_field.ly)<3.1 /* BPix */: traj_field.mod_on.det==1 &&(/* FPix */traj_field.mod_on.panel==1 ?(/* Panel 1 */traj_field.mod_on.module==1 ? std::abs(traj_field.ly)<0.7: traj_field.mod_on.module==2 ? std::abs(traj_field.ly)<1.1 && !(traj_field.mod_on.disk==-1 && traj_field.lx>0.25 && traj_field.ly> 0.75)&& !(traj_field.mod_on.disk== 1 && traj_field.lx>0.25 && traj_field.ly<-0.75): traj_field.mod_on.module==3 ? std::abs(traj_field.ly)<1.5&& !(traj_field.mod_on.disk==-1 && traj_field.ly > 1.1)&& !(traj_field.mod_on.disk== 1 && traj_field.ly <-1.0)&& !(traj_field.mod_on.disk==-2 && traj_field.lx>0.15 && traj_field.ly> 1.1)&& !(traj_field.mod_on.disk== 2 && traj_field.lx>0.15 && traj_field.ly<-1.1): traj_field.mod_on.module==4 && std::abs(traj_field.ly)<1.9&& !(traj_field.mod_on.disk==-2 && traj_field.ly> 1.5)&& !(traj_field.mod_on.disk== 2 && traj_field.ly<-1.5)): traj_field.mod_on.panel==2 &&(/* Panel 2 */traj_field.mod_on.module==1 ? std::abs(traj_field.ly)<0.7: traj_field.mod_on.module==2 ? std::abs(traj_field.ly)<1.1: traj_field.mod_on.module==3 && std::abs(traj_field.ly)<1.6&& !(traj_field.mod_on.disk>0 && traj_field.ly>1.5)&& !(traj_field.mod_on.disk<0 && traj_field.ly<-1.5)));/* ROC Edge cut - LY */float ly_mod = std::abs(traj_field.ly) + (traj_field.mod_on.det==1&&(traj_field.mod_on.panel+traj_field.mod_on.module)%2==1)*0.405;float d_rocedge = std::abs(std::abs(fmod(ly_mod,0.81)-0.405)-0.405);bool rocedge_ly = d_rocedge > 0.06;
			
			bool result = edge_ly && rocedge_ly;
			return result;
		};
	}

} // Histogram_generation

#endif