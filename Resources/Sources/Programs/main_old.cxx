#include "../Includes/Ntuple_reader/Ntuple_reader.hh"
#include "../Includes/Timer/Timer_colored.hh"
#include "../Includes/Console_colors/Console_colors.hh"
#include "../Includes/Console_actor/Console_actor.hh"
#include "../Includes/Console_actor/common_actors.hh"

#include "TCanvas.h"
#include "TPad.h"
#include "TH2.h"
#include "TFile.h"
#include "TTree.h"

#include <iostream>
#include <string>
#include <sstream>


int main( int argc, char** argv )
{

	/****************************************************************************************************
	 *                                      Setting up the project                                      *
	 ****************************************************************************************************/

	system( "clear" );
	std :: string process_name( argv[0] );
	if( process_name.find( "./", 0, 2 ) != std :: string :: npos )
	{
		process_name = process_name.substr( process_name.find( "./", 0, 2 ) + 2 );
	}
	std :: cout << process_prompt << process_name << " is being executed..." << std :: endl;
	Timer* timer = new Timer_colored( timer_prompt );

	/****************************************************************************************************
	 *                                    Initializing the data files                                   *
	 ****************************************************************************************************/
	
	std :: string input_file_name  = "./Data/Ntuple.root";
	std :: string output_file_name = "./Results/test.root";
	
	Ntuple_reader* ntuple_reader = new Ntuple_reader( input_file_name );
	ntuple_reader -> set_output_file( output_file_name, "RECREATE" );

	/****************************************************************************************************
	 *                                       Configuring the run                                        *
	 ****************************************************************************************************/

	timer -> restart( "configuring parameters..." );

	/*__________________________________________1D Histograms__________________________________________*/

	ntuple_reader -> request_histogram( "eventTree", "ntracks", "Number of tracks in events", "Number of tracks", 200, 0, 200 );
	ntuple_reader -> request_histogram( "trajTree",  "validhit_vs_module", "Valid and invalid hits on modules", "Module number;0: invalid hit, 1: valid hit", 10, -5, 5, 2, 0, 2 );
	ntuple_reader -> request_histogram( "clustTree", "Z_ROC_occupancy",   "Total ROC occupancy for ladders on layer 1", "ROC number", 65, 0, 65 );
	ntuple_reader -> request_histogram( "clustTree", "PHI_ROC_occupancy", "Total ROC occupancy for disks on layer 1",   "ROC number", 37, 0, 37 );
	ntuple_reader -> request_histogram( "clustTree", "detID_list", "DetID list", "DetID;", 601, -300, 300 );

	/*__________________________________________2D Histograms__________________________________________*/

	ntuple_reader -> request_histogram( "clustTree", "cluster_occupancy", "Clusters in ROCs", "Z direction;PHI direction", 65, 0, 65, 37, 0, 37 );
	ntuple_reader -> request_histogram( "clustTree", "ROC_ID_map", "ROC_ID of given ROC", "Z direction;PHI direction", 65, 0, 65, 37, 0, 37 );
	ntuple_reader -> request_histogram( "trajTree", "ROC_total_hits", "Total hits in ROCs", "Z direction;PHI direction", 65, 0, 65, 37, 0, 37 );
	ntuple_reader -> request_histogram( "trajTree", "ROC_validhits", "Valid hits in ROCs", "Z direction;PHI direction", 65, 0, 65, 37, 0, 37 );

	timer -> print_seconds( "Took about ", " second(s)." );

	/****************************************************************************************************
	 *                                 Looping through the Ntuple files                                 *
	 ****************************************************************************************************/

	timer -> restart( "Running tree loops..." );
	ntuple_reader -> run();
	timer -> print_seconds( "Took about ", " second(s)." );

	/****************************************************************************************************
	 *                                            Terminating                                           *
	 ****************************************************************************************************/

	delete ntuple_reader;

	std :: cout << process_prompt << process_name << " terminated succesfully." << std :: endl;
	return 0; 

}
