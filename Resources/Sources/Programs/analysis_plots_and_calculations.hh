#ifndef ANALYSIS_PLOTS_AND_CALCULATIONS_H
#define ANALYSIS_PLOTS_AND_CALCULATIONS_H

#include "../Includes/Console_actor/common_actors.hh"
#include "../Includes/Post_loop_analyzer/Post_loop_analyzer.hh"
#include "../Includes/ROC_position/ROC_position.hh"

#include "TCanvas.h"
#include "TPad.h"
#include "TH2.h"
#include "TFile.h"
#include "TF1.h"
#include "TTree.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <memory>
#include <set>

/******************************************************************************************************/
/******************************************************************************************************/
/*                                                                                                    */
/*                       Important function: analysis_plots_and_calculations()                        */
/*                                                                                                    */
/******************************************************************************************************/
/******************************************************************************************************/

// int analysis_plots_and_calculations(int argc, char** argv, Input_output_interface* input_output_interface, double& badroc_limit_layer_1, double& badroc_limit_layer_2, double& badroc_limit_layer_3)
int analysis_plots_and_calculations(int argc, char** argv, const std::string& firts_loop_output_file_name_p, const std::string& analysis_output_name_p, std::set<ROC_position>& bad_roc_list_p)
{
	std::unique_ptr<Timer_colored> timer(new Timer_colored(timer_prompt));
	std::string process_name(argv[0]);
	if(process_name.find("./", 0, 2) != std::string::npos)
	{
		process_name = process_name.substr(process_name.find("./", 0, 2) + 2);
	}
	std::cout << process_prompt << process_name << " is now calculating layer dynamic inefficencies (ignoring static ROC errors)..." << std::endl;
	timer -> restart("Starting analysis...");
	std::unique_ptr<Post_loop_analyzer> analyzer(new Post_loop_analyzer);
	std::cerr << debug_prompt << "Analysis input name: " << firts_loop_output_file_name_p << std::endl;
	analyzer -> fetch_histograms_from_file(firts_loop_output_file_name_p);
	analyzer -> create_tier_1_analysis_plots();
	analyzer -> create_tier_2_analysis_plots();
	std::set<ROC_position> bad_ROC_list_temp(analyzer -> get_bad_ROC_list());
	std::swap(bad_ROC_list_temp, bad_roc_list_p);
	analyzer -> save_histograms(analysis_output_name_p.c_str());
	timer -> print_seconds("Took about ", " second(s).");
	return 0;
}

#endif