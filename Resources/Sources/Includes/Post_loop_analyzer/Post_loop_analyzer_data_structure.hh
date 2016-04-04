#ifndef POST_LOOP_ANALYZER_DATA_STRUCTURE_HH
#define POST_LOOP_ANALYZER_DATA_STRUCTURE_HH

#include "../Console_actor/common_actors.hh"

#include "TCanvas.h"
#include "TPad.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TF1.h"

#include <iomanip>
#include <sstream>
#include <memory>

extern bool debug;
extern bool debug_prompt_destructors;

struct Post_loop_analyzer_data_structure
{
		//////////////////////
		// Workflow Control //
		//////////////////////
		
		bool fetching_done = false;
		bool tier_1_analysis_done = false;
		bool tier_2_analysis_done = false;

		/******************************************************************************************************/
		/******************************************************************************************************/
		/*                                                                                                    */
		/*                                           Analysis parameterset                                    */
		/*                                                                                                    */
		/******************************************************************************************************/
		/******************************************************************************************************/

		///////////////////////////////////////////////////
		// Bad ROC filtering based on minimum efficiency //
		///////////////////////////////////////////////////
		
		double number_of_sigmas_scope_for_efficiencies_layer_1 = 3;
		double number_of_sigmas_scope_for_efficiencies_layer_2 = 3;
		double number_of_sigmas_scope_for_efficiencies_layer_3 = 3;
		double bad_ROC_eff_limit_layer_1;
		double bad_ROC_eff_limit_layer_2;
		double bad_ROC_eff_limit_layer_3;

		//////////////////////////////////////////////////
		// Bad ROC filtering based on cluster occupancy //
		//////////////////////////////////////////////////

		double number_of_sigmas_scope_for_cluster_occupancy_layer_1 = 3;
		double number_of_sigmas_scope_for_cluster_occupancy_layer_2 = 3;
		double number_of_sigmas_scope_for_cluster_occupancy_layer_3 = 3;

		/******************************************************************************************************/
		/******************************************************************************************************/
		/*                                                                                                    */
		/*                                           1D Histograms                                            */
		/*                                                                                                    */
		/******************************************************************************************************/
		/******************************************************************************************************/

		////////////////////////////////////////////
		// Cluster number geometry correspondence //
		////////////////////////////////////////////
		TH1D* clusters_module_number_correspondence_layer_1;
		TH1D* clusters_module_number_correspondence_layer_2;
		TH1D* clusters_module_number_correspondence_layer_3;
		TH1D* clusters_ladder_number_correspondence_layer_1;
		TH1D* clusters_ladder_number_correspondence_layer_2;
		TH1D* clusters_ladder_number_correspondence_layer_3;
		TH1D* clusters_ROCx_correspondence_layer_1;
		TH1D* clusters_ROCx_correspondence_layer_2;
		TH1D* clusters_ROCx_correspondence_layer_3;
		TH1D* clusters_ROCy_correspondence_layer_1;
		TH1D* clusters_ROCy_correspondence_layer_2;
		TH1D* clusters_ROCy_correspondence_layer_3;

		/////////////////////////////////////////////////////
		// Custer number instlumi and layer correspondence //
		/////////////////////////////////////////////////////
		
		TH1D* clusters_instlumi_correspondence_layer_1;
		TH1D* clusters_instlumi_correspondence_layer_2;
		TH1D* clusters_instlumi_correspondence_layer_3;
		TH1D* clusters_layer_correspondence;

		////////////////////////////////////////////
		// Hit efficiency geometry correspondence //
		////////////////////////////////////////////
		
		TH1D* hit_efficiency_module_number_correspondence_layer_1;
		TH1D* hit_efficiency_module_number_correspondence_layer_2;
		TH1D* hit_efficiency_module_number_correspondence_layer_3;
		TH1D* hit_efficiency_ladder_number_correspondence_layer_1;
		TH1D* hit_efficiency_ladder_number_correspondence_layer_2;
		TH1D* hit_efficiency_ladder_number_correspondence_layer_3;
		TH1D* hit_efficiency_ROCx_correspondence_layer_1;
		TH1D* hit_efficiency_ROCx_correspondence_layer_2;
		TH1D* hit_efficiency_ROCx_correspondence_layer_3;
		TH1D* hit_efficiency_ROCy_correspondence_layer_1;
		TH1D* hit_efficiency_ROCy_correspondence_layer_2;
		TH1D* hit_efficiency_ROCy_correspondence_layer_3;

		//////////////////////////////////////////////////////
		// Hit efficiency instlumi and layer correspondence //
		//////////////////////////////////////////////////////
		
		TH1D* hit_efficiency_instlumi_correspondence_layer_1;
		TH1D* hit_efficiency_instlumi_correspondence_layer_2;
		TH1D* hit_efficiency_instlumi_correspondence_layer_3;
		TH1D* hit_efficiency_layer_correspondence;

		////////////////////////////////////////////////
		// Hit efficiency impact angle correspondence //
		////////////////////////////////////////////////

		TH1D* hit_efficiency_impact_alpha_correspondence_layer_1;
		TH1D* hit_efficiency_impact_alpha_correspondence_layer_2;
		TH1D* hit_efficiency_impact_alpha_correspondence_layer_3;
		TH1D* hit_efficiency_impact_beta_correspondence_layer_1;
		TH1D* hit_efficiency_impact_beta_correspondence_layer_2;
		TH1D* hit_efficiency_impact_beta_correspondence_layer_3;

		/******************************************************************************************************/
		/******************************************************************************************************/
		/*                                                                                                    */
		/*                                           2D Histograms                                            */
		/*                                                                                                    */
		/******************************************************************************************************/
		/******************************************************************************************************/
		
		/////////////////////////////////////////////////////////////////////////////////////////////
		// Hit efficiency, total hit number, cluster occupancy and relative cluster occupancy maps //
		/////////////////////////////////////////////////////////////////////////////////////////////
		
		TH2D* total_hits_on_modules_map_layer_1;
		TH2D* total_hits_on_modules_map_layer_2;
		TH2D* total_hits_on_modules_map_layer_3;
		TH2D* total_hits_on_ROCs_map_layer_1;
		TH2D* total_hits_on_ROCs_map_layer_2;
		TH2D* total_hits_on_ROCs_map_layer_3;
		TH2D* clusters_on_modules_map_layer_1;
		TH2D* clusters_on_modules_map_layer_2;
		TH2D* clusters_on_modules_map_layer_3;
		TH2D* clusters_on_ROCs_map_layer_1;
		TH2D* clusters_on_ROCs_map_layer_2;
		TH2D* clusters_on_ROCs_map_layer_3;
		TH2D* hit_efficiency_on_modules_map_layer_1;
		TH2D* hit_efficiency_on_modules_map_layer_2;
		TH2D* hit_efficiency_on_modules_map_layer_3;
		TH2D* hit_efficiency_on_ROCs_map_layer_1;
		TH2D* hit_efficiency_on_ROCs_map_layer_2;
		TH2D* hit_efficiency_on_ROCs_map_layer_3;
		TH2D* relative_cluster_occupancy_on_modules_map_layer_1;
		TH2D* relative_cluster_occupancy_on_modules_map_layer_2;
		TH2D* relative_cluster_occupancy_on_modules_map_layer_3;
		TH2D* relative_cluster_occupancy_on_ROCs_map_layer_1;
		TH2D* relative_cluster_occupancy_on_ROCs_map_layer_2;
		TH2D* relative_cluster_occupancy_on_ROCs_map_layer_3;

		//////////////////////////////////////
		// Alpha-beta impact angle analysis //
		//////////////////////////////////////

		TH2D* beta_vs_alpha_correlation_layer_1;
		TH2D* beta_vs_alpha_correlation_layer_2;
		TH2D* beta_vs_alpha_correlation_layer_3;

		TH2D* hit_efficiency_vs_beta_vs_alpha_layer_1;
		TH2D* hit_efficiency_vs_beta_vs_alpha_layer_2;
		TH2D* hit_efficiency_vs_beta_vs_alpha_layer_3;

		TH2D* beta_vs_alpha_correlation_layer_1_rebinned = NULL;
		TH2D* beta_vs_alpha_correlation_layer_2_rebinned = NULL;
		TH2D* beta_vs_alpha_correlation_layer_3_rebinned = NULL;
		TH2D* hit_efficiency_vs_beta_vs_alpha_layer_1_rebinned = NULL;
		TH2D* hit_efficiency_vs_beta_vs_alpha_layer_2_rebinned = NULL;
		TH2D* hit_efficiency_vs_beta_vs_alpha_layer_3_rebinned = NULL;

		/******************************************************************************************************/
		/******************************************************************************************************/
		/*                                                                                                    */
		/*                                           ROC Filtering                                            */
		/*                                                                                                    */
		/******************************************************************************************************/
		/******************************************************************************************************/

		////////////////////////////////////////////
		// Filtering the least efficient ROCs out //
		////////////////////////////////////////////
		
		TH2D* bad_ROCs_simple_efficiency_base_layer_1;
		TH2D* bad_ROCs_simple_efficiency_base_layer_2;
		TH2D* bad_ROCs_simple_efficiency_base_layer_3;
		TH2D* bad_ROCs_weighted_efficiency_base_layer_1;
		TH2D* bad_ROCs_weighted_efficiency_base_layer_2;
		TH2D* bad_ROCs_weighted_efficiency_base_layer_3;

		////////////////////////////////////////////////////////////////////////////////////
		// Filtering the ROCs by their relative cluster occupancies; not used in analysis //
		////////////////////////////////////////////////////////////////////////////////////
		
		TH2D* bad_ROCs_relative_occupancy_base_layer_1;
		TH2D* bad_ROCs_relative_occupancy_base_layer_2;
		TH2D* bad_ROCs_relative_occupancy_base_layer_3;
		TH2D* bad_ROCs_cluster_occupancy_base_layer_1;
		TH2D* bad_ROCs_cluster_occupancy_base_layer_2;
		TH2D* bad_ROCs_cluster_occupancy_base_layer_3;
		TH2D* bad_ROCs_neighbour_hits_base_layer_1;
		TH2D* bad_ROCs_neighbour_hits_base_layer_2;
		TH2D* bad_ROCs_neighbour_hits_base_layer_3;

		//////////////////////////////////////////////////////////////////////////
		// Combining all the bad ROC filters that are relevant for the analysis //
		//////////////////////////////////////////////////////////////////////////
		
		TH2D* bad_ROCs_all_layer_1;
		TH2D* bad_ROCs_all_layer_2;
		TH2D* bad_ROCs_all_layer_3;

		TH1D* bad_ROC_filtering_3_distribution_layer_1;
		TH1D* bad_ROC_filtering_3_distribution_layer_2;
		TH1D* bad_ROC_filtering_3_distribution_layer_3;

		TF1* bad_ROC_filtering_3_fit_layer_1;
		TF1* bad_ROC_filtering_3_fit_layer_2;
		TF1* bad_ROC_filtering_3_fit_layer_3;


		Post_loop_analyzer_data_structure();
		~Post_loop_analyzer_data_structure();
};

Post_loop_analyzer_data_structure::Post_loop_analyzer_data_structure()
{

}

Post_loop_analyzer_data_structure::~Post_loop_analyzer_data_structure()
{
	if(debug_prompt_destructors)
	{
		std::cerr << debug_prompt << "~Post_loop_analyzer_data_structure()" << std::endl;
	}
}

#endif