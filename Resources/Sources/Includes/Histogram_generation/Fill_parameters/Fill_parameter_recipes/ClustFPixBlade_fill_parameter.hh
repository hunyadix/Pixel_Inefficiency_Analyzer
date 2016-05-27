#ifndef CLUSTFPIX_BLADE_FILL_PARAMETER_H
#define CLUSTFPIX_BLADE_FILL_PARAMETER_H

//#include "../../SmartHistos/SmartHistos.hh"
#include "../Fill_parameter_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

class ClustFPixBlade_fill_parameter : public Fill_parameter
{
	public:
		ClustFPixBlade_fill_parameter(Ntuple_reader*& ntuple_reader_p);
		~ClustFPixBlade_fill_parameter()
		{
			if(debug)
			{
				std::cout << debug_prompt << "~ClustFPixBlade_fill_parameter()" << std::endl;
			}
		};
	private:
		int get_roc_bin_y(Ntuple_reader*& ntuple_reader_p);
};

ClustFPixBlade_fill_parameter::ClustFPixBlade_fill_parameter(Ntuple_reader*& ntuple_reader_p)
{
	this -> name = "ClustFPixBlade";
	this -> nbin = 72; 
	this -> bins = { -12.5, 12.5 };
	this -> fill = [&ntuple_reader_p, this]()
	{
		auto module_field = ntuple_reader_p -> get_module_field_ptr(); 
		const auto& det = module_field -> det;
		const auto& module = module_field -> module;
		const auto& panel = module_field -> panel;
		const auto& blade = module_field -> blade;
		int clu_roc_biny = this -> get_roc_bin_y(ntuple_reader_p);
		if(clu_roc_biny == NOVAL_I || det !=1)
		{
			return static_cast<float>(NOVAL_I);
		}
		float roc_blade = (clu_roc_biny+module + panel - 7) / 12.0 - (blade < 0) * 13 + 0.54167;
		return roc_blade;
	};
	this -> axis_title = "Forward Pixel Blade"; 
}

int ClustFPixBlade_fill_parameter::get_roc_bin_y(Ntuple_reader*& ntuple_reader_p)
{
// Source: Pixel Histo Maker :)
	auto module_field = ntuple_reader_p -> get_module_field_ptr(); 
	auto cluster_field   = ntuple_reader_p -> get_cluster_field_ptr(); 
	const auto& det = module_field -> det;
	const auto& layer = module_field -> layer;
	const auto& module = module_field -> module;
	const auto& ladder = module_field -> ladder;
	const auto& panel = module_field -> panel;
	const auto& blade = module_field -> blade;
	const auto& half = module_field -> half;
	const auto& disk = module_field -> disk;
	const auto& x = cluster_field -> x;
	const auto& y = cluster_field -> y;
    int clu_roc_biny = NOVAL_I; 
    if(det==0)
    {
      // Roc is in bottom (0) or top bin (1) inside a ladder on th ROC map plot
      int binsely = ((half==1&&((ladder<0&&ladder%2==0)||ladder%2==1))
      	       ||(half==0&&((ladder<0 &&((ladder%2==-1&&x<80.0)
      								   ||(ladder%2==0&&x>=80.0)))
      					 ||(ladder>0 &&((ladder%2==0&&x<80.0)
      								     ||(ladder%2==1&&x>=80.0))))));
      clu_roc_biny = (layer * 6 + ladder + 4) * 2 + 1 + binsely;
    }
    else if (det==1)
    {
      // Gives the number of ROCs along ly
      int nrocly = module + panel;
      for (int i=0; i<nrocly; i++)
      {
        // ROC number = nrocly - 1 - i for + LX and nrocly + i for -LX.
        int j = (disk<0) ? i : nrocly - 1 - i;
        if (y>=(j*52.0)&&y<((j+1)*52.0))
          clu_roc_biny = 6 - nrocly + 2 * i + ((blade>0) ? blade-1 : blade + 12)*12 + 1;
      }
    }
    return clu_roc_biny;
}

#endif