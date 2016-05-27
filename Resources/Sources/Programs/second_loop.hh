#ifndef SECOND_LOOP_H
#define SECOND_LOOP_H

#include "../Includes/Ntuple_reader/Ntuple_reader.hh"

#include "../Includes/Console_actor/common_actors.hh"

#include "../Includes/Ntuple_reader/ntuple_reader_setup.hh"
#include "../Includes/ROC_position/ROC_position.hh"

#include <iostream>
#include <iomanip>
#include <string>
#include <set>

#ifndef NOVAL_I
#define NOVAL_I -9999
#endif

int second_loop(int argc, char** argv, const std::string& second_loop_input_paths_p, const std::string& second_loop_output_path_p, const int& schedule_p, const std::string& reweighting_file_path, const std::string& reweighting_histogram_name, const std::set<ROC_position> bad_ROC_list_p, bool is_mc_p)
{
	std::string process_name(argv[0]);
	if(process_name.find("./", 0, 2) != std::string::npos)
	{
		process_name = process_name.substr(process_name.find("./", 0, 2) + 2);
	}
	std::unique_ptr<Timer> timer(new Timer_colored(timer_prompt));
	std::cout << process_prompt << process_name << " is now running the second loop..." << std::endl;
	timer -> restart("Preparing to run tree loops (post_bad_ROC_map)...");

	Ntuple_reader* ntuple_reader = new Ntuple_reader();

	ntuple_reader -> read_input_paths_from_file(second_loop_input_paths_p.c_str());
	ntuple_reader -> set_output_file(second_loop_output_path_p.c_str(), "RECREATE");

	////////////////////////////////////////////
	// Using the schedule defined in main.cxx //
	////////////////////////////////////////////

	ntuple_reader -> set_schedule(schedule_p);

	//////////////////////////////////////
	// bad_ROC_cut_function declaration //
	//////////////////////////////////////

	TAxis* module_axis = new TAxis(72, -4.5, 4.5);
	TAxis* ladder_axis = new TAxis(90, -22.5, 22.5);

	auto bad_ROC_cut_function = [&ntuple_reader, &bad_ROC_list_p, &module_axis, &ladder_axis] ()
	{
		int layer_number = ntuple_reader -> get_module_field_ptr() -> layer;
		int module_bin = -1;
		int ladder_bin = -1;
		int traj_mod_det  = ntuple_reader -> get_traj_field_ptr()    -> mod_on.det;
		int clust_mod_det = ntuple_reader -> get_cluster_field_ptr() -> mod_on.det;
		// std::cerr << debug_prompt << "traj_mod_det: " << traj_mod_det << ", clust_mod_det:" << clust_mod_det << std::endl; 
		if(traj_mod_det == 0)
		{
			// std::cerr << debug_prompt << "Traj_mod path." << std::endl;
			double module_fill_value = Barrel_ROC::get_module_fill_value_from_local_y(ntuple_reader -> get_traj_field_ptr() -> ly, *(ntuple_reader -> get_module_field_ptr()));
			double ladder_fill_value = Barrel_ROC::get_ladder_fill_value_from_local_x(ntuple_reader -> get_traj_field_ptr() -> lx, *(ntuple_reader -> get_module_field_ptr()));
			module_bin = module_axis -> FindFixBin(module_fill_value);
			ladder_bin = ladder_axis -> FindFixBin(ladder_fill_value);
		}
		else
		{
			if(clust_mod_det == 0)
			{
				// std::cerr << debug_prompt << " Clust mod path. " << std::endl;
				double module_fill_value = Barrel_ROC::get_module_fill_value_from_cluster_xy(ntuple_reader -> get_cluster_field_ptr() -> x, ntuple_reader -> get_cluster_field_ptr() -> y, *(ntuple_reader -> get_module_field_ptr()));
				double ladder_fill_value = Barrel_ROC::get_ladder_fill_value_from_cluster_xy(ntuple_reader -> get_cluster_field_ptr() -> x, ntuple_reader -> get_cluster_field_ptr() -> y, *(ntuple_reader -> get_module_field_ptr()));
				module_bin = module_axis -> FindFixBin(module_fill_value);
				ladder_bin = ladder_axis -> FindFixBin(ladder_fill_value);
			}
		}
		// std::cerr << debug_prompt << "Query results: module_bin: " << module_bin << ", ladder bin: " << ladder_bin << "." << std::endl; 
		// std::cin.get();
		if(module_bin == -1.0 && ladder_bin == -1.0)
		{
			return false;
		}
		if(bad_ROC_list_p.find({ .layer = layer_number, .module = module_bin, .ladder = ladder_bin }) != bad_ROC_list_p.end())
		{
			return false;
		}
		return true; 
	};

	const char* bad_ROC_cut_name = "bad_ROC_filtering";
	// const char* bad_ROC_cut_name = ""; // Debug mode: turn off any cuts + improve performance while doing so

	std::unique_ptr<Histogram_generation::Cut> bad_ROC_cut(new Histogram_generation::Cut);
	bad_ROC_cut -> set_name(bad_ROC_cut_name);
	bad_ROC_cut -> set_cut_function(bad_ROC_cut_function);

	///////////////////////////////////////////////
	// Setting up the ntuple reader's parameters //
	///////////////////////////////////////////////

	std::shared_ptr<Custom_smart_histos> histogram_requests(new Custom_smart_histos);
	std::shared_ptr<Histogram_reweighter> histogram_reweighter(new Histogram_reweighter(ntuple_reader));
	histogram_requests -> AddNewCut(bad_ROC_cut);
	Ntuple_reader_setup_options options = 
	{
		.ntuple_reader              = ntuple_reader,
		.histogram_requests         = histogram_requests,
		.histogram_reweighter       = histogram_reweighter,
		.extra_cut                  = bad_ROC_cut_name,
		.reweighting_file_path      = reweighting_file_path, 
		.reweighting_histogram_name = reweighting_histogram_name, 
		.is_mc                      = is_mc_p
	};
	int setup_results = ntuple_reader_setup(options);
	if(setup_results != 0)
	{
		std::cerr << error_prompt << "The ntuple_reader_setup() function failed." << std::endl;
	}

	timer -> print_seconds("Took about ", " second(s).");
	timer -> restart("Running tree loops (with bad_ROC_cut...)");
	ntuple_reader -> run();
	timer -> print_seconds("Took about ", " second(s).");

	//delete ntuple_reader;
	return 0;
}

#endif