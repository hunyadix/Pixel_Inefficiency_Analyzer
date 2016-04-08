#ifndef POST_LOOP_ANALYZER_H
#define POST_LOOP_ANALYZER_H

#include "Post_loop_analyzer_histogram_fetcher.hh"
#include "Post_loop_analyzer_histogram_saver.hh"
#include "../Console_actor/common_actors.hh"
#include "../ROC_position/ROC_position.hh"

#include "TCanvas.h"
#include "TPad.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TF1.h"

#include <iomanip>
#include <sstream>
#include <memory>
#include <string>
#include <utility>
#include <map>
#include <set>

extern bool debug;
extern bool debug_prompt_destructors;

class Post_loop_analyzer:
public Post_loop_analyzer_histogram_fetcher,
public Post_loop_analyzer_histogram_saver
{
	private:
		// Core
		template <typename T>
		bool                                 check_plots(std::vector<T> plot_pointers);
		double                               find_maximum_of_map(TH2D* map_p);
		bool                                 is_ROC_real(int layer_number_p, double module_number_p, double ladder_number_p);
		bool                                 check_coordinate_pair(TH2D* map_p, int layer_number_p, int module_number_p, int ladder_number_p);
		std::vector<std::pair<int, int>> get_possible_neighbours(int module_bin_p, int ladder_bin_p, double max_distance_p);
		std::vector<std::pair<int, int>> get_ROC_neighbours_array_on_map(TH2D* map_p, int layer_number_p, int module_bin_p, int ladder_bin_p, double max_distance_p);
		// Analysis plots
		void layer_efficiency_calculation_loop(TH2D* layer_total_hits_p, TH2D* layer_efficiency_p, double& eff_result_p, double& error_result_p);
		void create_hit_efficiency_layer_correspondence_plot();
		void create_relative_cluster_occupancy_plots();
		void filter_bad_ROCs_1_loop(TH2D* layer_total_hits_p, TH2D* layer_efficiency_p, TH2D* bad_ROCs_map_p, TH2D* bad_ROCs_map_second_p, 
	                                 int layer_number_p, const double& number_of_sigmas_scope_p, double& bad_ROC_eff_limit_p);
		void filter_bad_ROCs_1();
		void filter_bad_ROCs_2_loop(TH2D* layer_relative_cluster_occupancies_p, TH2D* bad_ROCs_map_p);
		void filter_bad_ROCs_2();
		void combine_filters(TH2D* simple_p, TH2D* weighted_p, TH2D* clust_occ_p, TH2D* total_hits_p, TH2D* rel_occ_p, TH2D* all_p);
		void filter_bad_ROCs_3_loop(TH2D* layer_cluster_occupancy_p, TH2D* bad_ROCs_map_p, TH1D*& distribution_p, TF1*& fit_p, int layer_number_p, double number_of_sigmas_scope_p);
		void filter_bad_ROCs_3();
		void filter_bad_ROCs_4_loop(TH2D* layer_total_hits_p, TH2D* bad_ROCs_map_p, TH1D*& distribution_p, TF1*& fit_p, int layer_number_p);
		void filter_bad_ROCs_4();
		void create_combined_filter();
		void bad_ROC_fetching_loop(int layer_number_p, std::set<ROC_position>& bad_ROC_List_p);
	public:
		Post_loop_analyzer();
		~Post_loop_analyzer();

		void                           create_tier_1_analysis_plots();
		void                           create_tier_2_analysis_plots();
		std::set<ROC_position>       get_bad_ROC_list();
};

Post_loop_analyzer::Post_loop_analyzer()
{

}

Post_loop_analyzer::~Post_loop_analyzer()
{
	if(debug_prompt_destructors)
	{
		std::cerr << debug_prompt << "~Post_loop_analyzer()" << std::endl;
	}
}

/******************************************************************************************************/
/******************************************************************************************************/
/*                                                                                                    */
/*                                           Private Methods                                          */
/*                                                                                                    */
/******************************************************************************************************/
/******************************************************************************************************/

template <typename T>
bool Post_loop_analyzer::check_plots(std::vector<T> plot_pointers)
{
	int num_bad_pointers = 0;
	for(T pointer: plot_pointers)
	{
		if(pointer == nullptr)
		{
			num_bad_pointers++;
		}
	}
	if(num_bad_pointers == 0)
	{
		return true;
	}
	else
	{
		std::cerr << error_prompt << "Number of missing plots: " << num_bad_pointers << std::endl;
	}
	return false;
}

double Post_loop_analyzer::find_maximum_of_map(TH2D* map_p)
{
	double return_value = map_p -> GetBinContent(1, 1);
	int max_module_bin_number = map_p -> GetXaxis() -> GetNbins();
	int max_ladder_bin_number = map_p -> GetYaxis() -> GetNbins();
	for(int module_ROC_bin = 1; module_ROC_bin <= max_module_bin_number; ++module_ROC_bin)
	{
		for(int ladder_ROC_bin = 1; ladder_ROC_bin <= max_ladder_bin_number; ++ladder_ROC_bin)
		{
			double current_bin_content = map_p -> GetBinContent(module_ROC_bin, ladder_ROC_bin);
			if(current_bin_content > return_value)
			{
				return_value = current_bin_content;
			}
		}
	}
	return return_value;
}

///////////////////////////
// ROC Validity Checking //
///////////////////////////

// Expects coordinates, not bins!
bool Post_loop_analyzer::is_ROC_real(int layer_number_p, double module_number_p, double ladder_number_p)
{
	// A bin contains it lower edge, but it does not contain it's upper edge
	if(-0.5 <= module_number_p && module_number_p < 0.5)
	{
		return false;
	}
	if(-1.0 <= ladder_number_p && ladder_number_p < 1.0)
	{
		return false;
	}
	if(layer_number_p == 1)
	{
		if(-10.5 <= ladder_number_p && ladder_number_p < -10.0)
		{
			return false;
		}
		if(10.0 <= ladder_number_p && ladder_number_p < 10.5)
		{
			return false;
		}
		if(ladder_number_p < -10.5 || 10.5 <= ladder_number_p)
		{
			return false;
		}
	}
	if(layer_number_p == 2)
	{
		if(-16.5 <= ladder_number_p && ladder_number_p < -16.0)
		{
			return false;
		}
		if(16.0 <= ladder_number_p && ladder_number_p < 16.5)
		{
			return false;
		}
		if(ladder_number_p < -16.5 || 16.5 <= ladder_number_p)
		{
			return false;
		}
	}
	if(layer_number_p == 3)
	{
		if(-22.5 <= ladder_number_p && ladder_number_p < -22.0)
		{
			return false;
		}
		if(22.0 <= ladder_number_p && ladder_number_p < 22.5)
		{
			return false;
		}
	}
	if(module_number_p < -4.5 || 4.5 <= module_number_p)
	{
		// std::cerr << error_prompt << "Warning: Post_loop_analyzer::is_ROC_real() is checking module that does not exists." << std::endl;
		// std::cerr << error_prompt << "Info: layer: " << layer_number_p << ";. Module: " << module_number_p << "; Ladder: " << ladder_number_p << std::endl;
		return false;
	}
	if(ladder_number_p < -22.5 || 22.5 <= ladder_number_p)
	{
		// std::cerr << error_prompt << "Warning: Post_loop_analyzer::is_ROC_real() is checking module that does not exists." << std::endl;
		// std::cerr << error_prompt << "Info: layer: " << layer_number_p << ";. Module: " << module_number_p << "; Ladder: " << ladder_number_p << std::endl;
		return false;
	}
	return true;
}

// Expects a map and bins
bool Post_loop_analyzer::check_coordinate_pair(TH2D* map_p, int layer_number_p, int module_number_p, int ladder_number_p)
{
	return this -> is_ROC_real(layer_number_p, map_p -> GetXaxis() -> GetBinCenter(module_number_p), map_p -> GetYaxis() -> GetBinCenter(ladder_number_p));
}

////////////////////////
// Finding Neighbours //
////////////////////////

std::vector<std::pair<int, int>> Post_loop_analyzer::get_possible_neighbours(int module_bin_p, int ladder_bin_p, double max_distance_p)
{
	std::vector<std::pair<int, int>> possible_neighbours;
	int min_module_bin = module_bin_p - static_cast<int>(max_distance_p) - 1;
	int max_module_bin = module_bin_p + static_cast<int>(max_distance_p) + 1;
	int min_ladder_bin = ladder_bin_p - static_cast<int>(max_distance_p) - 1;
	int max_ladder_bin = ladder_bin_p + static_cast<int>(max_distance_p) + 1;
	double max_distance_squared = max_distance_p * max_distance_p;
	for(auto selected_module_bin = min_module_bin; selected_module_bin <= max_module_bin; ++selected_module_bin)
	{
		for(auto selected_ladder_bin = min_ladder_bin; selected_ladder_bin <= max_ladder_bin; ++selected_ladder_bin)
		{
			if(selected_module_bin == module_bin_p && selected_ladder_bin == ladder_bin_p)
			{
				continue;
			}
			double module_distance_squared = (module_bin_p - selected_module_bin) * (module_bin_p - selected_module_bin);
			double ladder_distance_squared = (ladder_bin_p - selected_ladder_bin) * (ladder_bin_p - selected_ladder_bin);
			if(module_distance_squared + ladder_distance_squared < max_distance_squared)
			{
				possible_neighbours.push_back(std::make_pair(selected_module_bin, selected_ladder_bin));
			}
		}
	}
	return possible_neighbours;
}

std::vector<std::pair<int, int>> Post_loop_analyzer::get_ROC_neighbours_array_on_map(TH2D* map_p, int layer_number_p, int module_bin_p, int ladder_bin_p, double max_distance_p)
{
	std::vector<std::pair<int, int>> possible_neighbours;
	std::vector<std::pair<int, int>> real_neighbours;
	possible_neighbours = get_possible_neighbours(module_bin_p, ladder_bin_p, max_distance_p);
	for(const auto& coordinate_pair: possible_neighbours)
	{
		bool coordinates_valid = this -> check_coordinate_pair(map_p, layer_number_p, coordinate_pair.first, coordinate_pair.second);
		if(coordinates_valid)
		{
			real_neighbours.push_back(coordinate_pair);
			// std::cerr << debug_prompt << "Size bin centers (module/ladder): " << module_number << " / " << ladder_number << std::endl;
			// std::cin.get();
		}
	}
	return real_neighbours;
}

///////////////////////////////////////////////////////////////////////////
// This should not be used; Instead loop on the data to measure this out //
///////////////////////////////////////////////////////////////////////////

/*void Post_loop_analyzer::layer_efficiency_calculation_loop(TH2D* layer_total_hits_p, TH2D* layer_efficiency_p, double& eff_result_p, double& error_result_p)
{
	int max_module_bin_number = layer_total_hits_p -> GetXaxis() -> GetNbins();
	int max_ladder_bin_number = layer_total_hits_p -> GetYaxis() -> GetNbins();
	unsigned long long int denominator = 0;
	long double total_layer_efficiency = 0; 
	for(int module_ROC_bin = 1; module_ROC_bin <= max_module_bin_number; ++module_ROC_bin)
	{
		for(int ladder_ROC_bin = 1; ladder_ROC_bin <= max_ladder_bin_number; ++ladder_ROC_bin)
		{
			double current_ROC_efficiency = layer_efficiency_p -> GetBinContent(module_ROC_bin, ladder_ROC_bin);
			double current_number_of_hits = layer_total_hits_p -> GetBinContent(module_ROC_bin, ladder_ROC_bin);
			if(current_ROC_efficiency > 0.0 && current_ROC_efficiency < 1.0)
			{
				total_layer_efficiency += current_ROC_efficiency * current_number_of_hits;
			}
			if(current_number_of_hits > 0)
			{
				denominator += static_cast<unsigned long long int>(current_number_of_hits);
			}
		}
	}
	if(denominator == 0)
	{
		std::cerr << error_prompt << "Error creating hit efficiency layer correspondence plots: layer total hits histogram seems to be corrupted..." << std::endl;
		return;
	}
	total_layer_efficiency = total_layer_efficiency / static_cast<long double>(denominator);
	// std::cerr << debug_prompt << total_layer_efficiency << " " << denominator << std::endl; 
	eff_result_p = static_cast<double>(total_layer_efficiency);
	error_result_p = eff_result_p * (1.0 / static_cast<double>(denominator)); 
}*/

///////////////////////////////////////////////////////////////////////////
// This should not be used; Instead loop on the data to measure this out //
///////////////////////////////////////////////////////////////////////////

/*void Post_loop_analyzer::create_hit_efficiency_layer_correspondence_plot()
{
	if(this -> fetching_done == false)
	{
		std::cerr << error_prompt << "Cannot create analysis plots before the main histograms get fetched..." << std::endl;
		return;
	}
	this -> hit_efficiency_layer_correspondence = new TH1D("hit_efficiency_layer_correspondence", "Hit efficiency vs Layer", 3, -0.5, 2.5);
	double layer_1_hit_eff;
	double layer_2_hit_eff;
	double layer_3_hit_eff;
	double layer_1_hit_error;
	double layer_2_hit_error;
	double layer_3_hit_error;
	this -> layer_efficiency_calculation_loop(this -> total_hits_on_ROCs_map_layer_1, this -> hit_efficiency_on_ROCs_map_layer_1, layer_1_hit_eff, layer_1_hit_error);
	this -> layer_efficiency_calculation_loop(this -> total_hits_on_ROCs_map_layer_2, this -> hit_efficiency_on_ROCs_map_layer_2, layer_2_hit_eff, layer_2_hit_error);
	this -> layer_efficiency_calculation_loop(this -> total_hits_on_ROCs_map_layer_3, this -> hit_efficiency_on_ROCs_map_layer_3, layer_3_hit_eff, layer_3_hit_error);
	this -> hit_efficiency_layer_correspondence -> SetBinContent(1, layer_1_hit_eff);
	this -> hit_efficiency_layer_correspondence -> SetBinContent(2, layer_2_hit_eff);
	this -> hit_efficiency_layer_correspondence -> SetBinContent(3, layer_3_hit_eff);
	this -> hit_efficiency_layer_correspondence -> SetBinError(1, layer_1_hit_error);
	this -> hit_efficiency_layer_correspondence -> SetBinError(2, layer_2_hit_error);
	this -> hit_efficiency_layer_correspondence -> SetBinError(3, layer_3_hit_error);
}*/

void Post_loop_analyzer::create_relative_cluster_occupancy_plots()
{
	if(this -> fetching_done == false)
	{
		std::cerr << error_prompt << "Cannot create analysis plots before the main histograms get fetched..." << std::cerr;
		return;
	}
	std::function<void(TH2D*, TH2D*, TH2D*)> create_relative_cluster_occupancy = [this](TH2D* layer_total_hits_p, TH2D* layer_cluster_occupancy_p, TH2D* layer_relative_cluster_occupancy_p)
	{
		if(!check_plots(std::vector<TH2D*>({layer_total_hits_p, layer_cluster_occupancy_p, layer_relative_cluster_occupancy_p})))
		{
			std::cerr << error_prompt << "Plots were missing while creating relative cluster occupancy plots. Generation terminated." << std::endl;
			return;
		}
		int max_module_bin_number = layer_total_hits_p -> GetXaxis() -> GetNbins();
		int max_ladder_bin_number = layer_total_hits_p -> GetYaxis() -> GetNbins();
		// std::cerr << debug_prompt << "Ladder bins: " << max_ladder_bin_number << std::endl;
		// std::cerr << debug_prompt << "Module bins: " << max_module_bin_number << std::endl;
		// std::cin.get();
		for(int module_ROC_bin = 1; module_ROC_bin <= max_module_bin_number; ++module_ROC_bin)
		{
			for(int ladder_ROC_bin = 1; ladder_ROC_bin <= max_ladder_bin_number; ++ladder_ROC_bin)
			{
				double current_ROC_hits      = layer_total_hits_p        -> GetBinContent(module_ROC_bin, ladder_ROC_bin);
				double current_ROC_occupancy = layer_cluster_occupancy_p -> GetBinContent(module_ROC_bin, ladder_ROC_bin);
				if(current_ROC_occupancy == 0)
				{
					layer_relative_cluster_occupancy_p -> SetBinContent(module_ROC_bin, ladder_ROC_bin, -9999.0);
					continue;
				}
				double current_ROC_relative_efficiecy = current_ROC_hits / current_ROC_occupancy;
				if(current_ROC_occupancy == 0)
				{
					layer_relative_cluster_occupancy_p -> SetBinContent(module_ROC_bin, ladder_ROC_bin, -9999.0);
					continue;
				}
				layer_relative_cluster_occupancy_p -> SetBinContent(module_ROC_bin, ladder_ROC_bin, current_ROC_relative_efficiecy);
			}
		}
	};
	this -> relative_cluster_occupancy_on_modules_map_layer_1 = new TH2D("layer_1_relative_cluster_occupancy_ROC_resolution", "Layer_1_relative_cluster_occupancy;Ladders;Modules", 9, -4.5, 4.5, 45, -22.5, 22.5);
	this -> relative_cluster_occupancy_on_modules_map_layer_2 = new TH2D("layer_2_relative_cluster_occupancy_ROC_resolution", "Layer_2_relative_cluster_occupancy;Ladders;Modules", 9, -4.5, 4.5, 45, -22.5, 22.5);
	this -> relative_cluster_occupancy_on_modules_map_layer_3 = new TH2D("layer_3_relative_cluster_occupancy_ROC_resolution", "Layer_3_relative_cluster_occupancy;Ladders;Modules", 9, -4.5, 4.5, 45, -22.5, 22.5);
	this -> relative_cluster_occupancy_on_ROCs_map_layer_1 = new TH2D("layer_1_relative_cluster_occupancy_module_resolution", "Layer_1_relative_cluster_occupancy;Ladders;Modules", 72, -4.5, 4.5, 90, -22.5, 22.5);
	this -> relative_cluster_occupancy_on_ROCs_map_layer_2 = new TH2D("layer_2_relative_cluster_occupancy_module_resolution", "Layer_2_relative_cluster_occupancy;Ladders;Modules", 72, -4.5, 4.5, 90, -22.5, 22.5);
	this -> relative_cluster_occupancy_on_ROCs_map_layer_3 = new TH2D("layer_3_relative_cluster_occupancy_module_resolution", "Layer_3_relative_cluster_occupancy;Ladders;Modules", 72, -4.5, 4.5, 90, -22.5, 22.5);

	create_relative_cluster_occupancy(this -> total_hits_on_modules_map_layer_1, this -> clusters_on_modules_map_layer_1, this -> relative_cluster_occupancy_on_modules_map_layer_1);
	create_relative_cluster_occupancy(this -> total_hits_on_modules_map_layer_2, this -> clusters_on_modules_map_layer_2, this -> relative_cluster_occupancy_on_modules_map_layer_2);
	create_relative_cluster_occupancy(this -> total_hits_on_modules_map_layer_3, this -> clusters_on_modules_map_layer_3, this -> relative_cluster_occupancy_on_modules_map_layer_3);
	create_relative_cluster_occupancy(this -> total_hits_on_ROCs_map_layer_1, this -> clusters_on_ROCs_map_layer_1, this -> relative_cluster_occupancy_on_ROCs_map_layer_1);
	create_relative_cluster_occupancy(this -> total_hits_on_ROCs_map_layer_2, this -> clusters_on_ROCs_map_layer_2, this -> relative_cluster_occupancy_on_ROCs_map_layer_2);
	create_relative_cluster_occupancy(this -> total_hits_on_ROCs_map_layer_3, this -> clusters_on_ROCs_map_layer_3, this -> relative_cluster_occupancy_on_ROCs_map_layer_3);
}

/////////////////////////////////////////////////
// Loop to filter out the least efficient ROCs //
/////////////////////////////////////////////////

void Post_loop_analyzer::filter_bad_ROCs_1_loop(TH2D* layer_total_hits_p, TH2D* layer_efficiency_p, TH2D* bad_ROCs_map_p, TH2D* bad_ROCs_map_second_p, 
	                                               int layer_number_p, const double& number_of_sigmas_scope_p, double& bad_ROC_eff_limit_p)
{
	int max_module_bin_number = layer_total_hits_p -> GetXaxis() -> GetNbins();
	int max_ladder_bin_number = layer_total_hits_p -> GetYaxis() -> GetNbins();
	std::string layer_name = "layer_"; 
	layer_name += std::to_string(layer_number_p);
	// std::cerr << debug_prompt << "layer name: " << layer_name << std::endl;
	// std::cerr << debug_prompt << "layer number: " << layer_number_p << std::endl;
	std::string distribution_name("ROC_efficiency_distribution_simple_");
	distribution_name += layer_name;
	TH1D* roc_efficiency_distribution = new TH1D(distribution_name.c_str(), "ROC efficiency distribution, non-weighted;efficiency;num_rocs", 1000, 0.0, 1.0);
	distribution_name += "_second";
	TH1D* roc_efficiency_distribution_weighted = new TH1D(distribution_name.c_str(), "ROC efficiency distribution, weighted;efficiency;num_rocs", 1000, 0.0, 1.0);
	for(int module_ROC_bin = 1; module_ROC_bin <= max_module_bin_number; ++module_ROC_bin)
	{
		for(int ladder_ROC_bin = 1; ladder_ROC_bin <= max_ladder_bin_number; ++ladder_ROC_bin)
		{
			double current_ROC_efficiency = layer_efficiency_p -> GetBinContent(module_ROC_bin, ladder_ROC_bin);
			double current_number_of_hits = layer_total_hits_p -> GetBinContent(module_ROC_bin, ladder_ROC_bin);
			if(!(check_coordinate_pair(layer_efficiency_p, layer_number_p, module_ROC_bin, ladder_ROC_bin)))
			{
				continue;
			}
			if(current_ROC_efficiency == -1.0 || current_ROC_efficiency > 1.0 )
			{
				std::cerr << error_prompt << "Data processing error, found a pixel with -1.0 or bigger than 1.0 efficiency." << std::endl;
				continue;
			}
			if(current_ROC_efficiency != 0)
			{
				roc_efficiency_distribution -> Fill(current_ROC_efficiency - 0.0005);
				roc_efficiency_distribution_weighted -> Fill(current_ROC_efficiency - 0.0005, current_number_of_hits);
			}
		}
	}
	/*std::string fit_name("roc_efficiency_distribution_fit_");
	fit_name += layer_name;
	TF1* roc_efficiency_distribution_fit = new TF1(fit_name.c_str(), "gaus", 0.94, 1.0);
	fit_name += "_second";
	TF1* roc_efficiency_distribution_fit_weighted = new TF1(fit_name.c_str(), "gaus", 0.94, 1.0);
	roc_efficiency_distribution_fit -> FixParameter(0, 0);
	roc_efficiency_distribution_fit -> SetParameter(1, roc_efficiency_distribution -> GetMean());
	roc_efficiency_distribution -> Fit(roc_efficiency_distribution_fit, "WWM0C");
	roc_efficiency_distribution_fit_weighted -> FixParameter(0, 0);
	roc_efficiency_distribution_fit_weighted -> SetParameter(1, roc_efficiency_distribution_weighted -> GetMean());
	roc_efficiency_distribution_weighted -> Fit(roc_efficiency_distribution_fit_weighted, "WWM0C");
	TFile* test_file = new TFile("Results/debug.root", "RECREATE");
	this -> save_histogram_as_canvas(roc_efficiency_distribution);
	this -> save_histogram_as_canvas(roc_efficiency_distribution_fit);
	test_file -> Close();
	std::cerr << debug_prompt << "Mean for " << layer_name << " " << roc_efficiency_distribution_fit -> GetParameter(1) << std::endl;
	std::cerr << debug_prompt << "Sigma for " << layer_name << " " << roc_efficiency_distribution_fit -> GetParameter(2) << std::endl;
	bad_ROC_eff_limit_p = roc_efficiency_distribution_fit -> GetParameter(1) - number_of_sigmas_scope_p * (roc_efficiency_distribution_fit -> GetParameter(2));
	double bad_ROC_eff_limit_weighted = roc_efficiency_distribution_fit_weighted -> GetParameter(1) - number_of_sigmas_scope_p * (roc_efficiency_distribution_fit -> GetParameter(2));
	// std::cout << process_prompt << "Minimum efficiency of good ROCs: " << bad_ROC_eff_limit_p << std::endl;*/

	/**
	 * This should be optimally calculated using the distribution of the ROC efficiencies. TODO: fix later
	 */

	bad_ROC_eff_limit_p        = roc_efficiency_distribution          -> GetMean() - 0.02;
	double bad_ROC_eff_limit_weighted = roc_efficiency_distribution_weighted -> GetMean() - 0.02;

	for(int module_ROC_bin = 1; module_ROC_bin <= max_module_bin_number; ++module_ROC_bin)
	{
		for(int ladder_ROC_bin = 1; ladder_ROC_bin <= max_ladder_bin_number; ++ladder_ROC_bin)
		{
			double current_ROC_efficiency = layer_efficiency_p -> GetBinContent(module_ROC_bin, ladder_ROC_bin);
			if(!(check_coordinate_pair(layer_efficiency_p, layer_number_p, module_ROC_bin, ladder_ROC_bin)))
			{
				bad_ROCs_map_p        -> SetBinContent(module_ROC_bin, ladder_ROC_bin, 0.0);
				bad_ROCs_map_second_p -> SetBinContent(module_ROC_bin, ladder_ROC_bin, 0.0);
				continue;
			}
			if(current_ROC_efficiency == -1.0 || current_ROC_efficiency > 1.0 )
			{
				bad_ROCs_map_p        -> SetBinContent(module_ROC_bin, ladder_ROC_bin, 0.0);
				bad_ROCs_map_second_p -> SetBinContent(module_ROC_bin, ladder_ROC_bin, 0.0);
				std::cerr << error_prompt << "Data processing error, found a pixel with -1.0 or bigger than 1.0 efficiency." << std::endl;
				continue;
			}
			if(current_ROC_efficiency < bad_ROC_eff_limit_p)
			{
				bad_ROCs_map_p -> SetBinContent(module_ROC_bin, ladder_ROC_bin, 1.0);
			}
			else
			{
				bad_ROCs_map_p -> SetBinContent(module_ROC_bin, ladder_ROC_bin, 0.0);
			}
			if(current_ROC_efficiency < bad_ROC_eff_limit_weighted)
			{
				bad_ROCs_map_second_p -> SetBinContent(module_ROC_bin, ladder_ROC_bin, 1.0);
			}
			else
			{
				bad_ROCs_map_second_p -> SetBinContent(module_ROC_bin, ladder_ROC_bin, 0.0);
			}
		}
	}
};

////////////////////////////////////////////
// Filtering the least efficient ROCs out //
////////////////////////////////////////////

void Post_loop_analyzer::filter_bad_ROCs_1()
{
	if(this -> tier_1_analysis_done == false)
	{
		std::cerr << error_prompt << "Should not filter for ROCs before all the statistics plots are ready to use..." << std::endl;
		return;
	}
	this -> bad_ROCs_simple_efficiency_base_layer_1 = new TH2D("layer_1_bad_ROCs_simple", "Simple efficiency filtering results, layer 1;Ladders;Modules", 72, -4.5, 4.5, 90, -22.5, 22.5);
	this -> bad_ROCs_simple_efficiency_base_layer_2 = new TH2D("layer_2_bad_ROCs_simple", "Simple efficiency filtering results, layer 2;Ladders;Modules", 72, -4.5, 4.5, 90, -22.5, 22.5);
	this -> bad_ROCs_simple_efficiency_base_layer_3 = new TH2D("layer_3_bad_ROCs_simple", "Simple efficiency filtering results, layer 3;Ladders;Modules", 72, -4.5, 4.5, 90, -22.5, 22.5);
	this -> bad_ROCs_weighted_efficiency_base_layer_1 = new TH2D("layer_1_bad_ROCs_weighted", "Weighted efficiency filtering results, layer 1;Ladders;Modules", 72, -4.5, 4.5, 90, -22.5, 22.5);
	this -> bad_ROCs_weighted_efficiency_base_layer_2 = new TH2D("layer_2_bad_ROCs_weighted", "Weighted efficiency filtering results, layer 2;Ladders;Modules", 72, -4.5, 4.5, 90, -22.5, 22.5);
	this -> bad_ROCs_weighted_efficiency_base_layer_3 = new TH2D("layer_3_bad_ROCs_weighted", "Weighted efficiency filtering results, layer 3;Ladders;Modules", 72, -4.5, 4.5, 90, -22.5, 22.5);

	this -> filter_bad_ROCs_1_loop(this -> total_hits_on_ROCs_map_layer_1, this -> hit_efficiency_on_ROCs_map_layer_1, this -> bad_ROCs_simple_efficiency_base_layer_1, this -> bad_ROCs_weighted_efficiency_base_layer_1,
		                            1, this -> number_of_sigmas_scope_for_efficiencies_layer_1, this -> bad_ROC_eff_limit_layer_1);
	this -> filter_bad_ROCs_1_loop(this -> total_hits_on_ROCs_map_layer_2, this -> hit_efficiency_on_ROCs_map_layer_2, this -> bad_ROCs_simple_efficiency_base_layer_2, this -> bad_ROCs_weighted_efficiency_base_layer_2,
		                            2, this -> number_of_sigmas_scope_for_efficiencies_layer_2, this -> bad_ROC_eff_limit_layer_2);
	this -> filter_bad_ROCs_1_loop(this -> total_hits_on_ROCs_map_layer_3, this -> hit_efficiency_on_ROCs_map_layer_3, this -> bad_ROCs_simple_efficiency_base_layer_3, this -> bad_ROCs_weighted_efficiency_base_layer_3,
		                            3, this -> number_of_sigmas_scope_for_efficiencies_layer_3, this -> bad_ROC_eff_limit_layer_3);
}

///////////////////////////////////////////////////////////////////////////////////////
// Loop to filter ROCs by finding the ROCs with the worst relative cluster occupancy //
///////////////////////////////////////////////////////////////////////////////////////

void Post_loop_analyzer::filter_bad_ROCs_2_loop(TH2D* layer_relative_cluster_occupancies_p, TH2D* bad_ROCs_map_p)
{
	int max_module_bin_number = layer_relative_cluster_occupancies_p -> GetXaxis() -> GetNbins();
	int max_ladder_bin_number = layer_relative_cluster_occupancies_p -> GetYaxis() -> GetNbins();
	double rel_clust_occ_sum = 0;
	int rel_clust_occ_denom = 0;
	for(int module_ROC_bin = 1; module_ROC_bin <= max_module_bin_number; ++module_ROC_bin)
	{
		for(int ladder_ROC_bin = 1; ladder_ROC_bin <= max_ladder_bin_number; ++ladder_ROC_bin)
		{
			double current_ROC_relative_occupancy = layer_relative_cluster_occupancies_p -> GetBinContent(module_ROC_bin, ladder_ROC_bin);
			if(current_ROC_relative_occupancy > 0.0 && current_ROC_relative_occupancy <= 1.00)
			{
				rel_clust_occ_sum += current_ROC_relative_occupancy;
				++rel_clust_occ_denom;
			}
		}
	}
	double rel_clust_occ_mean = rel_clust_occ_sum / rel_clust_occ_denom;
	double minimum_limit = rel_clust_occ_mean / 2.0;
	for(int module_ROC_bin = 1; module_ROC_bin <= max_module_bin_number; ++module_ROC_bin)
	{
		for(int ladder_ROC_bin = 1; ladder_ROC_bin <= max_ladder_bin_number; ++ladder_ROC_bin)
		{
			double current_ROC_relative_occupancy = layer_relative_cluster_occupancies_p -> GetBinContent(module_ROC_bin, ladder_ROC_bin);
			if(current_ROC_relative_occupancy > 0.0 && current_ROC_relative_occupancy <= 1.00)
			{
				if(current_ROC_relative_occupancy < minimum_limit)
				{
					bad_ROCs_map_p -> SetBinContent(module_ROC_bin, ladder_ROC_bin, 1.0);
				}
				else
				{
					bad_ROCs_map_p -> SetBinContent(module_ROC_bin, ladder_ROC_bin, 0.0);
				}
			}
			else
			{
				bad_ROCs_map_p -> SetBinContent(module_ROC_bin, ladder_ROC_bin, -1.0);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////
// Filtering the ROCs by their relative cluster occupancies; not used in analysis //
////////////////////////////////////////////////////////////////////////////////////

void Post_loop_analyzer::filter_bad_ROCs_2()
{
	if(this -> tier_1_analysis_done == false)
	{
		std::cerr << error_prompt << "Should not filter for ROCs before all the statistics plots are ready to use..." << std::endl;
		return;
	}
	this -> bad_ROCs_relative_occupancy_base_layer_1 = new TH2D("layer_1_bad_ROCs_rel_occ", "Efficiency filtering by rel. occ. results, layer 1;Ladders;Modules", 72, -4.5, 4.5, 90, -22.5, 22.5);
	this -> bad_ROCs_relative_occupancy_base_layer_2 = new TH2D("layer_2_bad_ROCs_rel_occ", "Efficiency filtering by rel. occ. results, layer 2;Ladders;Modules", 72, -4.5, 4.5, 90, -22.5, 22.5);
	this -> bad_ROCs_relative_occupancy_base_layer_3 = new TH2D("layer_3_bad_ROCs_rel_occ", "Efficiency filtering by rel. occ. results, layer 3;Ladders;Modules", 72, -4.5, 4.5, 90, -22.5, 22.5);

	this -> filter_bad_ROCs_2_loop(this -> relative_cluster_occupancy_on_ROCs_map_layer_1, this -> bad_ROCs_relative_occupancy_base_layer_1);
	this -> filter_bad_ROCs_2_loop(this -> relative_cluster_occupancy_on_ROCs_map_layer_2, this -> bad_ROCs_relative_occupancy_base_layer_2);
	this -> filter_bad_ROCs_2_loop(this -> relative_cluster_occupancy_on_ROCs_map_layer_3, this -> bad_ROCs_relative_occupancy_base_layer_3);
}

/////////////////////////////////////////////////////////////////////////////////////
// Filtering out the ROCs with very bad cluster occupancies + neighbour comparison //
/////////////////////////////////////////////////////////////////////////////////////

void Post_loop_analyzer::filter_bad_ROCs_3_loop(TH2D* layer_cluster_occupancy_p, TH2D* bad_ROCs_map_p, TH1D*& distribution_p, TF1*& fit_p, int layer_number_p, double number_of_sigmas_scope_p)
{
	int max_module_bin_number = layer_cluster_occupancy_p -> GetXaxis() -> GetNbins();
	int max_ladder_bin_number = layer_cluster_occupancy_p -> GetYaxis() -> GetNbins();
	const int maximum_number_of_clusters = static_cast<int>(this -> find_maximum_of_map(layer_cluster_occupancy_p));
	std::string distribution_name("ROC_cluster_occupancy_distribution_");
	distribution_name += "layer_";
	distribution_name += std::to_string(layer_number_p);
	std::cerr << debug_prompt << "Maximum number of clusters on a read-out chip: " << maximum_number_of_clusters << std::endl;
	distribution_p = new TH1D(distribution_name.c_str(), "ROC cluster occupancy distribution, non-weighted;efficiency;num_rocs", 100, 0.0, maximum_number_of_clusters);
	for(int module_ROC_bin = 1; module_ROC_bin <= max_module_bin_number; ++module_ROC_bin)
	{
		for(int ladder_ROC_bin = 1; ladder_ROC_bin <= max_ladder_bin_number; ++ladder_ROC_bin)
		{
			double current_ROC_occupancy = layer_cluster_occupancy_p -> GetBinContent(module_ROC_bin, ladder_ROC_bin);
			if(!check_coordinate_pair(layer_cluster_occupancy_p, layer_number_p, module_ROC_bin, ladder_ROC_bin))
			{
				bad_ROCs_map_p -> SetBinContent(module_ROC_bin, ladder_ROC_bin, 0.0);
				continue;
			}
			distribution_p -> Fill(current_ROC_occupancy);
		}
	}
	std::string fit_name("roc_cluster_occupancy_distribution_fit_");
	fit_name += "layer_";
	fit_name += std::to_string(layer_number_p);
	// FIXME: The cluster ROC occupancy does not seem to follow gaussian distribution
	fit_p = new TF1(fit_name.c_str(), "gaus", 0.0, maximum_number_of_clusters);
	fit_p -> FixParameter(0, 0);
	fit_p -> SetParameter(1, maximum_number_of_clusters * .60);
	distribution_p -> Fit(fit_p, "WWM0CQ");
	// double good_ROC_min_clusters_limit = fit_p -> GetParameter(1) - number_of_sigmas_scope_p * (fit_p -> GetParameter(2));
	double good_ROC_min_clusters_limit = fit_p -> GetParameter(1) * 0.65;
	for(int module_ROC_bin = 1; module_ROC_bin <= max_module_bin_number; ++module_ROC_bin)
	{
		for(int ladder_ROC_bin = 1; ladder_ROC_bin <= max_ladder_bin_number; ++ladder_ROC_bin)
		{
			/////////////////////////////////////////////
			// Filtering based on data collection rate //
			/////////////////////////////////////////////

			bad_ROCs_map_p -> SetBinContent(module_ROC_bin, ladder_ROC_bin, 0.0);
			double current_cluster_occupancy = layer_cluster_occupancy_p -> GetBinContent(module_ROC_bin, ladder_ROC_bin); 
			if(!check_coordinate_pair(layer_cluster_occupancy_p, layer_number_p, module_ROC_bin, ladder_ROC_bin))
			{
				continue;
			}
			if(current_cluster_occupancy < good_ROC_min_clusters_limit)
			{
				bad_ROCs_map_p -> SetBinContent(module_ROC_bin, ladder_ROC_bin, 1.0);
				continue;
			}

			///////////////////////////////////
			// Filtering based on neighbours //
			///////////////////////////////////
			
			std::vector<std::pair<int, int>> neighbour_list = this -> get_ROC_neighbours_array_on_map(layer_cluster_occupancy_p, layer_number_p, module_ROC_bin, ladder_ROC_bin, 2.0);
			double neighbours_average    = 0;
			double num__valid_neighbours = 0;
			for(auto& coordinates: neighbour_list)
			{
				double current_neighbour_occupancy = layer_cluster_occupancy_p -> GetBinContent(coordinates.first, coordinates.second);
				if(current_neighbour_occupancy)
				{
					++num__valid_neighbours;
					neighbours_average += current_neighbour_occupancy;
				}
			}
			neighbours_average = neighbours_average / num__valid_neighbours;
			if(current_cluster_occupancy / neighbours_average < 0.85)
			{
				bad_ROCs_map_p -> SetBinContent(module_ROC_bin, ladder_ROC_bin, 1.0);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////
// Filtering out the ROCs with very bad cluster occupancies +  neighbour comparison //
//////////////////////////////////////////////////////////////////////////////////////

void Post_loop_analyzer::filter_bad_ROCs_3()
{
	if(this -> tier_1_analysis_done == false)
	{
		std::cerr << error_prompt << "Should not filter for ROCs before all the statistics plots are ready to use..." << std::endl;
		return;
	}
	this -> bad_ROCs_cluster_occupancy_base_layer_1 = new TH2D("layer_1_bad_ROCs_clust_occ", "Efficiency filtering by clust. occ. results, layer 1;Ladders;Modules", 72, -4.5, 4.5, 90, -22.5, 22.5);
	this -> bad_ROCs_cluster_occupancy_base_layer_2 = new TH2D("layer_2_bad_ROCs_clust_occ", "Efficiency filtering by clust. occ. results, layer 2;Ladders;Modules", 72, -4.5, 4.5, 90, -22.5, 22.5);
	this -> bad_ROCs_cluster_occupancy_base_layer_3 = new TH2D("layer_3_bad_ROCs_clust_occ", "Efficiency filtering by clust. occ. results, layer 3;Ladders;Modules", 72, -4.5, 4.5, 90, -22.5, 22.5);

	this -> filter_bad_ROCs_3_loop(this -> clusters_on_ROCs_map_layer_1, this -> bad_ROCs_cluster_occupancy_base_layer_1, this -> bad_ROC_filtering_3_distribution_layer_1, this -> bad_ROC_filtering_3_fit_layer_1, 1, this -> number_of_sigmas_scope_for_cluster_occupancy_layer_1);
	this -> filter_bad_ROCs_3_loop(this -> clusters_on_ROCs_map_layer_2, this -> bad_ROCs_cluster_occupancy_base_layer_2, this -> bad_ROC_filtering_3_distribution_layer_2, this -> bad_ROC_filtering_3_fit_layer_2, 2, this -> number_of_sigmas_scope_for_cluster_occupancy_layer_2);
	this -> filter_bad_ROCs_3_loop(this -> clusters_on_ROCs_map_layer_3, this -> bad_ROCs_cluster_occupancy_base_layer_3, this -> bad_ROC_filtering_3_distribution_layer_3, this -> bad_ROC_filtering_3_fit_layer_3, 3, this -> number_of_sigmas_scope_for_cluster_occupancy_layer_3);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Filtering out pixels with much less hits compared to the average of their neighbours //
//////////////////////////////////////////////////////////////////////////////////////////

void Post_loop_analyzer::filter_bad_ROCs_4_loop(TH2D* layer_total_hits_p, TH2D* bad_ROCs_map_p, TH1D*& distribution_p, TF1*& fit_p, int layer_number_p)
{
	int max_module_bin_number = layer_total_hits_p -> GetXaxis() -> GetNbins();
	int max_ladder_bin_number = layer_total_hits_p -> GetYaxis() -> GetNbins();
	//const int maximum_number_of_clusters = static_cast<int>(this -> find_maximum_of_map(layer_total_hits_p));
	for(int module_ROC_bin = 1; module_ROC_bin <= max_module_bin_number; ++module_ROC_bin)
	{
		for(int ladder_ROC_bin = 1; ladder_ROC_bin <= max_ladder_bin_number; ++ladder_ROC_bin)
		{
			bad_ROCs_map_p -> SetBinContent(module_ROC_bin, ladder_ROC_bin, 0.0);
			if(!check_coordinate_pair(layer_total_hits_p, layer_number_p, module_ROC_bin, ladder_ROC_bin))
			{
				continue;
			}
			std::vector<std::pair<int, int>> neighbour_list = this -> get_ROC_neighbours_array_on_map(layer_total_hits_p, layer_number_p, module_ROC_bin, ladder_ROC_bin, 5.0);
			double neighbours_average    = 0;
			double num__valid_neighbours = 0;
			double pixel_value           = layer_total_hits_p -> GetBinContent(module_ROC_bin, ladder_ROC_bin);
			for(auto& coordinates: neighbour_list)
			{
				double current_neighbour_occupancy = layer_total_hits_p -> GetBinContent(coordinates.first, coordinates.second);
				if(current_neighbour_occupancy)
				{
					++num__valid_neighbours;
					neighbours_average += current_neighbour_occupancy;
				}
			}
			neighbours_average = neighbours_average / num__valid_neighbours;
			if(pixel_value / neighbours_average < 0.75)
			{
				bad_ROCs_map_p -> SetBinContent(module_ROC_bin, ladder_ROC_bin, 1.0);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
// Filtering out pixels with much less hits compared to the average of their neighbours //
//////////////////////////////////////////////////////////////////////////////////////////

void Post_loop_analyzer::filter_bad_ROCs_4()
{
	if(this -> tier_1_analysis_done == false)
	{
		std::cerr << error_prompt << "Should not filter for ROCs before all the statistics plots are ready to use..." << std::endl;
		return;
	}
	this -> bad_ROCs_neighbour_hits_base_layer_1 = new TH2D("layer_1_bad_ROCs_clust_occ_neigh", "Efficiency filtering by comparison to neighs. results, layer 1;Ladders;Modules", 72, -4.5, 4.5, 90, -22.5, 22.5);
	this -> bad_ROCs_neighbour_hits_base_layer_2 = new TH2D("layer_2_bad_ROCs_clust_occ_neigh", "Efficiency filtering by comparison to neighs. results, layer 2;Ladders;Modules", 72, -4.5, 4.5, 90, -22.5, 22.5);
	this -> bad_ROCs_neighbour_hits_base_layer_3 = new TH2D("layer_3_bad_ROCs_clust_occ_neigh", "Efficiency filtering by comparison to neighs. results, layer 3;Ladders;Modules", 72, -4.5, 4.5, 90, -22.5, 22.5);

	this -> filter_bad_ROCs_4_loop(this -> total_hits_on_ROCs_map_layer_1, this -> bad_ROCs_neighbour_hits_base_layer_1, this -> bad_ROC_filtering_3_distribution_layer_1, this -> bad_ROC_filtering_3_fit_layer_1, 1);
	this -> filter_bad_ROCs_4_loop(this -> total_hits_on_ROCs_map_layer_2, this -> bad_ROCs_neighbour_hits_base_layer_2, this -> bad_ROC_filtering_3_distribution_layer_2, this -> bad_ROC_filtering_3_fit_layer_2, 2);
	this -> filter_bad_ROCs_4_loop(this -> total_hits_on_ROCs_map_layer_3, this -> bad_ROCs_neighbour_hits_base_layer_3, this -> bad_ROC_filtering_3_distribution_layer_3, this -> bad_ROC_filtering_3_fit_layer_3, 3);
}

/////////////////////////////////////
// Loop to combine all the filters //
/////////////////////////////////////

void Post_loop_analyzer::combine_filters(TH2D* simple_p, TH2D* weighted_p, TH2D* clust_occ_p, TH2D* total_hits_p, TH2D* rel_occ_p, TH2D* all_p)
{
	int max_module_bin_number = simple_p -> GetXaxis() -> GetNbins();
	int max_ladder_bin_number = simple_p -> GetYaxis() -> GetNbins();
	for(int module_ROC_bin = 1; module_ROC_bin <= max_module_bin_number; ++module_ROC_bin)
	{
		for(int ladder_ROC_bin = 1; ladder_ROC_bin <= max_ladder_bin_number; ++ladder_ROC_bin)
		{
			double current_simple           = simple_p     -> GetBinContent(module_ROC_bin, ladder_ROC_bin);
			double current_weighted         = weighted_p   -> GetBinContent(module_ROC_bin, ladder_ROC_bin);
			double current_clust_occ_based  = clust_occ_p  -> GetBinContent(module_ROC_bin, ladder_ROC_bin);
			double current_total_hits_based = total_hits_p -> GetBinContent(module_ROC_bin, ladder_ROC_bin);
			// Rel occ filtering is excluded.
			// double current_rel_occ  = rel_occ_p -> GetBinContent(module_ROC_bin, ladder_ROC_bin);
			if(
				current_simple           == 1.0 ||
				current_weighted         == 1.0 ||
				current_clust_occ_based  == 1.0 || 
				current_total_hits_based == 1.0 ||
				// current_rel_occ          == 1.0 || 
				0)
			{
				all_p -> SetBinContent(module_ROC_bin, ladder_ROC_bin, 1.0);
			}
			else
			{
				all_p -> SetBinContent(module_ROC_bin, ladder_ROC_bin, 0.0);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Combining all the bad ROC filters that are relevant for the analysis //
//////////////////////////////////////////////////////////////////////////

void Post_loop_analyzer::create_combined_filter()
{
	if(this -> tier_1_analysis_done == false)
	{
		std::cerr << error_prompt << "Should not filter for ROCs before all the statistics plots are ready to use..." << std::endl;
		return;
	}

	this -> bad_ROCs_all_layer_1 = new TH2D("layer_1_bad_ROCs_combined", "All efficiency filtering combined, layer 1;Ladders;Modules", 72, -4.5, 4.5, 90, -22.5, 22.5);
	this -> bad_ROCs_all_layer_2 = new TH2D("layer_2_bad_ROCs_combined", "All efficiency filtering combined, layer 2;Ladders;Modules", 72, -4.5, 4.5, 90, -22.5, 22.5);
	this -> bad_ROCs_all_layer_3 = new TH2D("layer_3_bad_ROCs_combined", "All efficiency filtering combined, layer 3;Ladders;Modules", 72, -4.5, 4.5, 90, -22.5, 22.5);

	this -> combine_filters(this -> bad_ROCs_simple_efficiency_base_layer_1,  this -> bad_ROCs_weighted_efficiency_base_layer_1, this -> bad_ROCs_cluster_occupancy_base_layer_1,
		                     this -> bad_ROCs_neighbour_hits_base_layer_1, this -> bad_ROCs_relative_occupancy_base_layer_1, this -> bad_ROCs_all_layer_1);
	this -> combine_filters(this -> bad_ROCs_simple_efficiency_base_layer_2,  this -> bad_ROCs_weighted_efficiency_base_layer_2, this -> bad_ROCs_relative_occupancy_base_layer_2,
		                     this -> bad_ROCs_neighbour_hits_base_layer_2, this -> bad_ROCs_relative_occupancy_base_layer_2, this -> bad_ROCs_all_layer_2);
	this -> combine_filters(this -> bad_ROCs_simple_efficiency_base_layer_3,  this -> bad_ROCs_weighted_efficiency_base_layer_3, this -> bad_ROCs_cluster_occupancy_base_layer_3,
		                     this -> bad_ROCs_neighbour_hits_base_layer_3, this -> bad_ROCs_relative_occupancy_base_layer_3, this -> bad_ROCs_all_layer_3);
}

void Post_loop_analyzer::bad_ROC_fetching_loop(int layer_number_p, std::set<ROC_position>& bad_ROC_List_p)
{
	TH2D* bad_ROC_histogram = nullptr;
	if(layer_number_p == 1)
	{
		bad_ROC_histogram = this -> bad_ROCs_all_layer_1;
	}
	if(layer_number_p == 2)
	{
		bad_ROC_histogram = this -> bad_ROCs_all_layer_2;
	}
	if(layer_number_p == 3)
	{
		bad_ROC_histogram = this -> bad_ROCs_all_layer_3;
	}
	int max_module_bin_number = bad_ROC_histogram -> GetXaxis() -> GetNbins();
	int max_ladder_bin_number = bad_ROC_histogram -> GetYaxis() -> GetNbins();
	for(int module_ROC_bin = 1; module_ROC_bin <= max_module_bin_number; ++module_ROC_bin)
	{
		for(int ladder_ROC_bin = 1; ladder_ROC_bin <= max_ladder_bin_number; ++ladder_ROC_bin)
		{
			if(bad_ROC_histogram -> GetBinContent(module_ROC_bin, ladder_ROC_bin))
			{
				// double module_bin_center = bad_ROC_histogram -> GetXaxis() -> GetBinCenter(module_ROC_bin);
				// double ladder_bin_center = bad_ROC_histogram -> GetYaxis() -> GetBinCenter(ladder_ROC_bin);
				ROC_position list_item(layer_number_p, module_ROC_bin, ladder_ROC_bin);
				bad_ROC_List_p.insert(list_item);
			}
		}
	}
}

/******************************************************************************************************/
/******************************************************************************************************/
/*                                                                                                    */
/*                                           Public Methods                                           */
/*                                                                                                    */
/******************************************************************************************************/
/******************************************************************************************************/

void Post_loop_analyzer::create_tier_1_analysis_plots()
{
	if(this -> fetching_done == false)
	{
		std::cerr << error_prompt << "Cannot create analysis plots before the main histograms get fetched..." << std::cerr;
		return;
	}

	// this -> create_hit_efficiency_layer_correspondence_plot();
	this -> create_relative_cluster_occupancy_plots();

	this -> tier_1_analysis_done = true;
}

void Post_loop_analyzer::create_tier_2_analysis_plots()
{
	if(this -> tier_1_analysis_done == false)
	{
		std::cerr << error_prompt << "Cannot create analysis plots before the main histograms get fetched..." << std::cerr;
		return;
	}

	this -> filter_bad_ROCs_1();
	std::cout << process_prompt << "Filtering: \"bad efficiency\" done..." << std::endl;
	this -> filter_bad_ROCs_2();
	std::cout << process_prompt << "Filtering: \"bad cluster occupancy\" done..." << std::endl;
	this -> filter_bad_ROCs_3();
	std::cout << process_prompt << "Filtering: \"bad relative cluster occupancy\" done..." << std::endl;
	this -> filter_bad_ROCs_4();
	std::cout << process_prompt << "Filtering: \"bad hit reconstruction\" done..." << std::endl;
	this -> create_combined_filter();
	std::cout << process_prompt << "Filtering: \"combined filter\" done..." << std::endl;

	this -> tier_2_analysis_done = true;
}

std::set<ROC_position> Post_loop_analyzer::get_bad_ROC_list()
{
	if(this -> tier_2_analysis_done == false)
	{
		std::cerr << error_prompt << "get_bad_ROC_list() is unable to provide a bad ROC list before the filters are generated..." << std::endl;
		exit(-1);
	}
	std::set<ROC_position> bad_ROC_list;
	bad_ROC_fetching_loop(1, bad_ROC_list);
	bad_ROC_fetching_loop(2, bad_ROC_list);
	bad_ROC_fetching_loop(3, bad_ROC_list);
	return bad_ROC_list;
}

#endif