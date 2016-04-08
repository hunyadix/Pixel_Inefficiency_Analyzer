#ifndef POST_LOOP_ANALYZER_HISTOGRAM_FETCHER_H
#define POST_LOOP_ANALYZER_HISTOGRAM_FETCHER_H

#include "Post_loop_analyzer_data_structure.hh"
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

class Post_loop_analyzer_histogram_fetcher : 
virtual protected Post_loop_analyzer_data_structure
{
	protected:

		template<typename Object_type>
		Object_type* get_histogram_by_name(TFile* file_p, const std::string& name_p);
	public:
		Post_loop_analyzer_histogram_fetcher();
		~Post_loop_analyzer_histogram_fetcher();
		void fetch_histograms_from_file(const std::string& filename_p);
};

Post_loop_analyzer_histogram_fetcher::Post_loop_analyzer_histogram_fetcher()
{

}

Post_loop_analyzer_histogram_fetcher::~Post_loop_analyzer_histogram_fetcher()
{
	if(debug_prompt_destructors)
	{
		std::cerr << debug_prompt << "~Post_loop_analyzer_histogram_fetcher()" << std::endl;
	}
}

/******************************************************************************************************/
/******************************************************************************************************/
/*                                                                                                    */
/*                                           Private Methods                                          */
/*                                                                                                    */
/******************************************************************************************************/
/******************************************************************************************************/

template<typename Object_type>
Object_type* Post_loop_analyzer_histogram_fetcher::get_histogram_by_name(TFile* file_p, const std::string& name_p)
{
	Object_type* histogram = static_cast<Object_type*>(file_p -> Get(name_p.c_str()));
	if(!histogram)
	{
		std::cerr << error_prompt << "Error finding histogram: " << name_p << std::endl;
		return nullptr;
	}
	histogram -> SetDirectory(0);
	return histogram;
}

/******************************************************************************************************/
/******************************************************************************************************/
/*                                                                                                    */
/*                                           Public Methods                                           */
/*                                                                                                    */
/******************************************************************************************************/
/******************************************************************************************************/

void Post_loop_analyzer_histogram_fetcher::fetch_histograms_from_file(const std::string& filename_p)
{
	TFile* input_file(new TFile(filename_p.c_str(), "READ"));
	if(!(input_file -> IsOpen()))
	{
		std::cerr << error_prompt << "Error opening the input file: " << filename_p << "." << std::endl;
	}

	// ============================== 1D histograms ==============================

	// Cluster number geometry correspondence
	this -> clusters_module_number_correspondence_layer_1       = this -> get_histogram_by_name<TH1D>(input_file, "Modules/Layer1");
	this -> clusters_module_number_correspondence_layer_2       = this -> get_histogram_by_name<TH1D>(input_file, "Modules/Layer2");
	this -> clusters_module_number_correspondence_layer_3       = this -> get_histogram_by_name<TH1D>(input_file, "Modules/Layer3");
	this -> clusters_ladder_number_correspondence_layer_1       = this -> get_histogram_by_name<TH1D>(input_file, "Ladders/Layer1");
	this -> clusters_ladder_number_correspondence_layer_2       = this -> get_histogram_by_name<TH1D>(input_file, "Ladders/Layer2");
	this -> clusters_ladder_number_correspondence_layer_3       = this -> get_histogram_by_name<TH1D>(input_file, "Ladders/Layer3");
	this -> clusters_ROCx_correspondence_layer_1                = this -> get_histogram_by_name<TH1D>(input_file, "ClustROCx/Layer1");
	this -> clusters_ROCx_correspondence_layer_2                = this -> get_histogram_by_name<TH1D>(input_file, "ClustROCx/Layer2");
	this -> clusters_ROCx_correspondence_layer_3                = this -> get_histogram_by_name<TH1D>(input_file, "ClustROCx/Layer3");
	this -> clusters_ROCy_correspondence_layer_1                = this -> get_histogram_by_name<TH1D>(input_file, "ClustROCy/Layer1");
	this -> clusters_ROCy_correspondence_layer_2                = this -> get_histogram_by_name<TH1D>(input_file, "ClustROCy/Layer2");
	this -> clusters_ROCy_correspondence_layer_3                = this -> get_histogram_by_name<TH1D>(input_file, "ClustROCy/Layer3");
	// Custer number instlumi and layer correspondence
	this -> clusters_instlumi_correspondence_layer_1            = this -> get_histogram_by_name<TH1D>(input_file, "Instlumi/Layer1");
	this -> clusters_instlumi_correspondence_layer_2            = this -> get_histogram_by_name<TH1D>(input_file, "Instlumi/Layer2");
	this -> clusters_instlumi_correspondence_layer_3            = this -> get_histogram_by_name<TH1D>(input_file, "Instlumi/Layer3");
	// Hit efficiency geometry correspondence
	this -> hit_efficiency_module_number_correspondence_layer_1 = this -> get_histogram_by_name<TH1D>(input_file, "HitEfficiency_vs_Modules/Layer1");
	this -> hit_efficiency_module_number_correspondence_layer_2 = this -> get_histogram_by_name<TH1D>(input_file, "HitEfficiency_vs_Modules/Layer2");
	this -> hit_efficiency_module_number_correspondence_layer_3 = this -> get_histogram_by_name<TH1D>(input_file, "HitEfficiency_vs_Modules/Layer3");
	this -> hit_efficiency_ladder_number_correspondence_layer_1 = this -> get_histogram_by_name<TH1D>(input_file, "HitEfficiency_vs_Ladders/Layer1");
	this -> hit_efficiency_ladder_number_correspondence_layer_2 = this -> get_histogram_by_name<TH1D>(input_file, "HitEfficiency_vs_Ladders/Layer2");
	this -> hit_efficiency_ladder_number_correspondence_layer_3 = this -> get_histogram_by_name<TH1D>(input_file, "HitEfficiency_vs_Ladders/Layer3");
	this -> hit_efficiency_ROCx_correspondence_layer_1          = this -> get_histogram_by_name<TH1D>(input_file, "HitEfficiency_vs_TrajROCx/Layer1");
	this -> hit_efficiency_ROCx_correspondence_layer_2          = this -> get_histogram_by_name<TH1D>(input_file, "HitEfficiency_vs_TrajROCx/Layer2");
	this -> hit_efficiency_ROCx_correspondence_layer_3          = this -> get_histogram_by_name<TH1D>(input_file, "HitEfficiency_vs_TrajROCx/Layer3");
	this -> hit_efficiency_ROCy_correspondence_layer_1          = this -> get_histogram_by_name<TH1D>(input_file, "HitEfficiency_vs_TrajROCy/Layer1");
	this -> hit_efficiency_ROCy_correspondence_layer_2          = this -> get_histogram_by_name<TH1D>(input_file, "HitEfficiency_vs_TrajROCy/Layer2");
	this -> hit_efficiency_ROCy_correspondence_layer_3          = this -> get_histogram_by_name<TH1D>(input_file, "HitEfficiency_vs_TrajROCy/Layer3");
	// Hit_efficiency instlumi and layer correspondence
	this -> hit_efficiency_instlumi_correspondence_layer_1      = this -> get_histogram_by_name<TH1D>(input_file, "HitEfficiency_vs_Instlumi/Layer1");
	this -> hit_efficiency_instlumi_correspondence_layer_2      = this -> get_histogram_by_name<TH1D>(input_file, "HitEfficiency_vs_Instlumi/Layer2");
	this -> hit_efficiency_instlumi_correspondence_layer_3      = this -> get_histogram_by_name<TH1D>(input_file, "HitEfficiency_vs_Instlumi/Layer3");
	this -> hit_efficiency_impact_alpha_correspondence_layer_1  = this -> get_histogram_by_name<TH1D>(input_file, "HitEfficiency_vs_ImpactAngleAlpha/Layer1");
	this -> hit_efficiency_impact_alpha_correspondence_layer_2  = this -> get_histogram_by_name<TH1D>(input_file, "HitEfficiency_vs_ImpactAngleAlpha/Layer2");
	this -> hit_efficiency_impact_alpha_correspondence_layer_3  = this -> get_histogram_by_name<TH1D>(input_file, "HitEfficiency_vs_ImpactAngleAlpha/Layer3");
	this -> hit_efficiency_impact_beta_correspondence_layer_1   = this -> get_histogram_by_name<TH1D>(input_file, "HitEfficiency_vs_ImpactAngleBeta/Layer1");
	this -> hit_efficiency_impact_beta_correspondence_layer_2   = this -> get_histogram_by_name<TH1D>(input_file, "HitEfficiency_vs_ImpactAngleBeta/Layer2");
	this -> hit_efficiency_impact_beta_correspondence_layer_3   = this -> get_histogram_by_name<TH1D>(input_file, "HitEfficiency_vs_ImpactAngleBeta/Layer3");
	this -> hit_efficiency_impact_gamma_correspondence_layer_1  = this -> get_histogram_by_name<TH1D>(input_file, "HitEfficiency_vs_ImpactAngleGamma/Layer1");
	this -> hit_efficiency_impact_gamma_correspondence_layer_2  = this -> get_histogram_by_name<TH1D>(input_file, "HitEfficiency_vs_ImpactAngleGamma/Layer2");
	this -> hit_efficiency_impact_gamma_correspondence_layer_3  = this -> get_histogram_by_name<TH1D>(input_file, "HitEfficiency_vs_ImpactAngleGamma/Layer3");
	this -> hit_efficiency_layer_correspondence                 = this -> get_histogram_by_name<TH1D>(input_file, "HitEfficiency_vs_Layers/HitEfficiency_vs_Layers");

	// ============================== 2D histograms ==============================

	// Hit efficiency, total hit number, cluster occupancy and relative cluster occupancy maps
	this -> total_hits_on_modules_map_layer_1       = this -> get_histogram_by_name<TH2D>(input_file, "TLadders_vs_TModules/Layer1");
	this -> total_hits_on_modules_map_layer_2       = this -> get_histogram_by_name<TH2D>(input_file, "TLadders_vs_TModules/Layer2");
	this -> total_hits_on_modules_map_layer_3       = this -> get_histogram_by_name<TH2D>(input_file, "TLadders_vs_TModules/Layer3");
	this -> total_hits_on_ROCs_map_layer_1          = this -> get_histogram_by_name<TH2D>(input_file, "TrajROCy_vs_TrajROCx/Layer1");
	this -> total_hits_on_ROCs_map_layer_2          = this -> get_histogram_by_name<TH2D>(input_file, "TrajROCy_vs_TrajROCx/Layer2");
	this -> total_hits_on_ROCs_map_layer_3          = this -> get_histogram_by_name<TH2D>(input_file, "TrajROCy_vs_TrajROCx/Layer3");
	this -> clusters_on_modules_map_layer_1         = this -> get_histogram_by_name<TH2D>(input_file, "Ladders_vs_Modules/Layer1");
	this -> clusters_on_modules_map_layer_2         = this -> get_histogram_by_name<TH2D>(input_file, "Ladders_vs_Modules/Layer2");
	this -> clusters_on_modules_map_layer_3         = this -> get_histogram_by_name<TH2D>(input_file, "Ladders_vs_Modules/Layer3");
	this -> clusters_on_ROCs_map_layer_1            = this -> get_histogram_by_name<TH2D>(input_file, "ClustROCy_vs_ClustROCx/Layer1");
	this -> clusters_on_ROCs_map_layer_2            = this -> get_histogram_by_name<TH2D>(input_file, "ClustROCy_vs_ClustROCx/Layer2");
	this -> clusters_on_ROCs_map_layer_3            = this -> get_histogram_by_name<TH2D>(input_file, "ClustROCy_vs_ClustROCx/Layer3");
	this -> hit_efficiency_on_modules_map_layer_1   = this -> get_histogram_by_name<TH2D>(input_file, "HitEfficiency_vs_Ladders_vs_Modules/Layer1");
	this -> hit_efficiency_on_modules_map_layer_2   = this -> get_histogram_by_name<TH2D>(input_file, "HitEfficiency_vs_Ladders_vs_Modules/Layer2");
	this -> hit_efficiency_on_modules_map_layer_3   = this -> get_histogram_by_name<TH2D>(input_file, "HitEfficiency_vs_Ladders_vs_Modules/Layer3");
	this -> hit_efficiency_on_ROCs_map_layer_1      = this -> get_histogram_by_name<TH2D>(input_file, "HitEfficiency_vs_TrajROCy_vs_TrajROCx/Layer1");
	this -> hit_efficiency_on_ROCs_map_layer_2      = this -> get_histogram_by_name<TH2D>(input_file, "HitEfficiency_vs_TrajROCy_vs_TrajROCx/Layer2");
	this -> hit_efficiency_on_ROCs_map_layer_3      = this -> get_histogram_by_name<TH2D>(input_file, "HitEfficiency_vs_TrajROCy_vs_TrajROCx/Layer3");
	this -> beta_vs_alpha_correlation_layer_1       = this -> get_histogram_by_name<TH2D>(input_file, "ImpactAngleBeta_vs_ImpactAngleAlpha/Layer1");
	this -> beta_vs_alpha_correlation_layer_2       = this -> get_histogram_by_name<TH2D>(input_file, "ImpactAngleBeta_vs_ImpactAngleAlpha/Layer2");
	this -> beta_vs_alpha_correlation_layer_3       = this -> get_histogram_by_name<TH2D>(input_file, "ImpactAngleBeta_vs_ImpactAngleAlpha/Layer3");
	this -> beta_vs_alpha_correlation_layer_1       = this -> get_histogram_by_name<TH2D>(input_file, "ImpactAngleBeta_vs_ImpactAngleGamma/Layer1");
	this -> beta_vs_alpha_correlation_layer_2       = this -> get_histogram_by_name<TH2D>(input_file, "ImpactAngleBeta_vs_ImpactAngleGamma/Layer2");
	this -> beta_vs_alpha_correlation_layer_3       = this -> get_histogram_by_name<TH2D>(input_file, "ImpactAngleBeta_vs_ImpactAngleGamma/Layer3");
	this -> hit_efficiency_vs_beta_vs_alpha_layer_1 = this -> get_histogram_by_name<TH2D>(input_file, "HitEfficiency_vs_ImpactAngleBeta_vs_ImpactAngleAlpha/Layer1");
	this -> hit_efficiency_vs_beta_vs_alpha_layer_2 = this -> get_histogram_by_name<TH2D>(input_file, "HitEfficiency_vs_ImpactAngleBeta_vs_ImpactAngleAlpha/Layer2");
	this -> hit_efficiency_vs_beta_vs_alpha_layer_3 = this -> get_histogram_by_name<TH2D>(input_file, "HitEfficiency_vs_ImpactAngleBeta_vs_ImpactAngleAlpha/Layer3");
	this -> hit_efficiency_vs_beta_vs_gamma_layer_1 = this -> get_histogram_by_name<TH2D>(input_file, "HitEfficiency_vs_ImpactAngleBeta_vs_ImpactAngleGamma/Layer1");
	this -> hit_efficiency_vs_beta_vs_gamma_layer_2 = this -> get_histogram_by_name<TH2D>(input_file, "HitEfficiency_vs_ImpactAngleBeta_vs_ImpactAngleGamma/Layer2");
	this -> hit_efficiency_vs_beta_vs_gamma_layer_3 = this -> get_histogram_by_name<TH2D>(input_file, "HitEfficiency_vs_ImpactAngleBeta_vs_ImpactAngleGamma/Layer3");

	input_file -> Close();
	delete input_file;

	this -> fetching_done = true;
}


#endif