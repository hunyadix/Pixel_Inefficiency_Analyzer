/******************************************************************************************************/
/*                                                                                                    */
/*                      Program to link output files from split input analysis.                       */
/*                      Date: 2016. CMSSW version: CMSSW_80x                                          */
/*                      Author: Adam Hunyadi                                                          */
/*                      E-mail: hunyadix@gmail.com                                                    */
/*                                                                                                    */
/******************************************************************************************************/

#include "../Includes/SmartHistos/Custom_smart_histos.hh"
#include "../Includes/Console_actor/common_actors.hh"

#include "../Includes/Ntuple_reader/ntuple_reader_setup.hh"

#include "TFile.h"

#include <string>
#include <vector>
#include <memory>

bool debug                            = false;
bool debug_show_smarthisto_outputs    = true;
bool debug_prompt_inputs              = true;
bool debug_prompt_file_input_output   = false;
bool debug_prompt_destructors         = true;
bool debug_prompt_histogram_saving    = true;
bool debug_customsmarthisto_inserting = false;
bool debug_customsmarthisto_updating  = false;

int main(int argc, char** argv)
{
	using Histogram_generation::Cut_factory;
	std::vector<std::string> input_files_to_link =
	{
		"Results/2012/complex_step_1.root"
	};

	Ntuple_reader* ntuple_reader = new Ntuple_reader();
	std::shared_ptr<Custom_smart_histos>  histogram_requests(new Custom_smart_histos);
	std::shared_ptr<Histogram_reweighter> histogram_reweighter(new Histogram_reweighter(ntuple_reader));
	histogram_requests ->                 AddNewCut(Cut_factory::get_cut("", ntuple_reader));

	Ntuple_reader_setup_options options = 
	{
		.ntuple_reader              = ntuple_reader,
		.histogram_requests         = histogram_requests,
		.histogram_reweighter       = histogram_reweighter,
		.extra_cut                  = "",
		.reweighting_file_path      = "", 
		.reweighting_histogram_name = "", 
		.is_mc                      = false
	};
	
	ntuple_reader_setup(options);

	for(const auto& filename: input_files_to_link)
	{
		histogram_requests -> Add(filename);
		std::cerr << debug_prompt << "Results in \"" << filename << "\" have been merged into the output_file." << std::endl;
	}

	// auto output_file = std::unique_ptr<TFile>(new TFile("Results/reweighting_step_1.root", "RECREATE"));
	TFile* output_file = new TFile("Results/2012/step_1_fixed.root", "RECREATE");
	if(!(output_file -> IsOpen()))
	{
		std::cerr << error_prompt << "Error opening the output file." << std::endl;
	}
	
	histogram_requests -> Write();
	histogram_requests -> DrawPlots();

	output_file -> Close();
	delete output_file;
	return 0; 
}
