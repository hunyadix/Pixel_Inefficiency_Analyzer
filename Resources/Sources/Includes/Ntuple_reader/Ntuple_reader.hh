#ifndef NTUPLE_READER_H
#define NTUPLE_READER_H

#include "Tree_initializer.hh"
#include "../Console_actor/common_actors.hh"
#include "./../SmartHistos/Custom_smart_histos.hh"

#include <iomanip>
#include <sstream>
#include <fstream>
#include <functional>
#include <stdexcept>

extern bool debug;
extern bool debug_show_smarthisto_outputs;
extern bool debug_prompt_inputs;

class Ntuple_reader : public Tree_initializer
{
	private:
		Custom_smart_histos*         histogram_requests;
		std::ostringstream           histogram_requests_output;
		std::vector<std::string>     input_file_path_list;
		int                          schedule = 0;
		bool                         saved_before = false;
		void conditional_loop_files(bool condition, const std::string& selected_tree_name, const std::function<void()>& selected_tree_loop);
		void loop_files(const std::string& selected_tree_name, const std::function<void()>& selected_tree_loop);
		void tree_loop(TTree* tree_p, const std::string& tree_name_p);
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
		void set_schedule(int schedule_p);
		void set_histogram_requests(Custom_smart_histos* histogram_requests_p);
		Custom_smart_histos* get_histogram_requests();
		void read_input_paths_from_file(const std::string& input_paths_file_path_p);
		void debug_print_current_input_name();
		void run();
};

#endif