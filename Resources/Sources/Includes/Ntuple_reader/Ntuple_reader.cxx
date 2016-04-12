#include "Ntuple_reader.hh"

Ntuple_reader::Ntuple_reader()
{

}

Ntuple_reader::Ntuple_reader(std::string filename_p)
{
	this -> open_input_file(filename_p);
}

Ntuple_reader::~Ntuple_reader()
{
	if(debug_show_smarthisto_outputs)
	{
		std::cout << debug_prompt << "~Ntuple_reader()..." << std::endl;
		std::cout << debug_prompt << "SmartHistos messages: " << std::endl;
		std::cout << histogram_requests_output.str();
	}
}

void Ntuple_reader::tree_loop(TTree* tree_p, std::string& tree_name_p)
{
	Int_t n_entries = tree_p -> GetEntries();
	float percent_done = 0;
	std::cout << std::setw(4) << "  0%";
	std::streambuf *coutbuf = std::cout.rdbuf(); // Saving the old buffer and redirecting it
	std::cout.rdbuf(this -> histogram_requests_output.rdbuf());
	// Only use the std::cerr stream here for debugging!
	try
	{
		if(n_entries == 0)
		{
			throw std::runtime_error("No entries found in tree.");
		}
		// std::cerr << debug_prompt << "Entries: " << n_entries << std::endl;
		for(Int_t i = 0; i < n_entries; ++i) 
		{
			// std::cerr << debug_prompt << "getting entry..." << std::endl;
			tree_p -> GetEntry(i);
			// std::cerr << debug_prompt << "updating..." << std::endl;
			this -> histogram_requests -> UpdateRelevantFillParametersPostfixesCuts(tree_name_p);
			// this -> get_relevant_cuts_for_tree(tree_name_p) -> calculate_cuts();
			// Histogram_generation::Cut_factory::get_relevant_cuts_for_tree
			// std::cerr << debug_prompt << "filling..." << std::endl;
			this -> histogram_requests -> Fill(tree_name_p); // do not try moving here :)
			// std::cerr << debug_prompt << "Fill called." << std::endl; std::cin.get();
			if(i % 100000 == 0)
			{
				std::cout.rdbuf(coutbuf); // using the default output stream
				percent_done = static_cast<float>(i) / n_entries;
				std::cout << std::string(4, '\b') << std::flush;
				std::cout << std::setw(3) << static_cast<int>(percent_done * 100) << "%" << std::flush;
				std::cout.rdbuf(this -> histogram_requests_output.rdbuf()); // redirect again
			}
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << error_prompt << e.what() << "(" << tree_name_p << ")" << std::endl;
		exit(-1);
	}
	std::cout.rdbuf(coutbuf);
	std::cout << std::string(4, '\b') << std::flush;
	std::cout << "100%";
}

void Ntuple_reader::event_tree_loop()
{
	this -> prepare_to_run_on_event_tree();
	std::string tree_name = "eventTree";
	this -> tree_loop(this -> event_tree, tree_name);
}

void Ntuple_reader::lumi_tree_loop()
{
	this -> prepare_to_run_on_lumi_tree();
	std::string tree_name = "lumiTree";
	this -> tree_loop(this -> lumi_tree, tree_name);
}

void Ntuple_reader::run_tree_loop()
{
	this -> prepare_to_run_on_run_tree();
	std::string tree_name = "runTree";
	this -> tree_loop(this -> run_tree, tree_name);
}

void Ntuple_reader::traj_tree_loop()
{
	this -> prepare_to_run_on_traj_tree();
	std::string tree_name = "trajTree";
	this -> tree_loop(this -> traj_tree, tree_name);
}

void Ntuple_reader::clust_tree_loop()
{
	this -> prepare_to_run_on_clust_tree();
	std::string tree_name = "clustTree";
	this -> tree_loop(this -> clust_tree, tree_name);
}

void Ntuple_reader::save_histogram_list()
{
	if(!(this -> histogram_requests))
	{
		std::cerr << error_prompt << "Error in Ntuple_reader::save_histogram_list(): unset histogram_requests." << std::endl;
		exit(-1);
	}
	this -> output_file -> cd();
	std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
	std::cout.rdbuf(this -> histogram_requests_output.rdbuf()); //redirect
	this -> histogram_requests -> DrawPlots();
	this -> histogram_requests -> Write();
	std::cout.rdbuf(coutbuf);
	if(this -> saved_before == true)
	{
		std::cerr << error_prompt << "There was multiple Ntuple_reader::save_histogram_list() calls. It should be called only once." << std::endl;
		exit(-1);
	}
	this -> saved_before = true;
}

void Ntuple_reader::read_input_paths_from_file(const std::string& input_paths_file_path_p)
{
	std::ifstream input_paths_file(input_paths_file_path_p, std::ifstream::in);
	if(!(input_paths_file.is_open()))
	{
		std::cerr << error_prompt << "Error opening the file containing the paths of the TimingStudy Ntuples..." << std::endl;
		exit(-1);
	}
	std::string line;
	while(std::getline(input_paths_file, line))
	{
	    std::istringstream iss(line);
	    std::string file_path;
	    if(!(iss >> file_path))
	    {
	    	break;
	    }
	    input_file_path_list.push_back(file_path);
	}
	if(debug_prompt_inputs)
	{
		for(const auto& file_path: this -> input_file_path_list)
		{
			std::cout << debug_prompt << file_path << std::endl;
		}
	}
}

void Ntuple_reader::set_schedule(const int schedule_p)
{
	this -> schedule = schedule_p;
}

void Ntuple_reader::set_histogram_requests(Custom_smart_histos* histogram_requests_p)
{
	this -> histogram_requests = histogram_requests_p;
}

void Ntuple_reader::run()
{
	if(!(this -> histogram_requests))
	{
		std::cerr << error_prompt << "Error in Ntuple_reader::run(): unset histogram_requests." << std::endl;
		exit(-1);
	}
	if(this -> schedule & Loop_request_flags::event_tree_loop_request)
	{
		for(auto it = this -> input_file_path_list.begin(); it != this -> input_file_path_list.end(); ++it)
		{
			const std::string& file_path = *it;
			this -> change_input_file(file_path);
			std::cout << process_prompt << std::left << std::setw(40) << "Looping on tree: event_tree.";
			this -> event_tree_loop();
			std::cout << "   " << it - this -> input_file_path_list.begin() + 1 << "/" << this -> input_file_path_list.size() << std::endl;
		}
	}
	if(this -> schedule & Loop_request_flags::lumi_tree_loop_request)
	{
		for(auto it = this -> input_file_path_list.begin(); it != this -> input_file_path_list.end(); ++it)
		{
			const std::string& file_path = *it;
			this -> change_input_file(file_path);
			std::cout << process_prompt << std::left << std::setw(40) << "Looping on tree: lumi_tree.";
			this -> lumi_tree_loop();
			std::cout << "   " << it - this -> input_file_path_list.begin() + 1 << "/" << this -> input_file_path_list.size() << std::endl;
		}
	}
	if(this -> schedule & Loop_request_flags::run_tree_loop_request)
	{
		for(auto it = this -> input_file_path_list.begin(); it != this -> input_file_path_list.end(); ++it)
		{
			const std::string& file_path = *it;
			this -> change_input_file(file_path);
			std::cout << process_prompt << std::left << std::setw(40) << "Looping on tree: run_tree.";
			this -> run_tree_loop();
			std::cout << "   " << it - this -> input_file_path_list.begin() + 1 << "/" << this -> input_file_path_list.size() << std::endl;
		}
	}
	if(this -> schedule & Loop_request_flags::clust_tree_loop_request)
	{
		for(auto it = this -> input_file_path_list.begin(); it != this -> input_file_path_list.end(); ++it)
		{
			const std::string& file_path = *it;
			this -> change_input_file(file_path);
			std::cout << process_prompt << std::left << std::setw(40) <<  "Looping on tree: clust_tree.";
			this -> clust_tree_loop();
			std::cout << "   " << it - this -> input_file_path_list.begin() + 1 << "/" << this -> input_file_path_list.size() << std::endl;
		}
	}
	if(this -> schedule & Loop_request_flags::traj_tree_loop_request)
	{
		for(auto it = this -> input_file_path_list.begin(); it != this -> input_file_path_list.end(); ++it)
		{
			const std::string& file_path = *it;
			this -> change_input_file(file_path);
			std::cout << process_prompt << std::left << std::setw(40) <<  "Looping on tree: traj_tree.";
			this -> traj_tree_loop();
			std::cout << "   " << it - this -> input_file_path_list.begin() + 1 << "/" << this -> input_file_path_list.size() << std::endl;
		}
	}
	std::cout << process_prompt << std::left << std::setw(45) << "Looping finished." << std::endl;
	std::cout << process_prompt << "Saving histograms: " << std::left << std::setw(30) << "in progress." << std::flush;
	this -> save_histogram_list();
	std::cout << std::string(30, '\b');
	std::cout << std::left << std::setw(30) << "succesful." << std::endl;
}