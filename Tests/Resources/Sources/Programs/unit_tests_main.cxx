/******************************************************************************************************/
/*                                                                                                    */
/*                      Unit test for Pixel Inefficiency Analyzer.                                    */
/*                      Results confirmed using the PixelHistoMaker of TimingStudy                    */
/*                      Date: 2015. CMSSW version: CMSSW_7_5_0                                        */
/*                      Author: Adam Hunyadi                                                          */
/*                      E-mail: hunyadix@gmail.com                                                    */
/*                                                                                                    */
/******************************************************************************************************/

#define EVT_LOOP 0
#define TRAJ_LOOP 1
#define CLUST_LOOP 0
#define NTHFILE 1

// TimingStudy versions
#define VER 5
#if VER == 5
#define SPLIT 0
#define TREEREADER_VER 38
#define DATASTRUCT_VER 36
#endif

#define MAX_PU 100
#define PU_REWEIGHT_MC 1

//#define COMPLETE 0

#include "../../../../References/TreeLooper.h"

#include "/data/jkarancs/CMSSW/TimingStudy/CMSSW_7_1_0_pre1/src/DPGAnalysis/PixelTimingStudy/test/PixelHistoMaker/modifications.h"
#include "/data/jkarancs/CMSSW/TimingStudy/CMSSW_7_1_0_pre1/src/DPGAnalysis/PixelTimingStudy/test/PixelHistoMaker/lowlumi_runs_2011.txt"
#include "/data/jkarancs/CMSSW/TimingStudy/CMSSW_7_1_0_pre1/src/DPGAnalysis/PixelTimingStudy/test/PixelHistoMaker/run_203002.txt"

#define EVENT_TREE_STRUCTURE_H
#define LUMINOSITY_TREE_STRUCTURE_H
#define CLUSTER_H
#define CLUSTER_STRUCTURE_H
#define DIGI_H
#define MODULE_STRUCTURE_H
#define RUN_STRUCTURE_H
#define TRACK_STRUCTURE_H
#define TRAJ_MEASUREMENT_H
#define TRAJ_MEASUREMENT_STRUCTURE_H

#define SMARTHISTOS_H
#define SCAN_POINTS_H

#include "../../../../Resources/Sources/Includes/Ntuple_reader/Ntuple_reader.hh"
#include "../../../../Resources/Sources/Includes/Console_actor/common_actors.hh"
#include "../../../../Resources/Sources/Includes/Timer/Timer_colored.hh"

#include "../../../../Resources/Sources/Includes/Histogram_generation/Postfixes/Postfix_factory.hh"
#include "../../../../Resources/Sources/Includes/Histogram_generation/Fill_parameters/Fill_parameter_factory.hh"
#include "../../../../Resources/Sources/Includes/Histogram_generation/Cuts/SmartHisto_cut_factory.hh"

#include "../../../../Resources/Sources/Includes/Histogram_generation/Root_color_sets/root_color_sets.hh"

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

bool debug = true;

//////////////////
// Test options //
//////////////////

struct Test_options
{
	bool add_cuts;
};

////////////////////
// Test functions //
////////////////////

void generate_efficiency_cuts_PixelHistoMaker(const std::string& input_file_paths, const std::string& output_file_path, Test_options options);
void generate_efficiency_cuts_PixelIneffAnalyzer(const std::string& input_file_paths, const std::string& output_file_path, Test_options options);
void test_efficiency_cuts(const std::string& input_file_paths, const std::string& output_file_path);

int main(int argc, char** argv)
{
	try
	{
		///////////////////
		// Project setup //
		///////////////////

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
			std::cout << process_prompt << "Current working directory: " << buffer << std::endl;
		}

		////////////////////
		// File variables //
		////////////////////

		std::string input_file_paths_file_name;
		std::string efficiency_cuts_test_output_path;

		/////////////////////
		// Data file paths //
		/////////////////////

		input_file_paths_file_name       = "Data/samples_dcolscan_dynIneff_fill_2015_CMSSW76X_paths.txt";
		//input_file_paths_file_name       = "Data/debug_paths.txt";
		efficiency_cuts_test_output_path = "Results/efficiency_cuts_test_output.root";

		//////////////////////
		// Performing tests //
		//////////////////////

		test_efficiency_cuts(input_file_paths_file_name, efficiency_cuts_test_output_path);

	}
	catch(const std::exception& e)
	{
		std::cout << error_prompt << e.what() << std::endl;
	}
	return 0; 
}

void generate_efficiency_cuts_PixelHistoMaker(const std::string& input_file_paths, const std::string& output_file_path, Test_options options)
{
	// int argc = 1;
	// char* argv[] = { (char*)("./PixelHistoMaker") };

	RunData run;
	LumiData lumi;
	EventData e;
	TrajMeasurement t;
	Cluster c;

	Variables v;
	TreeReader tr(&run,&lumi,&e,&e,&t,&e,&c);
	SmartHistos sh;
	
	if(EVT_LOOP)
	{
		sh.AddHistoType("evt");
	}
	if(TRAJ_LOOP)
	{
		sh.AddHistoType("traj");
	}
	if(CLUST_LOOP)
	{
		sh.AddHistoType("clust");
	}

	TreeLooper looper(&tr, &v, EVT_LOOP, TRAJ_LOOP, CLUST_LOOP); // Set these in the beginning of this file

	std::unique_ptr<Timer_colored> timer(new Timer_colored(timer_prompt));

	///////////////
	// Postfixes //
	///////////////

	sh.AddNewPostfix("Layers", [&v](){return v.pf_layers;}, "Lay[1to3]", "L[1to3]", col3_red_to_blue);
	
	/////////////////////
	// Fill parameters //
	/////////////////////

	sh.AddSpecial({.name = "HitEfficiency",  .name_plus_1d = "ValidHit", .axis = "Hit Efficiency",  .axis_plus_1d = "Valid Hit"});

	sh.AddNewFillParam("LayersParam",   {.nbin = 3, .bins = { 0.5, 3.5}, .fill = [&v](){return v.pf_layers + 1;}, .axis_title = "Layers"});
	sh.AddNewFillParam("HitEfficiency", {.nbin = 2, .bins = {-0.5, 1.5}, .fill = [&t](){return t.validhit;     }, .axis_title = "Hit Efficiency"});
	sh.AddNewFillParam("LaddersParam",          { .nbin=  45, .bins={  -22.5,   22.5}, .fill=[&v](){ return v.ladder;          }, .axis_title="Ladders"});
	sh.AddNewFillParam("ModulesParam",          { .nbin=   9, .bins={   -4.5,    4.5}, .fill=[&v](){ return v.module;          }, .axis_title="Modules"});

	//////////
	// Cuts //
	//////////

	sh.AddNewCut("EffCuts", [&v](){ return v.effcut_all; });

	////////////////////////
	// Customise the test //
	////////////////////////

	std::vector<std::string> added_cuts = {};
	if(options.add_cuts)
	{
		added_cuts.push_back("EffCuts");
	}

	///////////////////////////
	// Histogram definitions //
	///////////////////////////

	sh.AddHistos("traj", {.fill = "HitEfficiency_vs_LayersParam",                  .pfs = {}, .cuts = added_cuts, .draw = "LPE1", .opt = "", .ranges = {0,0}});
	sh.AddHistos("traj", {.fill = "HitEfficiency_vs_ModulesParam",                 .pfs = {}, .cuts = added_cuts, .draw = "PE1",  .opt = "", .ranges = {0,0, 0.98,1}});
	sh.AddHistos("traj", {.fill = "HitEfficiency_vs_LaddersParam",                 .pfs = {}, .cuts = added_cuts, .draw = "PE1",  .opt = "", .ranges = {0,0, 0.98,1}});
	sh.AddHistos("traj", {.fill = "HitEfficiency_vs_LaddersParam_vs_ModulesParam", .pfs = {}, .cuts = added_cuts, .draw = "COLZ", .opt = "", .ranges = {0,0, 0,0, 0.97,1}});
	sh.AddHistos("traj", {.fill = "LaddersParam_vs_ModulesParam",                  .pfs = {}, .cuts = added_cuts, .draw = "COLZ", .opt = "", .ranges = {0,0, 0,0, 0.97,1}});

	std::function<void()> add_special_histos = [&sh](){};
	sh.SetHistoWeights({});
	
	std::cout << "-----------------------------------------------------------------\n";
	std::cout << "Creating the following plots:\n"; 
	sh.PrintNames();
	std::cout << "-----------------------------------------------------------------\n";

	std::ifstream input_paths_file(input_file_paths, std::ifstream::in);
	std::string line;
	while(std::getline(input_paths_file, line))
	{
		std::istringstream iss(line);
		std::string file_path;
		if(!(iss >> file_path))
		{
			break;
		}
		looper.AddFile(file_path.c_str());
	}
	input_paths_file.close();

	///////////////////
	// Running loops //
	///////////////////

	timer -> restart("Running loops in PixelHistoMaker ...");
	looper.LoopOnTrees(&sh, add_special_histos, 0);

	// Write histograms and canvases in an output file
	TFile* f_out = new TFile(output_file_path.c_str(),"RECREATE"); 
	sh.DrawPlots();
	sh.Write();
	timer -> print_seconds("Took about ", " second(s).");
	std::cout << "Writing plots to file: " << f_out -> GetName() << " done.\n";
	f_out -> Close();
}

void generate_efficiency_cuts_PixelIneffAnalyzer(const std::string& input_file_paths, const std::string& output_file_path, Test_options options)
{
	using Histogram_generation::Cut_factory;

	Ntuple_reader* ntuple_reader = new Ntuple_reader();

	int schedule = // Ntuple_reader::Loop_request_flags::event_tree_loop_request |
				   // Ntuple_reader::Loop_request_flags::lumi_tree_loop_request  |
				   // Ntuple_reader::Loop_request_flags::run_tree_loop_request   |
				   // Ntuple_reader::Loop_request_flags::clust_tree_loop_request |
				   Ntuple_reader::Loop_request_flags::traj_tree_loop_request  |
				   0;

	bool is_mc = true;

	ntuple_reader -> read_input_paths_from_file(input_file_paths);
	ntuple_reader -> set_output_file(output_file_path, "UPDATE");

	ntuple_reader -> set_schedule(schedule);

	std::shared_ptr<Custom_smart_histos> histogram_requests(new Custom_smart_histos);
	ntuple_reader -> set_histogram_requests(histogram_requests.get());

	histogram_requests -> AddHistoType("eventTree");
	histogram_requests -> AddHistoType("lumiTree");
	histogram_requests -> AddHistoType("runTree");
	histogram_requests -> AddHistoType("clustTree");
	histogram_requests -> AddHistoType("trajTree");

	/////////////////////
	// Fill parameters //
	/////////////////////

	histogram_requests -> AddSpecial({.name = "HitEfficiency", .name_plus_1d = "ValidHit", .axis = "Hit Efficiency", .axis_plus_1d = "Valid Hit"});

	histogram_requests -> AddNewFillParam(Fill_parameter_factory::get_fill_parameter("HitEfficiency", ntuple_reader, is_mc));
	histogram_requests -> AddNewFillParam(Fill_parameter_factory::get_fill_parameter("Layers", ntuple_reader, is_mc));
	histogram_requests -> AddNewFillParam(Fill_parameter_factory::get_fill_parameter("Ladders", ntuple_reader, is_mc));
	histogram_requests -> AddNewFillParam(Fill_parameter_factory::get_fill_parameter("Modules", ntuple_reader, is_mc));

	///////////////
	// Postfixes //
	///////////////

	histogram_requests -> AddNewPostfix(Postfix_factory::get_postfix("Layers", ntuple_reader));

	//////////
	// Cuts //
	//////////

	histogram_requests -> AddNewCut(Histogram_generation::Cut_factory::get_cut("effcut_all", ntuple_reader)); 
	histogram_requests -> AddNewCut(Histogram_generation::Cut_factory::get_cut("", ntuple_reader));

	////////////////////////
	// Customise the test //
	////////////////////////

	std::vector<std::string> added_cuts = {};
	if(options.add_cuts)
	{
		added_cuts.push_back("effcut_all");
	}

	///////////////////////////
	// Histogram definitions //
	///////////////////////////

	histogram_requests -> AddHistos("trajTree", {.fill = "HitEfficiency_vs_Layers",             .pfs = {}, .cuts = added_cuts , .draw = "LPE1", .opt = "", .ranges = {0.0, 0.0}});
	histogram_requests -> AddHistos("trajTree", {.fill = "HitEfficiency_vs_Modules",            .pfs = {}, .cuts = added_cuts , .draw = "PE1",  .opt = "", .ranges = {0,0, 0.98,1}});
	histogram_requests -> AddHistos("trajTree", {.fill = "HitEfficiency_vs_Ladders",            .pfs = {}, .cuts = added_cuts , .draw = "PE1",  .opt = "", .ranges = {0,0, 0.98,1}});
	histogram_requests -> AddHistos("trajTree", {.fill = "HitEfficiency_vs_Ladders_vs_Modules", .pfs = {}, .cuts = added_cuts , .draw = "COLZ", .opt = "", .ranges = {0,0, 0,0, 0.97,1}});
	histogram_requests -> AddHistos("trajTree", {.fill = "Ladders_vs_Modules",                  .pfs = {}, .cuts = added_cuts , .draw = "COLZ", .opt = "", .ranges = {0,0, 0,0, 0.97,1}});

	///////////////////
	// Running loops //
	///////////////////

	std::unique_ptr<Timer_colored> timer(new Timer_colored(timer_prompt));
	
	timer -> restart("Running loops in Pixel Inefficiency Analyzer...");
	ntuple_reader -> run();
	timer -> print_seconds("Took about ", " second(s).");

	delete ntuple_reader;
}

void test_efficiency_cuts(const std::string& input_file_paths, const std::string& output_file_path)
{
	Test_options no_cuts_default_options =
	{
		.add_cuts = true
	};
	
	generate_efficiency_cuts_PixelHistoMaker(input_file_paths, output_file_path, no_cuts_default_options);
	generate_efficiency_cuts_PixelIneffAnalyzer(input_file_paths, output_file_path, no_cuts_default_options);
}
