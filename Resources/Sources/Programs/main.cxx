/******************************************************************************************************/
/*                                                                                                    */
/*                      Program to loop through TimingStudy Ntuples and perform                       */
/*                      efficiency analysis.                                                          */
/*                      Date: 2015. CMSSW version: CMSSW_7_5_0                                        */
/*                      Author: Adam Hunyadi                                                          */
/*                      E-mail: hunyadix@gmail.com                                                    */
/*                                                                                                    */
/******************************************************************************************************/

#include "../Includes/Ntuple_reader/Ntuple_reader.hh"
#include "../Includes/Timer/Timer_colored.hh"
#include "../Includes/Console_actor/Console_actor.hh"
#include "../Includes/Console_actor/common_actors.hh"

#include "../Includes/Histogram_generation/Root_color_sets/root_color_sets.hh"

#include "../Includes/Command_line_parsing/main_command_line_parser.hh"
#include "../Includes/Ntuple_reader/ntuple_reader_setup.hh"

#include "analysis_plots_and_calculations.hh"
#include "second_loop.hh"
#include "second_loop_analysis.hh"

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
#include <algorithm>
#include <set>

bool debug                            = false;
bool debug_show_smarthisto_outputs    = true;
bool debug_prompt_inputs              = true;
bool debug_prompt_file_input_output   = false;
bool debug_prompt_destructors         = true;
bool debug_prompt_histogram_saving    = true;
bool debug_customsmarthisto_inserting = false;
bool debug_customsmarthisto_updating  = false;

/******************************************************************************************************/
/******************************************************************************************************/
/*                                                                                                    */
/*                                    Important function: main()                                      */
/*                                                                                                    */
/******************************************************************************************************/
/******************************************************************************************************/

int main(int argc, char** argv)
{
	using Histogram_generation::Cut_factory;
	try
	{
		/******************************************************************************************************/
		/*                                       Setting up the project                                       */
		/******************************************************************************************************/

		std::unique_ptr<Timer_colored> timer(new Timer_colored(timer_prompt));
		system("clear");
		std::string process_name(argv[0]);
		if(process_name.find("./", 0, 2) != std::string::npos)
		{
			process_name = process_name.substr(process_name.find("./", 0, 2) + 2);
		}
		std::cout << process_prompt << process_name << " is being executed..." << std::endl;

/////////////////////////////////////////////////
// Print out where the program root is located //
/////////////////////////////////////////////////
		
		char buffer[256];
		char *ppath = getcwd(buffer, sizeof(buffer));
		if(ppath)
		{
			std::cout << debug_prompt << "Buffer: " << buffer << std::endl;
		}

////////////////////
// File variables //
////////////////////

		std::string input_data_file_name; // Disabled, use input file_paths instead
		std::string input_file_paths_file_name;
		std::string firts_loop_output_file_name;
		std::string analysis_output_name;
		std::string second_loop_output_name;
		std::string second_analysis_output_name;

		std::string bad_ROC_file_input_path;       // For reading saved bad ROCs
		std::string analysis_output_parent_folder; // For save the bad ROC list
		std::string bad_ROC_list_save_path;

///////////////////////
// Control variables //
///////////////////////

		bool tree_loops_requested                      = true;
		bool analysis_plots_and_calculations_requested = false;
		bool second_loop_requested                     = false;
		bool second_analysis_requested                 = false;

/////////////////////
// Data file paths //
/////////////////////
		
/////////////////////////
// Data for validation //
/////////////////////////

		// input_file_paths_file_name   = "Data/Dcolscans/dcol100_paths.txt";
		// firts_loop_output_file_name  = "Results/Dcolscans/dcol100.root";
		// analysis_output_name         = "Results/Dcolscans/dcol100_stats.root";
		// second_loop_output_file_name = "Results/Dcolscans/dcol100_with_filtering.root";

		// input_file_paths_file_name  = "Data/Reweighting_parts/reweighting_part_10.txt";
		// firts_loop_output_file_name = "Results/High_stat_2232b/reweighting_validation_data_part_10_step_1.root";
		// analysis_output_name        = "Results/High_stat_2232b/reweighting_validation_data_part_10_step_2.root";
		// second_loop_output_name     = "Results/High_stat_2232b/reweighting_validation_data_part_10_step_3.root";
		// second_analysis_output_name = "Results/High_stat_2232b/reweighting_validation_data_part_10_step_4.root";
		
////////////////////////////////////////////////
// Low statistics check (data for validation) //
////////////////////////////////////////////////

		// input_file_paths_file_name  = "Data/low_stat_reweighting_validadtion.txt";
		// firts_loop_output_file_name = "Results/Low_stat_rew_val/debug_low_stat_reweighting_validation_step_1.root";
		// analysis_output_name        = "Results/Low_stat_rew_val/debug_low_stat_reweighting_validation_step_2.root";
		// second_loop_output_name     = "Results/Low_stat_rew_val/debug_low_stat_reweighting_validation_step_3.root";
		// second_analysis_output_name = "Results/Low_stat_rew_val/debug_low_stat_reweighting_validation_step_4.root";
		
///////////////////////
// MC for validation //
///////////////////////

		// input_file_paths_file_name  = "Data/mc_minbias_2015_paths.txt";
		// firts_loop_output_file_name = "Results/mc_for_reweighting_validation.root";

/////////////////////////////////////////
// General purpose check (data and mc) //
/////////////////////////////////////////

		input_file_paths_file_name  = "Data/debug_list.txt";
		// input_file_paths_file_name  = "Data/PIA_debug.txt";
		firts_loop_output_file_name = "Results/Debug/debug_data_step_1.root";
		analysis_output_name        = "Results/Debug/debug_data_step_2.root";
		second_loop_output_name     = "Results/Debug/debug_data_step_3.root";
		second_analysis_output_name = "Results/Debug/debug_data_step_4.root";

//////////////////////
// 2012 Monte-Carlo //
//////////////////////

		// input_file_paths_file_name  = "Data/2012_MC_dynineff.txt";
		// firts_loop_output_file_name = "Results/2012_MC_dynineff/step_1.root";
		// analysis_output_name        = "Results/2012_MC_dynineff/step_2.root";
		// second_loop_output_name     = "Results/2012_MC_dynineff/step_3.root";
		// second_analysis_output_name = "Results/2012_MC_dynineff/step_4.root";

///////////////////////////////
// 2012 general purpose data //
///////////////////////////////

		// input_file_paths_file_name  = "Data/2012_data.txt";
		// firts_loop_output_file_name = "Results/2012/complex_step_1.root";
		// analysis_output_name        = "Results/2012/complex_step_2.root";
		// second_loop_output_name     = "Results/2012/complex_step_3.root";
		// second_analysis_output_name = "Results/2012/complex_step_4.root";

/////////////////////////
// FPix check for run  //
/////////////////////////

		// input_file_paths_file_name  = "Data/2016_run_273725_no_cut.txt";
		// firts_loop_output_file_name = "Results/FPix_check/step_1.root";
		// analysis_output_name        = "Results/FPix_check/step_2.root";
		// second_loop_output_name     = "Results/FPix_check/step_3.root";
		// second_analysis_output_name = "Results/FPix_check/step_4.root";

////////////////////////////////////////////////////
// Data or simulation (for instlumi calculation)  //
////////////////////////////////////////////////////

		bool is_mc = false; // Turn on reweighting

//////////////////////////////////////////////////////
// Pileup reweighting based on this file/histoggram //
//////////////////////////////////////////////////////

		// Reweighting is active, when is_mc is true.
		// std::string reweighting_file_path      = "/data/jkarancs/CMSSW/TimingStudy/CMSSW_7_1_0_pre1/src/DPGAnalysis/PixelTimingStudy/test/PixelHistoMaker/input/pileup_data_2232b_69mb.root";
		// std::string reweighting_histogram_name = "pileup";

		std::string reweighting_file_path      = "/afs/cern.ch/user/a/ahunyadi/private/Pixel_Inefficiency_Analyzer/Results/2012/general_step_1.root";
		std::string reweighting_histogram_name = "Pileup/Pileup";

/////////////////////////
// Extra cut selection //
/////////////////////////

		bool add_custom_extra_cut = false;

/////////////////////////////
// Read bad ROCs from file //
/////////////////////////////

		bool read_bad_ROCs_from_file = true;
		bad_ROC_file_input_path = "";

////////////////////////////
// Bad ROC list save path //
////////////////////////////

		if(analysis_output_name.size())
		{
			analysis_output_parent_folder = analysis_output_name.substr(0, analysis_output_name.find_last_of("/\\"));
			bad_ROC_list_save_path = analysis_output_parent_folder + "/bad_ROC_list.txt";
			bad_ROC_file_input_path = bad_ROC_list_save_path;
		}

//////////////
// Schedule //
//////////////

		int schedule = 
		               Ntuple_reader::Loop_request_flags::event_tree_loop_request |
		               // Ntuple_reader::Loop_request_flags::lumi_tree_loop_request  |
		               // Ntuple_reader::Loop_request_flags::run_tree_loop_request   |
		               Ntuple_reader::Loop_request_flags::clust_tree_loop_request |
		               Ntuple_reader::Loop_request_flags::traj_tree_loop_request  |
		               0;

//////////////////////////////////////////////////////////
// Parsing command line arguments and performing checks //
//////////////////////////////////////////////////////////

		// Disabled
		//parse_main_command_line_arguments(argc, argv, input_data_file_name, firts_loop_output_file_name);
		
		std::set<ROC_position> bad_ROC_list;

		if(is_mc)
		{
			std::cout << process_prompt << "Running on " << c_lgray << "MONTE CARLO SIMULATED" << c_def << " data (MC)." << std::endl;
			std::cout << process_prompt << "Reweighting is " << c_red << "ACTIVE" << c_def << "." << std::endl;
		}
		else
		{
			std::cout << process_prompt << "Running on " << c_lgray << "REAL" << c_def << " data (RAWTOTIMINGSTUDY)." << std::endl;
		}
		               
		if(tree_loops_requested)
		{
			timer -> restart("Preparing to run tree loops...");
			Ntuple_reader* ntuple_reader = new Ntuple_reader();

/******************************************************************************************************/
/*                                     Initializing the data files                                    */
/******************************************************************************************************/

			ntuple_reader -> read_input_paths_from_file(input_file_paths_file_name.c_str());
			ntuple_reader -> set_output_file(firts_loop_output_file_name, "RECREATE");

			ntuple_reader -> set_schedule(schedule);

/******************************************************************************************************/
/*                                        Configuring the run                                         */
/******************************************************************************************************/

			std::string extra_cut("");

			std::shared_ptr<Custom_smart_histos> histogram_requests(new Custom_smart_histos);
			std::shared_ptr<Histogram_reweighter> histogram_reweighter(new Histogram_reweighter(ntuple_reader));
			histogram_requests -> AddNewCut(Cut_factory::get_cut("", ntuple_reader));
			
///////////////////////
// Custom extra cuts //
///////////////////////

			std::unique_ptr<Histogram_generation::Cut> bunchspacing_cut;

			if(add_custom_extra_cut)
			{
				const char* bunchspacing_selection_name = "bunchspacing_selection";
				bunchspacing_cut.reset(new Histogram_generation::Cut);
				bunchspacing_cut -> set_name(bunchspacing_selection_name);
// 2232 bunch cut
				const int& run = ntuple_reader -> get_event_field_ptr() -> run;
				// bunchspacing_cut -> set_cut_function([&run] () { return (run >= 260099 && run <= 260235) || (run >= 260424 && run <= 260627); } );
// events of run 273725 (2016, pilot_blade mystery)	
				bunchspacing_cut -> set_cut_function([&run] () { return run == 273725; } );
				histogram_requests -> AddNewCut(bunchspacing_cut);
				extra_cut = bunchspacing_selection_name;
			}

			Ntuple_reader_setup_options options = 
			{
				.ntuple_reader              = ntuple_reader,
				.histogram_requests         = histogram_requests,
				.histogram_reweighter       = histogram_reweighter,
				.extra_cut                  = extra_cut,
				.reweighting_file_path      = reweighting_file_path, 
				.reweighting_histogram_name = reweighting_histogram_name, 
				.is_mc                      = is_mc
			};
			int setup_results = ntuple_reader_setup(options);
			if(setup_results != 0)
			{
				std::cerr << error_prompt << "The ntuple_reader_setup() function failed." << std::endl;
			};
			timer -> print_seconds("Took about ", " second(s).");

/******************************************************************************************************/
/*                                  Looping through the Ntuple files                                  */
/******************************************************************************************************/
			
			timer -> restart("Running tree loops...");
			ntuple_reader -> run();
			timer -> print_seconds("Took about ", " second(s).");
			delete ntuple_reader; // contains call to Input_output_interface destructor, might interfere with root memory handling but is fine.
		}

/////////////////////////////////////////////////////////
// Creating plots, and filtering the data for bad ROCS //
/////////////////////////////////////////////////////////

		if(analysis_plots_and_calculations_requested != 0)
		{
			int analysis_plots_and_calculations_results = analysis_plots_and_calculations(argc, argv, firts_loop_output_file_name, analysis_output_name, bad_ROC_list);
			if(analysis_plots_and_calculations_results)
			{
				std::cout << error_prompt << "Function analysis_plots_and_calculations() seems to have invalid results. (might ended with failure?)" << std::endl;
			}
			std::ofstream bad_ROC_output_file(bad_ROC_list_save_path);
			for(const auto& bad_ROC_position: bad_ROC_list)
			{
				bad_ROC_output_file << bad_ROC_position.layer << " " << bad_ROC_position.module << " " << bad_ROC_position.ladder << std::endl;
			}
			bad_ROC_output_file.close();
		}

/////////////////////////////////
// Loop with bad ROC exclusion //
/////////////////////////////////

		if(second_loop_requested)
		{
			if(read_bad_ROCs_from_file)
			{
				bad_ROC_list.clear();
				std::ifstream bad_ROC_input_file(bad_ROC_file_input_path);
				if(!(bad_ROC_input_file.is_open()))
				{
					std::cerr << error_prompt << "Error opening the bad ROC input file." << std::endl;
					std::cerr << debug_prompt << "Input path: " << bad_ROC_file_input_path << std::endl; 
					exit(-1);
				}
				while(1)
				{
					std::string line;
					getline(bad_ROC_input_file, line);
					std::stringstream data_stream(line);
					if(bad_ROC_input_file.good())
					{
						int layer  = 0;
						int module = 0;
						int ladder = 0;
						data_stream >> layer >> module >> ladder;
						ROC_position position(layer, module, ladder);
						bad_ROC_list.insert(position);
					}
					else
					{
						break;
					}
				}
			}
			if(bad_ROC_list.size() == 0)
			{
				std::cerr << error_prompt << "The number of filtered bad ROCs is 0." << std::endl;
			}
			int second_loop_and_calculations_results = second_loop(argc, argv, input_file_paths_file_name, second_loop_output_name, schedule, 
			                                                       reweighting_file_path, reweighting_histogram_name, bad_ROC_list, is_mc);
			if(second_loop_and_calculations_results != 0)
			{
				std::cout << error_prompt << "Function analysis_plots_and_calculations() seems to have invalid results. (might ended with failure?)" << std::endl;
			} 
		}

		if(second_analysis_requested)
		{
			int second_loop_analysis_results = second_loop_analysis(argc, argv, second_loop_output_name, second_analysis_output_name);
			if(second_loop_analysis_results != 0)
			{
				std::cout << error_prompt << "Function analysis_plots_and_calculations() seems to have invalid results. (might ended with failure?)" << std::endl;
			} 
		}

/******************************************************************************************************/
/*                                            Terminating                                             */
/******************************************************************************************************/

	}
	catch(const std::exception& e)
	{
		std::cout << error_prompt << e.what() << std::endl;
	}
	return 0; 
}
