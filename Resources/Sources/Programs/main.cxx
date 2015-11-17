/******************************************************************************************************/
/*                                                                                                    */
/*                      Program to loop through TimingStudy Ntuples and perform                       */
/*                      efficiency analysis.                                                          */
/*                      Date: 2015. CMSSW version: CMSSW_7_5_0                                        */
/*                      Author: Adam Hunyadi                                                          */
/*                      E-mail: hunyadix@gmail.com                                                    */
/*                                                                                                    */
/******************************************************************************************************/

#include "../Includes/Ntuple_reader/Ntuple_reader.hh"
#include "../Includes/Timer/Timer_colored.hh"
#include "../Includes/Console_colors/Console_colors.hh"
#include "../Includes/Console_actor/Console_actor.hh"
#include "../Includes/Console_actor/common_actors.hh"

#include "../Includes/Histogram_generation/Postfixes/Postfix_factory.hh"
#include "../Includes/Histogram_generation/Fill_parameters/Fill_parameter_factory.hh"
#include "../Includes/Histogram_generation/Root_color_sets/root_color_sets.hh"

#include "../Includes/Command_line_parsing/main_command_line_parser.hh"

#include "TCanvas.h"
#include "TPad.h"
#include "TH2.h"
#include "TFile.h"
#include "TF1.h"
#include "TTree.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

int calculate_efficiency_limit( int argc, char** argv, Input_output_interface* input_output_interface, double number_of_sigmas_scope, double& badroc_limit );
int calculate_layer_efficiencies( int argc, char** argv, Input_output_interface* input_output_interface, double& badroc_limit );

bool debug = true;

/******************************************************************************************************/
/******************************************************************************************************/
/*                                                                                                    */
/*                                    Important function: main()                                      */
/*                                                                                                    */
/******************************************************************************************************/
/******************************************************************************************************/

int main( int argc, char** argv )
{
	try
	{

		/******************************************************************************************************/
		/*                                       Setting up the project                                       */
		/******************************************************************************************************/

		std :: unique_ptr<Timer> timer( new Timer_colored( timer_prompt ) );
		timer -> restart( "Preparing to run start tree loops..." );
		system( "clear" );
		std :: string process_name( argv[0] );
		if( process_name.find( "./", 0, 2 ) != std :: string :: npos )
		{
			process_name = process_name.substr( process_name.find( "./", 0, 2 ) + 2 );
		}

		double number_of_sigmas_scope  = 4.2919 / 2;
		double badroc_limit            = 0.0; // Calculated in calculate_efficiency_limit().

		std :: string input_file_name;
		std :: string output_file_name;

		parse_main_command_line_arguments( argc, argv, input_file_name, output_file_name );
		std :: cout << process_prompt << process_name << " is being executed..." << std :: endl;

		// Uncomment to override parsed command line arguments
		// input_file_name  = "./Data/Ntuple.root";
		// output_file_name = "./Results/Inefficiency_results.root";

		int schedule = //Ntuple_reader :: Loop_request_flags :: event_tree_loop_request   |
		               //Ntuple_reader :: Loop_request_flags :: lumi_tree_loop_request  |
		               //Ntuple_reader :: Loop_request_flags :: run_tree_loop_request   |
		               Ntuple_reader :: Loop_request_flags :: clust_tree_loop_request |
		               Ntuple_reader :: Loop_request_flags :: traj_tree_loop_request  |
		               0; //

		/******************************************************************************************************/
		/*                                     Initializing the data files                                    */
		/******************************************************************************************************/

		Ntuple_reader* ntuple_reader = new Ntuple_reader( input_file_name );
		ntuple_reader -> set_schedule( schedule );
		ntuple_reader -> set_output_file( output_file_name, "RECREATE" );

		/******************************************************************************************************/
		/*                                        Configuring the run                                         */
		/******************************************************************************************************/

		std :: shared_ptr<Custom_smart_histos> histogram_requests( new Custom_smart_histos() );
		ntuple_reader -> set_histogram_requests( histogram_requests.get() );
		//histogram_requests -> AddHistoType( "eventTree" );
		histogram_requests -> AddHistoType( "clustTree" );
		histogram_requests -> AddHistoType( "trajTree" );
		
		histogram_requests -> AddNewPostfix( Postfix_factory :: get_postfix( "Layers", ntuple_reader ) );

		histogram_requests -> AddNewFillParam( Fill_parameter_factory :: get_fill_parameter( "Ladders", ntuple_reader ) );
		histogram_requests -> AddNewFillParam( Fill_parameter_factory :: get_fill_parameter( "Modules", ntuple_reader ) );
		histogram_requests -> AddNewFillParam( Fill_parameter_factory :: get_fill_parameter( "TrajROCx", ntuple_reader ) );
		histogram_requests -> AddNewFillParam( Fill_parameter_factory :: get_fill_parameter( "TrajROCy", ntuple_reader ) );
		histogram_requests -> AddNewFillParam( Fill_parameter_factory :: get_fill_parameter( "ClustROCx", ntuple_reader ) );
		histogram_requests -> AddNewFillParam( Fill_parameter_factory :: get_fill_parameter( "ClustROCy", ntuple_reader ) );
		histogram_requests -> AddNewFillParam( Fill_parameter_factory :: get_fill_parameter( "HitEfficiency", ntuple_reader ) );
		
		histogram_requests -> SetHistoWeights({});

		/*__________________________________________1D Histograms____________________________________________*/

		//histogram_requests -> AddHistos( "eventTree", { .fill="NTracks",.pfs={}, .cuts={}, .draw="", .opt="", .ranges={} } );
		histogram_requests -> AddHistos( "clustTree", { .fill="Ladders_vs_Modules", .pfs={"Layers"}, .cuts={}, .draw="COLZ", .opt="", .ranges={ 0,0, 0,0, 0.0, 0.0 } });  
		histogram_requests -> AddHistos( "clustTree", { .fill="ClustROCy_vs_ClustROCx", .pfs={"Layers"}, .cuts={}, .draw="COLZ", .opt="", .ranges={ 0,0, 0,0, 0.0, 0.0 } });  
		histogram_requests -> AddHistos( "trajTree",  { .fill="HitEfficiency_vs_Ladders_vs_Modules", .pfs={"Layers"}, .cuts={}, .draw="COLZ", .opt="", .ranges={ 0,0, 0,0, 0.92, 1} });  
		histogram_requests -> AddHistos( "trajTree",  { .fill="HitEfficiency_vs_TrajROCy_vs_TrajROCx", .pfs={"Layers"}, .cuts={}, .draw="COLZ", .opt="", .ranges={ 0,0, 0,0, 0.92, 1} });  

		/*__________________________________________2D Histograms____________________________________________*/


		timer -> print_seconds( "Took about ", " second(s)." );

		/******************************************************************************************************/
		/*                                  Looping through the Ntuple files                                  */
		/******************************************************************************************************/

		timer -> restart( "Running tree loops..." );
		ntuple_reader -> run( );
		timer -> print_seconds( "Took about ", " second(s)." );
		

		int calculate_efficiency_limit_end_results = calculate_efficiency_limit( argc, argv, ntuple_reader, number_of_sigmas_scope, badroc_limit );
		if( calculate_efficiency_limit_end_results == 1 )
		{
			std :: cout << error_prompt << "Function calculate_efficiency_limit() seems to have invalid results. (might ended with failure?)" << std :: endl;
		}
		int calculate_layer_efficiencies_results = calculate_layer_efficiencies( argc, argv, ntuple_reader, badroc_limit );
		if( calculate_layer_efficiencies_results == 1 )
		{
			std :: cout << error_prompt << "Function calculate_layer_efficiencies() seems to have invalid results. (might ended with failure?)" << std :: endl;
		}

		/******************************************************************************************************/
		/*                                            Terminating                                             */
		/******************************************************************************************************/

		// ntuple_reader -> close_output_file(); // unnecessary when Input_output_interface destructor call is present 
		// ntuple_reader -> close_input_file(); // unnecessary when Input_output_interface destructor call is present
		delete ntuple_reader;
		//gDirectory -> Clear();

	}
	catch( std :: exception& e )
	{
		std :: cout << error_prompt << e.what() << std :: endl;
	}
	return 0; 
}

/******************************************************************************************************/
/******************************************************************************************************/
/*                                                                                                    */
/*                           Important function: calculate_efficiency_limit()                         */
/*                                                                                                    */
/******************************************************************************************************/
/******************************************************************************************************/

int calculate_efficiency_limit( int argc, char** argv, Input_output_interface* input_output_interface, double number_of_sigmas_scope, double& badroc_limit )
{
	std :: string process_name( argv[0] );
	if( process_name.find( "./", 0, 2 ) != std :: string :: npos )
	{
		process_name = process_name.substr( process_name.find( "./", 0, 2 ) + 2 );
	}
	std :: cout << process_prompt << process_name << " is now looking for the cut value for good ROCs..." << std :: endl;
	std :: cout << process_prompt << "Good ROC scope: " << std :: setprecision( 2 ) << number_of_sigmas_scope << "." << std :: endl;
	Timer* timer = new Timer_colored( timer_prompt );

	/******************************************************************************************************/
	/*                                     Initializing the data files                                    */
	/******************************************************************************************************/

	input_output_interface -> set_output_directory( "Layer_1_ROC_efficiency_distribution", "CREATE" );

	/******************************************************************************************************/
	/*                          Calculating the minimum efficiency of good ROCS                           */
	/******************************************************************************************************/

	timer -> restart( "Calculating the minimum efficiency of good ROCs..." );
	TH2D* layer_1_hit_efficiencies = static_cast<TH2D*>(input_output_interface -> get_histogram_from_output( "HitEfficiency_vs_TrajROCy_vs_TrajROCx/Layer1" ) );
	TH2D* layer_2_hit_efficiencies = static_cast<TH2D*>(input_output_interface -> get_histogram_from_output( "HitEfficiency_vs_TrajROCy_vs_TrajROCx/Layer2" ) );
	TH2D* layer_3_hit_efficiencies = static_cast<TH2D*>(input_output_interface -> get_histogram_from_output( "HitEfficiency_vs_TrajROCy_vs_TrajROCx/Layer3" ) );
	if( !layer_1_hit_efficiencies || !layer_2_hit_efficiencies || !layer_3_hit_efficiencies )
	{
		std :: cerr << error_prompt << "Error finding the hit efficiency histograms." << std :: endl;
		exit( -1 );
	}
	TH1D* roc_efficiency_distribution = new TH1D( "ROC_efficiency_distribution", "ROC efficiency distribution;efficiency;num_rocs", 1000, 0.0, 1.0 );
	for( int module_ROC_bin = 1; module_ROC_bin <= 72; ++module_ROC_bin )
	{
		for( int ladder_ROC_bin = 1; ladder_ROC_bin <= 90; ++ladder_ROC_bin )
		{
			double current_ROC_efficiency = layer_1_hit_efficiencies -> GetBinContent( module_ROC_bin, ladder_ROC_bin );
			if( current_ROC_efficiency != 0 )
			{
				roc_efficiency_distribution -> Fill( current_ROC_efficiency - 0.0005 );
			}
			current_ROC_efficiency = layer_2_hit_efficiencies -> GetBinContent( module_ROC_bin, ladder_ROC_bin );
			if( current_ROC_efficiency != 0 )
			{
				roc_efficiency_distribution -> Fill( current_ROC_efficiency - 0.0005 );
			}
			current_ROC_efficiency = layer_3_hit_efficiencies -> GetBinContent( module_ROC_bin, ladder_ROC_bin );
			if( current_ROC_efficiency != 0 )
			{
				roc_efficiency_distribution -> Fill( current_ROC_efficiency - 0.0005 );
			}
		}
	}

	TF1* roc_efficiency_distribution_fit = new TF1( "roc_efficiency_distribution_fit", "gaus", 0.0, 1.0 );
	roc_efficiency_distribution_fit -> FixParameter( 0, 0 );
	roc_efficiency_distribution -> Fit( roc_efficiency_distribution_fit, "WWQMC" );
	input_output_interface -> save_histogram_as_canvas( roc_efficiency_distribution );

	roc_efficiency_distribution_fit -> Write();
	std :: cout << process_prompt << "Mean calculated: " << roc_efficiency_distribution_fit -> GetParameter( 1 ) << std :: endl;
	std :: cout << process_prompt << "Sigma parameter calculated: " << roc_efficiency_distribution_fit -> GetParameter( 2 ) << std :: endl;
	badroc_limit = roc_efficiency_distribution_fit -> GetParameter( 1 ) - number_of_sigmas_scope * (roc_efficiency_distribution_fit -> GetParameter( 2 ));
	std :: cout << process_prompt << "Minimum efficiency of good ROCs: " << badroc_limit << std :: endl; 
	timer -> print_seconds( "Took about ", " second(s)." );

	//gDirectory -> Clear();

	return 0;
}

/******************************************************************************************************/
/******************************************************************************************************/
/*                                                                                                    */
/*                         Important function: calculate_layer_efficiencies()                         */
/*                                                                                                    */
/******************************************************************************************************/
/******************************************************************************************************/

int calculate_layer_efficiencies( int argc, char** argv, Input_output_interface* input_output_interface, double& badroc_limit )
{
	std :: string process_name( argv[0] );
	if( process_name.find( "./", 0, 2 ) != std :: string :: npos )
	{
		process_name = process_name.substr( process_name.find( "./", 0, 2 ) + 2 );
	}
	std :: cout << process_prompt << process_name << " is now calculating layer dinamic inefficencies (ignoring static ROC errors)..." << std :: endl;
	std :: cout << process_prompt << "Good ROC definition: has better efficiency than: " << std :: setprecision( 2 ) << badroc_limit << "." << std :: endl;
	Timer* timer = new Timer_colored( timer_prompt );

	/******************************************************************************************************/
	/*                          Calculating the minimum efficiency of good ROCS                           */
	/******************************************************************************************************/

	timer -> restart( "Calculating the minimum efficiency of good ROCs..." );
	TH2D* layer_1_hit_efficiencies = static_cast<TH2D*>(input_output_interface -> get_histogram_from_output( "HitEfficiency_vs_TrajROCy_vs_TrajROCx/Layer1" ) );
	TH2D* layer_2_hit_efficiencies = static_cast<TH2D*>(input_output_interface -> get_histogram_from_output( "HitEfficiency_vs_TrajROCy_vs_TrajROCx/Layer2" ) );
	TH2D* layer_3_hit_efficiencies = static_cast<TH2D*>(input_output_interface -> get_histogram_from_output( "HitEfficiency_vs_TrajROCy_vs_TrajROCx/Layer3" ) );
	if( !layer_1_hit_efficiencies || !layer_2_hit_efficiencies || !layer_3_hit_efficiencies )
	{
		std :: cerr << error_prompt << "Error finding the hit efficiency histograms." << std :: endl;
		exit( -1 );
	}
	std :: function<double( TH2D*, double&, std :: string )> calculate_efficiency = []( TH2D* layer_hit_efficiencies, double& badroc_limit, std :: string layer_name )
	{
		double efficiency_sum    = 0;
		int    efficiency_weight = 0;
		int    num_bad_ROCs      = 0;
		double efficiency_avg    = 0;
		for( int module_ROC_bin = 1; module_ROC_bin <= 72; ++module_ROC_bin )
		{
			for( int ladder_ROC_bin = 1; ladder_ROC_bin <= 90; ++ladder_ROC_bin )
			{
				double current_ROC_efficiency = layer_hit_efficiencies -> GetBinContent( module_ROC_bin, ladder_ROC_bin );
				if( current_ROC_efficiency < badroc_limit )
				{
					++num_bad_ROCs;
					continue;
				}
				efficiency_sum += current_ROC_efficiency;
				++efficiency_weight;
			}
		}
		efficiency_avg = efficiency_sum / efficiency_weight;
		std :: cout << process_prompt << layer_name << ": " << std :: setprecision( 2 ) << efficiency_avg << " Good ROCs: " << efficiency_weight << " Bad ROCs: " << num_bad_ROCs << std :: endl;
		return efficiency_avg;
	};
	calculate_efficiency( layer_1_hit_efficiencies, badroc_limit, "Layer 1" );
	calculate_efficiency( layer_2_hit_efficiencies, badroc_limit, "Layer 2" );
	calculate_efficiency( layer_3_hit_efficiencies, badroc_limit, "Layer 3" );
	// std :: cout << process_prompt << "Layer 2: " << std :: setprecision( 2 ) << layer_2_efficiency << std :: endl;
	// std :: cout << process_prompt << "Layer 3: " << std :: setprecision( 2 ) << layer_3_efficiency << std :: endl;
	timer -> print_seconds( "Took about ", " second(s)." );

	//gDirectory -> Clear();

	return 0;
}