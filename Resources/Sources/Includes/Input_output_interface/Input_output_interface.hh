#ifndef INPUT_OUTPUT_INTERFACE_H
#define INPUT_OUTPUT_INTERFACE_H

#include "../Console_actor/common_actors.hh"

#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"

#include <memory>

extern bool debug;

class Input_output_interface
{
	protected:
		std :: unique_ptr<TFile> input_file;
		std :: unique_ptr<TFile> output_file;
		
	public:
		Input_output_interface();
		virtual ~Input_output_interface();

		void open_input_file( std :: string filename_p );
		void change_input_file( std :: string filename_p );
		void close_input_file();
		void set_output_file( std :: string filename_p, std :: string options_p = "RECREATE" );
		void change_output_file( std :: string filename_p, std :: string options_p = "RECREATE" );
		void set_output_directory( std :: string dir_p, std :: string options_p = "" );
		void close_output_file();
		void save_histogram( TH1* histo_p );
		void save_histogram( TH2* histo_p );
		void save_histogram_as_canvas( TH1* histo_p );
		void save_histogram_as_canvas( TH2* histo_p );
		TObject* get_histogram_from_output( const std :: string& name_p );
		TObject* get_histogram_from_input( const std :: string& name_p );
};

Input_output_interface :: Input_output_interface()
{

}

Input_output_interface :: ~Input_output_interface()
{
	if( debug )
	{
		std :: cout << debug_prompt << "~Input_output_interface()..." << std :: endl;
	}
	if( this -> input_file -> IsOpen() )
	{
		this -> close_input_file();
	}
	if( this -> output_file -> IsOpen() )
	{
		this -> close_output_file();
	}
	//std :: cout << "Debug: Input_output_interface destructor call." << std :: endl;
}

void Input_output_interface :: open_input_file( std :: string filename_p )
{
	this -> input_file = std :: unique_ptr<TFile>( new TFile( filename_p.c_str(), "READ" ) );
	if( !((this -> input_file) -> IsOpen()) )
	{
		std :: cerr << "Warning! Error opening the input file: " << filename_p << "." << std :: endl;
	}
}

void Input_output_interface :: change_input_file( std :: string filename_p )
{
	this -> close_input_file();
	this -> input_file.reset( new TFile( filename_p.c_str(), "READ" ) );
}

void Input_output_interface :: set_output_file( std :: string filename_p, std :: string options_p )
{
	this -> output_file = std :: unique_ptr<TFile>( new TFile( filename_p.c_str(), options_p.c_str() ) );
	if( !((this -> output_file) -> IsOpen()) )
	{
		std :: cerr << "Warning! Error opening the output file: " << filename_p << "." << std :: endl;
	}
}

void Input_output_interface :: change_output_file( std :: string filename_p, std :: string options_p )
{
	this -> close_output_file();
	this -> output_file.reset( new TFile( filename_p.c_str(), options_p.c_str() ) );
}

void Input_output_interface :: set_output_directory( std :: string dir_p, std :: string options_p )
{
	this -> output_file -> cd();
	if( options_p.find( "CREATE" ) != std :: string :: npos )
	{
		this -> output_file -> mkdir( dir_p.c_str() );
	}
	this -> output_file -> cd( dir_p.c_str() );
}

void Input_output_interface :: save_histogram( TH1* histo_p )
{
	histo_p -> Write();
}

void Input_output_interface :: save_histogram( TH2* histo_p )
{
	histo_p -> Write();
}

void Input_output_interface :: save_histogram_as_canvas( TH1* histo_p )
{
	std :: string canvas_name( histo_p -> GetName() );
	canvas_name += "_canvas";
	std :: unique_ptr<TCanvas> canvas( new TCanvas( canvas_name.c_str(), canvas_name.c_str() ) );
	canvas -> cd();
	histo_p -> Draw( "Hist" );
	canvas -> Write();
}

void Input_output_interface :: save_histogram_as_canvas( TH2* histo_p )
{
	std :: string canvas_name( histo_p -> GetName() );
	canvas_name += "_canvas";
	std :: unique_ptr<TCanvas> canvas( new TCanvas( canvas_name.c_str(), canvas_name.c_str() ) );
	canvas -> cd();
	histo_p -> Draw( "COLZ" );
	canvas -> Write(); // please
}

void Input_output_interface :: close_input_file()
{
	if( this -> input_file -> IsOpen() )
	{
		this -> input_file -> Close();
	}
	else
	{
		std :: cout << "Warning: The attempt to close the input_file failed (maybe it was already closed?)..." << std :: endl;
	}
}

void Input_output_interface :: close_output_file()
{
	if( this -> output_file -> IsOpen() )
	{
		this -> output_file -> Close();
	}
	else
	{
		std :: cout << "Warning: The attempt to close the output_file failed (maybe it was already closed?)..." << std :: endl;
	}
}

TObject* Input_output_interface :: get_histogram_from_input( const std :: string& name_p )
{
	return this -> input_file -> Get( name_p.c_str() );
}

TObject* Input_output_interface :: get_histogram_from_output( const std :: string& name_p )
{
	return this -> output_file -> Get( name_p.c_str() );
}


#endif