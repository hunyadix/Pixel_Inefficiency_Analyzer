#ifndef NTUPLE_READER_H
#define NTUPLE_READER_H

#include "Tree_initializer.hh"
#include "../Console_actor/common_actors.hh"
#include "./../SmartHistos/Custom_smart_histos.hh"

#include <iomanip>
#include <sstream>
#include <fstream>
#include <utility>

#include "TThread.h"

extern bool debug;

class Ntuple_reader : public Tree_initializer
{
	private:
		Custom_smart_histos*         histogram_requests;
		std::ostringstream           histogram_requests_output;
		std::vector<std::string>     input_file_path_list;
		int                          schedule = 0;
		bool                         saved_before = false;
		void tree_loop(TTree* tree_p, std::string& tree_name_p);
		void event_tree_loop();
		void lumi_tree_loop();
		void run_tree_loop();
		void traj_tree_loop();
		void clust_tree_loop();
		void save_histogram_list();
	public:
		enum Loop_request_flags
		{
			event_tree_loop_request = 1 << 0,
			lumi_tree_loop_request  = 1 << 1,
			run_tree_loop_request   = 1 << 2,
			traj_tree_loop_request  = 1 << 3,
			clust_tree_loop_request = 1 << 4
		};
		/*inline Loop_request_flags operator | (Loop_request_flags first_p, Loop_request_flags second_p)
		{
			return static_cast<Loop_request_flags>(static_cast<int>(first_p) | static_cast<int>(second_p));
		}*/
		Ntuple_reader();
		Ntuple_reader(std::string filename_p);
		~Ntuple_reader();
		void read_input_paths_from_file(const std::string& input_paths_file_path_p);
		void debug_print_current_input_name() { std::cerr << debug_prompt << this -> get_input_file_name() << std::endl; };
		void set_schedule(int schedule_p);
		void set_histogram_requests(Custom_smart_histos*     histogram_requests_p);
		void run();
		friend void* reader_thread(void* options);
};

struct Thread_data
{
	Ntuple_reader* ntuple_reader;
	Int_t first_entry;
	Int_t past_last_entry;
	std::string tree_name;
	TTree* tree;
};

void* reader_thread(void* options)
{
	Thread_data* thread_data = static_cast<Thread_data*>(options);
	for(Int_t i = thread_data -> first_entry; i < thread_data -> past_last_entry; ++i) 
	{
		TThread::Lock();
		thread_data -> tree -> GetEntry(i);
		thread_data -> ntuple_reader -> histogram_requests -> UpdateRelevantFillParametersAndCuts(thread_data -> tree_name);
		thread_data -> ntuple_reader -> histogram_requests -> Fill(thread_data -> tree_name);
		TThread::UnLock();
	}
	return nullptr;
}

Ntuple_reader::Ntuple_reader()
{

}

Ntuple_reader::Ntuple_reader(std::string filename_p)
{
	this -> open_input_file(filename_p);
}

Ntuple_reader::~Ntuple_reader()
{
	if(debug)
	{
		std::cout << debug_prompt << "~Ntuple_reader()..." << std::endl;
		std::cout << debug_prompt << "SmartHistos messages: " << std::endl;
		std::cout << histogram_requests_output.str();
	}
}

void Ntuple_reader::tree_loop(TTree* tree_p, std::string& tree_name_p)
{
	Int_t n_entries = tree_p -> GetEntries();
	// float percent_done = 0;
	std::cout << std::setw(4) << "  0%";
	std::streambuf *coutbuf = std::cout.rdbuf(); // Saving the old buffer and redirecting it
	std::cout.rdbuf(this -> histogram_requests_output.rdbuf());
	// Only use the std::cerr stream here for debugging!
	if(debug)
	{
		try
		{
			tree_p -> GetEntry(0);
		}
		catch(...)
		{
			std::cerr << error_prompt << "Error reading the tree: " << tree_name_p << ". (permissive)" << std::endl;
			exit(-1);
		}
	}



	std::vector<std::pair<Int_t, Int_t>> thread_start_end_entry_number_pairs;
	Int_t num_threads = 10;
	Int_t section_start = 0;
	Int_t section_length = n_entries / num_threads;
	for(Int_t i = 0; i < num_threads; ++i)
	{
		Int_t begin   = section_start;
		section_start = section_start + section_length;
		Int_t end     = section_start; // after last element
		thread_start_end_entry_number_pairs.push_back(std::make_pair(begin, end));
	}
	thread_start_end_entry_number_pairs.back().second = n_entries;
	std::vector<TThread*> thread_store;
	for(int i = 0; i < num_threads; ++i)
	{
		Thread_data thread_options =
		{
			.ntuple_reader = this,
			.first_entry = thread_start_end_entry_number_pairs[i].first,
			.past_last_entry = thread_start_end_entry_number_pairs[i].second,
			.tree_name = tree_name_p,
			.tree = tree_p
		};
		std::string thread_name("tree_reader_thread_");
		thread_name += i;
		void* data = static_cast<void*>(&thread_options);
		thread_store.push_back(new TThread(thread_name.c_str(), reader_thread, data));
		thread_store.back() -> Run();
	}

	// TThread::Ps();

	for(const auto& thread_pointer: thread_store)
	{
		thread_pointer -> Join();	
	}

	/*for(Int_t i = 0; i < n_entries; ++i) 
	{
		tree_p -> GetEntry(i);
		this -> histogram_requests -> UpdateRelevantFillParametersAndCuts(tree_name_p);
		this -> histogram_requests -> Fill(tree_name_p);
		if(i % 100000 == 0)
		{
			std::cout.rdbuf(coutbuf); // using the default output stream
			percent_done = static_cast<float>(i) / n_entries;
			std::cout << std::string(4, '\b') << std::flush;
			std::cout << std::setw(3) << static_cast<int>(percent_done * 100) << "%" << std::flush;
			std::cout.rdbuf(this -> histogram_requests_output.rdbuf()); // redirect again
		}
	}*/
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
	if(debug)
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



#endif