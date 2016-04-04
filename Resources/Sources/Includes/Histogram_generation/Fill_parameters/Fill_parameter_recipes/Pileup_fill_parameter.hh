#ifndef PILEUP_FILL_PARAMETER_H
#define PILEUP_FILL_PARAMETER_H

#include "../Fill_parameter_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"
#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>
#include <fstream>
#include <utility>
#include <map>

extern bool debug;

class Pileup_fill_parameter : public Fill_parameter
{
	private:
		std::map<std::pair<int, int>, double> pileup_map;
	public:
		Pileup_fill_parameter(Ntuple_reader*& ntuple_reader_p);
		~Pileup_fill_parameter()
		{
			if(debug)
			{
				std::cout << debug_prompt << "~Pileup_fill_parameter()" << std::endl;
			}
		};
};

//72, -4.5, 4.5, 42, -10.5, 10.5

Pileup_fill_parameter::Pileup_fill_parameter(Ntuple_reader*& ntuple_reader_p)
{
	///////////////
	// Map files //
	///////////////

	std::ifstream map_file_2012("Resources/Sources/Includes/Histogram_generation/Fill_parameters/Instlumi_map/run_ls_instlumi_pileup_2012.txt", std::ifstream::in);
	std::ifstream map_file_2015("Resources/Sources/Includes/Histogram_generation/Fill_parameters/Instlumi_map/run_ls_instlumi_pileup_2015.txt", std::ifstream::in);

	std::vector<std::ifstream*> pileup_maps;
	pileup_maps.push_back(&map_file_2012);
	pileup_maps.push_back(&map_file_2015);

	//////////////////////
	// Creating the map //
	//////////////////////

	for(auto& map_ptr: pileup_maps)
	{
		std::ifstream& map_file = *map_ptr;
		if(!(map_file.is_open()))
		{
			std::cerr << error_prompt << "Error in Pileup_fill_parameter(): Error opening map file..." << std::endl;
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
				this -> pileup_map[std::make_pair(run, lumisection)] = pileup;
				//std::cout << debug_prompt << this -> pileup_map[std::make_pair(run, lumisection)] << std::endl;
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

	this -> name = "Pileup"; 
	this -> nbin = 100; 
	this -> bins = { 0.0, 100.0 };
	this -> fill = [this, &ntuple_reader_p]()
	{
		/////////////////
		// Monte Carlo //
		/////////////////
		if(ntuple_reader_p -> get_event_field_ptr() -> run == 1)
		{
			double pileup = ntuple_reader_p -> get_event_field_ptr() -> pileup;
			if(pileup == -9999.0)
			{
				std::cerr << error_prompt << "Warning! Pileup parameter cannot be found for mc (pileup calculation)." << std::endl;
				ntuple_reader_p -> debug_print_current_input_name();
				return -9999.0;
			}
			return pileup; 
		}
		
		//////////
		// Data //
		//////////
		else
		{
			int run = ntuple_reader_p -> get_event_field_ptr() -> run;
			int lumisection = ntuple_reader_p -> get_event_field_ptr() -> ls;
			double pileup = this -> pileup_map[std::make_pair(run, lumisection)];
			if(pileup == 0)
			{
				std::pair<int, int> run_ls_pair = std::make_pair(run, lumisection);
				std::cerr << error_prompt << "Warning! Pileup parameter cannot be found for run: " << run << " at lumisection: " << lumisection << ". Instlumi is " << this -> pileup_map[run_ls_pair] << "." << std::endl;
				std::cerr << error_prompt << "Try checking and updating the instlumi and pileup maps!" << std::endl;
				ntuple_reader_p -> debug_print_current_input_name();
				return -9999.9;
			}
			return pileup;
		}
	};
	this -> axis_title = "Pileup"; 
}

#endif