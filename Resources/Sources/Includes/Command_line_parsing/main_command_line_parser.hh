#ifndef MAIN_COMMAND_LINE_PARSER_H
#define MAIN_COMMAND_LINE_PARSER_H

#include "tclap/include/tclap/CmdLine.h"

int parse_main_command_line_arguments( int argc, char** argv, std :: string& input_file, std :: string& output_file )
{
	try 
	{  
		// Define the command line object.
		TCLAP :: CmdLine cmd( "Command description message", ' ', "1.0" );

		// Define a value argument and add it to the command line.
		TCLAP :: ValueArg<std :: string> input_file_arg( "i", "input-file", "Path and name of the input Ntuple containing trees", false, "./Data/Ntuple.root", "string" );
		cmd.add( input_file_arg );
		TCLAP :: ValueArg<std :: string> output_file_arg( "o", "output-file", "Path and name of generate the output-file", false, "./Results/Inefficiency_results.root", "string" );
		cmd.add( output_file_arg );

		// Define a switch and add it to the command line.
		/*SwitchArg reverseSwitch("r","reverse","Print name backwards", false);
		cmd.add( reverseSwitch );*/

		// Parse the args.
		cmd.parse( argc, argv );

		// Get the value parsed by each arg. 
		output_file = output_file_arg.getValue();
		input_file  = input_file_arg .getValue();

		/*if ( reverseName )
		{
			reverse(name.begin(),name.end());
			cout << "My name (spelled backwards) is: " << name << endl;
		}
		else
			cout << "My name is: " << name << endl;*/
	}
	catch( TCLAP :: ArgException &e )  // catch any exceptions
	{
		std :: cerr << "error: " << e.error() << " for arg " << e.argId() << std :: endl;
	}

	return 0;
}

#endif