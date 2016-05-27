#ifndef CLUSTFPIX_PANEL_FILL_PARAMETER_H
#define CLUSTFPIX_PANEL_FILL_PARAMETER_H

//#include "../../SmartHistos/SmartHistos.hh"
#include "../Fill_parameter_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

class ClustFPixPanel_fill_parameter : public Fill_parameter
{
	public:
		ClustFPixPanel_fill_parameter(Ntuple_reader*& ntuple_reader_p);
		~ClustFPixPanel_fill_parameter()
		{
			if(debug)
			{
				std::cout << debug_prompt << "~ClustFPixPanel_fill_parameter()" << std::endl;
			}
		};
	private:
		int get_roc_bin_x(Ntuple_reader*& ntuple_reader_p);
};

ClustFPixPanel_fill_parameter::ClustFPixPanel_fill_parameter(Ntuple_reader*& ntuple_reader_p)
{

	this -> name = "ClustFPixPanel";
	this -> nbin = 72; 
	this -> bins = { -4.5, 4.5 };
	this -> fill = [&ntuple_reader_p, this]()
	{
		auto module_field = ntuple_reader_p -> get_module_field_ptr(); 
		const auto& det = module_field -> det;
		int clu_roc_binx = this -> get_roc_bin_x(ntuple_reader_p);
		if(clu_roc_binx == NOVAL_I || det != 1)
		{
			return static_cast<float>(NOVAL_I);
		}
		float roc_sdp = (clu_roc_binx - 36.5) / 8.0;
		return roc_sdp;
	};
	this -> axis_title = "Forward Pixel Side-Panel"; 
}

int ClustFPixPanel_fill_parameter::get_roc_bin_x(Ntuple_reader*& ntuple_reader_p)
{
// Source: Pixel Histo Maker :)
		auto module_field = ntuple_reader_p -> get_module_field_ptr(); 
		auto cluster_field = ntuple_reader_p -> get_cluster_field_ptr(); 
		const auto& det = module_field -> det;
		const auto& module = module_field -> module;
		const auto& panel = module_field -> panel;
		const auto& disk = module_field -> disk;
		const auto& x = cluster_field -> x;
		const auto& y = cluster_field -> y;
		int clu_sdp_x = ((disk > 0) ? 1 : -1) * (2 * (std::abs(disk) - 1) + panel);
// Side-Disk-Panel number - similar to module_on.module
		int clu_roc_binx = NOVAL_I;
		if(det==0)
		{
			for (int i=1;i<=8;i++)
				if (y>=((8-i)*52.0)&&y<((9-i)*52.0))
					clu_roc_binx = (module+4)*8 + i;
		} 
		else if (det==1)
		{
// Roc is left (0) or right (1) on the ROC map plot (+Z side)
			int binselx = (panel==1&&(module==1||module==4)) ? (module==1)
			: ((panel==1&&x<80.0)||(panel==2&&x>=80.0));
// Gives the Roc location inside a panel (0 to 5 on +Z side)
			int nperpan = 2 * module + panel - 1 + binselx;
			clu_roc_binx = ((disk>0) ? nperpan : 9 - nperpan) + (clu_sdp_x + 4) * 8 
			- 2 * ((abs(disk)==1) ? disk : 0);
		}
		return clu_roc_binx;
		
}

#endif