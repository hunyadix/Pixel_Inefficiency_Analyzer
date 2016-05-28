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

void Ntuple_reader::debug_print_current_input_name() {std::cerr << debug_prompt << this -> get_input_file_name() << std::endl;}
void Ntuple_reader::set_schedule(const int schedule_p) {this -> schedule = schedule_p;}
void Ntuple_reader::set_histogram_requests(Custom_smart_histos* histogram_requests_p) {this -> histogram_requests = histogram_requests_p;}
Custom_smart_histos* Ntuple_reader::get_histogram_requests() {return this -> histogram_requests;}

void Ntuple_reader::conditional_loop_files(bool condition, const std::string& selected_tree_name, const std::function<void()>& selected_tree_loop)
{
	if(condition) {loop_files(selected_tree_name, selected_tree_loop);}
}

void Ntuple_reader::loop_files(const std::string& selected_tree_name, const std::function<void()>& selected_tree_loop)
{
	for(auto file_path_ptr = this -> input_file_path_list.begin(); file_path_ptr != this -> input_file_path_list.end(); ++file_path_ptr)
	{
		this -> change_input_file(*file_path_ptr);
		std::cout << process_prompt << "Looping on tree: " << selected_tree_name << std::left << std::setw(36) << ".";
		selected_tree_loop();
		std::cout << "   " << file_path_ptr - this -> input_file_path_list.begin() + 1 << "/" << this -> input_file_path_list.size() << std::endl;
	}
}

int Ntuple_reader::check_tree_existence(const std::function<TTree*()> tree_preparation_function)
{
	int is_unchanged = 1;
	auto remove_checker = [this, &is_unchanged, &tree_preparation_function] (std::string file_path_ptr)
	{
		this -> change_input_file(file_path_ptr);
		TTree* tree = tree_preparation_function();
		if(!(tree -> GetEntries()))
		{
			std::cerr << error_prompt << "Tree reading error." << std::endl;
			this -> debug_print_current_input_name();
			is_unchanged = 0;
			return 1;
		}
		return 0;
	};
	std::remove_if(input_file_path_list.begin(), input_file_path_list.end(), remove_checker);
	return is_unchanged;
}

int Ntuple_reader::check_tree_existences()
{
	int result = 1;
	if(schedule & Loop_request_flags::event_tree_loop_request) {result &= check_tree_existence(std::bind(&Ntuple_reader::prepare_to_run_on_event_tree, this));}
	if(schedule & Loop_request_flags::lumi_tree_loop_request)  {result &= check_tree_existence(std::bind(&Ntuple_reader::prepare_to_run_on_lumi_tree,  this));}
	if(schedule & Loop_request_flags::run_tree_loop_request)   {result &= check_tree_existence(std::bind(&Ntuple_reader::prepare_to_run_on_run_tree,   this));}
	if(schedule & Loop_request_flags::clust_tree_loop_request) {result &= check_tree_existence(std::bind(&Ntuple_reader::prepare_to_run_on_clust_tree, this));}
	if(schedule & Loop_request_flags::traj_tree_loop_request)  {result &= check_tree_existence(std::bind(&Ntuple_reader::prepare_to_run_on_traj_tree,  this));}
	return result;
}

void Ntuple_reader::tree_loop(std::function<TTree*()>& tree_preparation_function, const std::string& tree_name)
{
	TTree* tree = tree_preparation_function(); 
	Int_t n_entries = tree -> GetEntries();
	float percent_done = 0;
	std::cout << std::setw(4) << "  0%";
	std::streambuf *coutbuf = std::cout.rdbuf(); // Saving the old buffer and redirecting it
	std::cout.rdbuf(this -> histogram_requests_output.rdbuf());
///////////////////////////////////////////////////////
// Only use the std::cerr stream here for debugging! //
///////////////////////////////////////////////////////
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
			tree -> GetEntry(i);
			// std::cerr << debug_prompt << "updating..." << std::endl;
			this -> histogram_requests -> UpdateRelevantFillParametersPostfixesCuts(tree_name);
			// std::cerr << debug_prompt << "filling..." << std::endl;
			this -> histogram_requests -> Fill(tree_name); // do not try moving here :)
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
		std::cerr << error_prompt << e.what() << "(" << tree_name << ")" << std::endl;
		exit(-1);
	}
	std::cout.rdbuf(coutbuf);
	std::cout << std::string(4, '\b') << std::flush;
	std::cout << "100%";
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

void Ntuple_reader::run()
{
	using std::bind;
	if(!(this -> histogram_requests))
	{
		std::cerr << error_prompt << "Error in Ntuple_reader::run(): unset histogram_requests." << std::endl;
		exit(-1);
	}
	if(!(this -> check_tree_existences()))
	{
		std::cerr << error_prompt << "Error reading the trees: Some of the trees were removed from the query." << std::endl;
		// exit(-1);
	}
	auto event_tree_loop = bind(&Ntuple_reader::tree_loop, this, static_cast<std::function<TTree*()>>(bind(&Ntuple_reader::prepare_to_run_on_event_tree, this)), "eventTree");
	auto lumi_tree_loop  = bind(&Ntuple_reader::tree_loop, this, static_cast<std::function<TTree*()>>(bind(&Ntuple_reader::prepare_to_run_on_lumi_tree, this)),  "lumiTree");
	auto run_tree_loop   = bind(&Ntuple_reader::tree_loop, this, static_cast<std::function<TTree*()>>(bind(&Ntuple_reader::prepare_to_run_on_run_tree, this)),   "runTree");
	auto clust_tree_loop = bind(&Ntuple_reader::tree_loop, this, static_cast<std::function<TTree*()>>(bind(&Ntuple_reader::prepare_to_run_on_clust_tree, this)), "clustTree");
	auto traj_tree_loop  = bind(&Ntuple_reader::tree_loop, this, static_cast<std::function<TTree*()>>(bind(&Ntuple_reader::prepare_to_run_on_traj_tree, this)),  "trajTree");
	// Tree names do not matter here
	this -> conditional_loop_files(schedule & Loop_request_flags::event_tree_loop_request, "event_tree", event_tree_loop);
	this -> conditional_loop_files(schedule & Loop_request_flags::lumi_tree_loop_request,  "lumi_tree",  lumi_tree_loop);
	this -> conditional_loop_files(schedule & Loop_request_flags::run_tree_loop_request,   "run_tree",   run_tree_loop);
	this -> conditional_loop_files(schedule & Loop_request_flags::clust_tree_loop_request, "clust_tree", clust_tree_loop);
	this -> conditional_loop_files(schedule & Loop_request_flags::traj_tree_loop_request,  "traj_tree",  traj_tree_loop);
	std::cout << process_prompt << std::left << std::setw(45) << "Looping finished." << std::endl;
	std::cout << process_prompt << "Saving histograms: " << std::left << std::setw(30) << "in progress." << std::flush;
	this -> save_histogram_list();
	std::cout << std::string(30, '\b');
	std::cout << std::left << std::setw(30) << "succesful." << std::endl;
}