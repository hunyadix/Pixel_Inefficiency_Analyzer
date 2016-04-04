#ifndef ROC_GEOMETRY_FUNCTIONS_H
#define ROC_GEOMETRY_FUNCTIONS_H

#include "./../Tree_structures/Module_structure.hh"
#include "./../Tree_structures/Traj_measurement.hh"
#include "../Console_actor/common_actors.hh"

namespace Barrel_ROC
{
	double convert_module_bin_to_fill_value(int module_bin_p)
	{
		return (module_bin_p - 36.5) / 8.0;
	}
	double convert_ladder_bin_to_fill_value(int ladder_bin_p, const ModuleData& module_data_p )
	{
		return (ladder_bin_p - 1.5) / 2.0 - (module_data_p.layer * 6 + 4);
	}
	int get_module_bin_from_local_y(const double local_y_p, const ModuleData& module_data_p)
	{
		if(local_y_p != NOVAL_F)
		{
			if(module_data_p.det == 0)
			{
				int is_on_roc_flag = std::abs(local_y_p) < 3.24;
				if(is_on_roc_flag)
				{
					int cluster_x = (int)((module_data_p.module + 4) * 8 - local_y_p / 0.81 + 5);
					return cluster_x;
				}
				{
					//std::cerr << error_prompt << "Barrel_ROC coordinate is outside of ROCs. (Permissive)" << std::endl;
				}
			}
			else
			{
					std::cerr << error_prompt << "Barrel_ROC coordinate calculation called to be performed on non-barrel ROC. (Permissive)" << std::endl;
			}
		}
		else
		{
			std::cerr << "Reading Barrel_ROC coordinates failed. (Permissive)" << std::endl;
		}
		return NOVAL_I;
	}
	double get_module_fill_value_from_local_y(const double local_y_p, const ModuleData& module_data_p)
	{
		int module_bin_num = get_module_bin_from_local_y(local_y_p, module_data_p);
		double roc_module  = convert_module_bin_to_fill_value(module_bin_num);
		return roc_module;
	}
	int get_ladder_bin_from_local_x(const double local_x_p, const ModuleData& module_data_p)
	{
		if(local_x_p != NOVAL_F)
		{
			int cluster_y = NOVAL_I;
			if(module_data_p.det == 0) 
			{
				int is_on_roc_flag = (module_data_p.half == 0 && std::abs(local_x_p) < 0.8164) | (module_data_p.half == 1 && std::abs(local_x_p) < 0.4082);
				if(is_on_roc_flag)
				{
					// Roc is in bottom (0) or top bin (1) inside a ladder on th ROC map plot
					int second_half_and_top_flag = (module_data_p.half == 1 && ((module_data_p.ladder < 0 &&   module_data_p.ladder % 2 == 0) || module_data_p.ladder % 2 == 1));
					int first_half_top_flag      = (module_data_p.half == 0 && ((module_data_p.ladder < 0 && ((module_data_p.ladder % 2 == -1 && local_x_p < 0.0) || 
					                                                            (module_data_p.ladder % 2 == 0 && local_x_p > 0.0))) ||
					                                                            (module_data_p.ladder > 0 && ((module_data_p.ladder % 2 == 0  && local_x_p < 0.0) || 
					                                                            (module_data_p.ladder % 2 == 1 && local_x_p > 0.0)))));
					int binsely = first_half_top_flag || second_half_and_top_flag;
					cluster_y = ((module_data_p.layer * 6 + module_data_p.ladder + 4) * 2 + 1 + binsely);
					return cluster_y;
				}
				else
				{
					//std::cerr << error_prompt << "Barrel_ROC coordinate is outside of ROCs. (Permissive)" << std::endl;
					return NOVAL_I;
				}
			}
			else
			{
				std::cerr << error_prompt << "Barrel_ROC coordinate calculation called to be performed on non-barrel ROC. (Permissive)" << std::endl;
			}
		}
		else
		{
			std::cerr << "Reading Barrel_ROC coordinates failed." << std::endl;
		}
		return NOVAL_I;
	}
	double get_ladder_fill_value_from_local_x(const double local_x_p, const ModuleData& module_data_p)
	{
		int ladder_bin_num = get_ladder_bin_from_local_x(local_x_p, module_data_p);
		double roc_ladder  = convert_ladder_bin_to_fill_value(ladder_bin_num, module_data_p);
		return roc_ladder;
	}
	int get_module_bin_from_cluster_xy(const int cluster_x_p, const int cluster_y_p, const ModuleData& module_data_p)
	{
		int module_coordinate = NOVAL_I;
		if(module_data_p.det == 0) 
		{
			for(int i = 1; i <= 8; i++)
			{
				if(cluster_y_p >= ((8-i) * 52.0) && cluster_y_p < ((9 - i) * 52.0))
				{
					module_coordinate = (module_data_p.module + 4) * 8 + i;
				}
			}
		} 
		else
		{
			int clu_sdpx = ((module_data_p.disk>0) ? 1 : -1) * (2 * (std::abs(module_data_p.disk) - 1) + module_data_p.panel);
			if(module_data_p.det == 1)
			{
				std::cerr << error_prompt << "Barrel_ROC coordinate calculation called to be performed on non-barrel ROC. (Permissive)" << std::endl;
				// Roc is left (0) or right (1) on the ROC map plot (+Z side)
				int binselx;
				if(((module_data_p.panel == 1) & ((module_data_p.module == 1) | (module_data_p.module == 4))))
				{
					binselx = (module_data_p.module == 1);
				}
				else
				{
					binselx = (((module_data_p.panel == 1) & (cluster_x_p < 80.0)) || ((module_data_p.panel == 2) & (cluster_x_p >= 80.0)));
				}
				// Gives the Roc location inside a panel (0 to 5 on +Z side)
				int nperpan = 2 * module_data_p.module + module_data_p.panel - 1 + binselx;
				module_coordinate = ((module_data_p.disk > 0) ? nperpan : 9 - nperpan) + (clu_sdpx + 4) * 8 - 2 * ((std::abs(module_data_p.disk) == 1) ? module_data_p.disk : 0);
			}
			else
			{
				std::cerr << error_prompt << "Invalid Barrel_ROC module det parameter. (Permissive)" << std::endl;
			}
		}
		return module_coordinate;
	}
	double get_module_fill_value_from_cluster_xy(const int cluster_x_p, const int cluster_y_p, const ModuleData& module_data_p)
	{
		int module_bin_num = get_module_bin_from_cluster_xy(cluster_x_p, cluster_y_p, module_data_p);
		double roc_module  = convert_module_bin_to_fill_value(module_bin_num);
		return roc_module;
	}
	int get_ladder_bin_from_cluster_xy(const int cluster_x_p, const int cluster_y_p, const ModuleData& module_data_p)
	{
		int ladder_coordinate = NOVAL_I; 
		if(module_data_p.det == 0)
		{
			// Roc is in bottom (0) or top bin (1) inside a ladder on th ROC map plot
			int binsely = ((module_data_p.half == 1 && ((module_data_p.ladder < 0 && module_data_p.ladder % 2 == 0) || module_data_p.ladder % 2 == 1)) ||
			               (module_data_p.half == 0 && ((module_data_p.ladder < 0  && ((module_data_p.ladder % 2 == -1 && cluster_x_p < 80.0) ||
			               (module_data_p.ladder % 2 == 0 && cluster_x_p >= 80.0))) ||
			               (module_data_p.ladder > 0  && ((module_data_p.ladder % 2 == 0 && cluster_x_p < 80.0) ||
			               (module_data_p.ladder % 2 == 1 && cluster_x_p >= 80.0))))));
			ladder_coordinate = (module_data_p.layer * 6 + module_data_p.ladder + 4) * 2 + 1 + binsely;
		} 
		else
		{
			std::cerr << error_prompt << "Barrel_ROC coordinate calculation called to be performed on non-barrel ROC. (Permissive)" << std::endl;
			if (module_data_p.det == 1) 
			{
				// Gives the number of ROCs along ly
				int nrocly = module_data_p.module + module_data_p.panel;
				for(int i=0; i<nrocly; i++) 
				{
					// ROC number = nrocly - 1 - i for + LX and nrocly + i for -LX.
					int j = (module_data_p.disk<0) ? i : nrocly - 1 - i;
					if (cluster_y_p>=(j * 52.0) && cluster_y_p<((j+1) * 52.0))
					{
						ladder_coordinate = 6 - nrocly + 2 * i + ((module_data_p.blade>0) ? module_data_p.blade-1 : module_data_p.blade + 12) * 12 + 1;
					}
				}
			}
		}
		return ladder_coordinate;
	}
	double get_ladder_fill_value_from_cluster_xy(const int cluster_x_p, const int cluster_y_p, const ModuleData& module_data_p)
	{
		int ladder_bin_num = get_ladder_bin_from_cluster_xy(cluster_x_p, cluster_y_p, module_data_p);
		double roc_ladder  = convert_ladder_bin_to_fill_value(ladder_bin_num, module_data_p);
		return roc_ladder;
	}
}

#endif