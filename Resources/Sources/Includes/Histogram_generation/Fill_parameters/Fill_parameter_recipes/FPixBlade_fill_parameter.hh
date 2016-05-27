#ifndef FPIX_BLADE_FILL_PARAMETER_H
#define FPIX_BLADE_FILL_PARAMETER_H

//#include "../../SmartHistos/SmartHistos.hh"
#include "../Fill_parameter_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

class FPixBlade_fill_parameter : public Fill_parameter
{
	public:
		FPixBlade_fill_parameter(Ntuple_reader*& ntuple_reader_p);
		~FPixBlade_fill_parameter()
		{
			if(debug)
			{
				std::cout << debug_prompt << "~FPixBlade_fill_parameter()" << std::endl;
			}
		};
	private:
		int get_roc_bin_y(Ntuple_reader*& ntuple_reader_p);
};

FPixBlade_fill_parameter::FPixBlade_fill_parameter(Ntuple_reader*& ntuple_reader_p)
{
	this -> name = "FPixBlade";
	this -> nbin = 72; 
	this -> bins = { -12.5, 12.5 };
	this -> fill = [&ntuple_reader_p, this]()
	{
		auto module_field = ntuple_reader_p -> get_module_field_ptr(); 
		const auto& det = module_field -> det;
		const auto& module = module_field -> module;
		const auto& panel = module_field -> panel;
		const auto& blade = module_field -> blade;
		int roc_biny = this -> get_roc_bin_y(ntuple_reader_p);
		if(det != 1 || roc_biny == NOVAL_I)
		{
			return static_cast<float>(NOVAL_I);
		}
		float roc_blade = (roc_biny + module + panel - 7) / 12.0 - ( blade < 0 ) * 13 + 0.54167; 
		return roc_blade;
	};
	this -> axis_title = "Forward Pixel Blade"; 
}

int FPixBlade_fill_parameter::get_roc_bin_y(Ntuple_reader*& ntuple_reader_p)
{
// Source: Pixel Histo Maker :)
	auto module_field = ntuple_reader_p -> get_module_field_ptr(); 
	auto traj_field   = ntuple_reader_p -> get_traj_field_ptr(); 
	const auto& det = module_field -> det;
	const auto& layer = module_field -> layer;
	const auto& module = module_field -> module;
	const auto& ladder = module_field -> ladder;
	const auto& panel = module_field -> panel;
	const auto& blade = module_field -> blade;
	const auto& half = module_field -> half;
	const auto& disk = module_field -> disk;
	const auto& lx = traj_field -> lx;
	const auto& ly = traj_field -> ly;
	int roc_biny = NOVAL_I;
	if(det==0) 
	{
	  if ((half==0&&fabs(lx)<0.8164)||(half==1&&fabs(lx)<0.4082)) 
	  {
// Roc is in bottom (0) or top bin (1) inside a ladder on th ROC map plot
	    int binsely = ((half==1&&((ladder<0&&ladder%2==0)||ladder%2==1))
		       ||(half==0&&((ladder<0 &&((ladder%2==-1&&lx<0.0)
								   ||(ladder%2==0&&lx>0.0)))
					     ||(ladder>0 &&((ladder%2==0&&lx<0.0)
								     ||(ladder%2==1&&lx>0.0))))));
		roc_biny = (layer * 6 + ladder + 4) * 2 + 1 + binsely;
	  }
	}
	else if(det==1)
	{
// Gives the number of ROCs along ly
	  int nrocly = module + panel;
	  if (fabs(ly)<(nrocly*0.405)) 
	  {
			int rocy = (int)((ly+(nrocly%2)*0.405)/0.81+nrocly/2);
			roc_biny = 7 - nrocly + 2 * ((disk<0) ? rocy : nrocly - 1 - rocy) 
	  + (blade-1 + (blade<0)*13)*12;
	  }
	}
	return roc_biny;
}

#endif