#ifndef INPUT_OUTPUT_INTERFACE_H
#define INPUT_OUTPUT_INTERFACE_H

#include "../Console_actor/common_actors.hh"

#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"

#include <iostream>
#include <memory>

extern bool debug;
extern bool debug_prompt_file_input_output;

class Input_output_interface
{
	protected:
		std::string input_file_name;
		std::string output_file_name;
		std::unique_ptr<TFile> input_file;
		std::unique_ptr<TFile> output_file;
		
	public:
		Input_output_interface();
		virtual ~Input_output_interface();

		void open_input_file(std::string filename_p);
		void change_input_file(std::string filename_p);
		void close_input_file();
		const std::string& get_input_file_name();
		const std::string& get_output_file_name();
		void set_output_file(std::string filename_p, std::string options_p = "RECREATE");
		void change_output_file(std::string filename_p, std::string options_p = "RECREATE");
		void set_output_directory(std::string dir_p, std::string options_p = "");
		void close_output_file();
		void save_histogram(TH1* histo_p);
		void save_histogram(TH2* histo_p);
		void save_histogram_as_canvas(TH1* histo_p);
		void save_histogram_as_canvas(TH2* histo_p);
		TObject* get_histogram_from_output(const std::string& name_p);
		TObject* get_histogram_from_input(const std::string& name_p);
};

#endif