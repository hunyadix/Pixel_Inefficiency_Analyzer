#ifndef FPIX_PANEL_FILL_PARAMETER_H
#define FPIX_PANEL_FILL_PARAMETER_H

//#include "../../SmartHistos/SmartHistos.hh"
#include "../Fill_parameter_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

class FPixPanel_fill_parameter : public Fill_parameter
{
	public:
		FPixPanel_fill_parameter(Ntuple_reader*& ntuple_reader_p);
		~FPixPanel_fill_parameter()
		{
			if(debug)
			{
				std::cout << debug_prompt << "~FPixPanel_fill_parameter()" << std::endl;
			}
		};
	private:
		int get_roc_bin_x(Ntuple_reader*& ntuple_reader_p);
};

FPixPanel_fill_parameter::FPixPanel_fill_parameter(Ntuple_reader*& ntuple_reader_p)
{

	this -> name = "FPixPanel";
	this -> nbin = 72; 
	this -> bins = { -4.5, 4.5 };
	this -> fill = [&ntuple_reader_p, this]()
	{
		auto module_field = ntuple_reader_p -> get_module_field_ptr(); 
		const auto& det = module_field -> det;
		int roc_binx = this -> get_roc_bin_x(ntuple_reader_p);
		if(roc_binx == NOVAL_I || det != 1)
		{
			return static_cast<float>(NOVAL_I);
		}
		float roc_sdp = roc_binx - 36.5 / 8.0;
		return roc_sdp;
	};
	this -> axis_title = "Forward Pixel Side-Panel"; 
}

int FPixPanel_fill_parameter::get_roc_bin_x(Ntuple_reader*& ntuple_reader_p)
{
// Source: Pixel Histo Maker :)
		auto module_field = ntuple_reader_p -> get_module_field_ptr(); 
		auto traj_field   = ntuple_reader_p -> get_traj_field_ptr(); 
		const auto& det = module_field -> det;
		const auto& module = module_field -> module;
		const auto& panel = module_field -> panel;
		const auto& disk = module_field -> disk;
		const auto& lx = traj_field -> lx;
		const auto& ly = traj_field -> ly;
		int roc_binx = NOVAL_I;
// Side-Disk-Panel number - similar to module_on.module
	    int sdp_x = ((disk > 0) ? 1 : -1) * (2 * (std::abs(disk) - 1) + panel);
		if(det == 0)
		{
			roc_binx = (std::abs(ly) < 3.24) ? (int)((module + 4) * 8 - ly / 0.81 + 5) : NOVAL_I;
		}
		else if(det==1)
		{
			int binselx = NOVAL_I;
			if(panel == 1 && (module == 1 || module == 4))
			{
				if(std::abs(lx)<0.4082)
				{
					binselx = module == 1;
				}
			}
			else if(std::abs(lx) < 0.8164)
			{
				binselx = (panel == 1 && lx < 0.0) || (panel == 2 && lx >= 0.0);
			}
// Gives the Roc location inside a panel (0 to 5 on +Z side)
			int nperpan = 2 * module + panel - 1 + binselx;
			if(binselx != NOVAL_I)
			{
				int param_1 = disk > 0 ? nperpan : 9 - nperpan;
				int param_2 = std::abs(disk) == 1 ? disk : 0;
				roc_binx = param_1 + (sdp_x + 4) * 8 - 2 * param_2;
			}
		}
		return roc_binx;
		
}

#endif