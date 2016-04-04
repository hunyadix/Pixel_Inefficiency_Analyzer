#ifndef INSTLUMI_FILL_PARAMETER_H
#define INSTLUMI_FILL_PARAMETER_H

#include "../Fill_parameter_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"
#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>
#include <fstream>
#include <utility>
#include <map>
#include <vector>

extern bool debug;

class Instlumi_fill_parameter : public Fill_parameter
{
	private:
		std::map<std::pair<int, int>, double> instlumi_map;
	public:
		Instlumi_fill_parameter(Ntuple_reader*& ntuple_reader_p);
		~Instlumi_fill_parameter()
		{
			if(debug)
			{
				std::cout << debug_prompt << "~Instlumi_fill_parameter()" << std::endl;
			}
		};
};

// 72, -4.5, 4.5, 42, -10.5, 10.5

Instlumi_fill_parameter::Instlumi_fill_parameter(Ntuple_reader*& ntuple_reader_p)
{
	///////////////
	// Map files //
	///////////////

	std::ifstream map_file_2012("Resources/Sources/Includes/Histogram_generation/Fill_parameters/Instlumi_map/run_ls_instlumi_pileup_2012.txt", std::ifstream::in);
	std::ifstream map_file_2015("Resources/Sources/Includes/Histogram_generation/Fill_parameters/Instlumi_map/run_ls_instlumi_pileup_2015.txt", std::ifstream::in);

	std::vector<std::ifstream*> instlumi_maps;
	instlumi_maps.push_back(&map_file_2012);
	instlumi_maps.push_back(&map_file_2015);

	//////////////////////
	// Creating the map //
	//////////////////////

	for(auto& map_ptr: instlumi_maps)
	{
		std::ifstream& map_file = *map_ptr;
		if(!(map_file.is_open()))
		{
			std::cerr << error_prompt << "Error in Instlumi_fill_parameter(): Error opening map file..." << std::endl;
			exit(-1); 
		}
		while(1)
		{
			std::string line;
			getline(map_file, line);
			std::stringstream data_stream(line);
			if(map_file.good())
			{
				int run;
				int lumisection;
				double instlumi;
				double pileup;
				data_stream >> run >> lumisection >> instlumi >> pileup;
				this -> instlumi_map[std::make_pair(run, lumisection)] = instlumi;
				// std::cout << debug_prompt << this -> instlumi_map[std::make_pair(run, lumisection)] << std::endl;
			}
			else
			{
				break;
			}
		}
		map_file.close();
	}

	///////////////////////////////
	// Fill parameter definition //
	///////////////////////////////

	this -> name = "Instlumi"; 
	this -> nbin = 100; 
	this -> bins = { 0.0, 20.0 };
	this -> fill = [this, &ntuple_reader_p]()
	{
		/////////////////
		// Monte Carlo //
		/////////////////
		
		if(ntuple_reader_p -> get_event_field_ptr() -> run == 1)
		{
			float pileup = ntuple_reader_p -> get_event_field_ptr() -> pileup;
			if(pileup == -9999.0)
			{
				std::cerr << error_prompt << "Warning! Pileup parameter cannot be found for mc (instlumi calculation)." << std::endl;
				ntuple_reader_p -> debug_print_current_input_name();
				return -9999.0;
			}
			// double instlumi = pileup * 0.222;  // 2012
			double instlumi = pileup * 0.3136; // 2015
			// double instlumi = pileup * 0.37935; // pre2016est 2700 bunch
			// double instlumi = pileup * 0.384408; // 2016est 2736 bunch
			return instlumi;
		}

		//////////
		// Data //
		//////////

		else
		{
			int run = ntuple_reader_p -> get_event_field_ptr() -> run;
			int lumisection = ntuple_reader_p -> get_event_field_ptr() -> ls;
			double instlumi = this -> instlumi_map[std::make_pair(run, lumisection)] / 23.3104;
			if(instlumi == 0)
			{
				std::pair<int, int> run_ls_pair = std::make_pair(run, lumisection);
				std::cerr << error_prompt << "Warning! Instlumi parameter cannot be found for run: " << run << " at lumisection: " << lumisection << ". Instlumi is " << this -> instlumi_map[run_ls_pair] << "." << std::endl;
				std::cerr << error_prompt << "Try checking and updating the instlumi and pileup maps!" << std::endl;
				ntuple_reader_p -> debug_print_current_input_name();
				// std::cin.get();
				return -9999.9;
			}
			return instlumi; 
		}
	};
	this -> axis_title = "Instlumi"; 
}

#endif