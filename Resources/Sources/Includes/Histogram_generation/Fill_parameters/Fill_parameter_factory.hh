#ifndef FILL_PARAMETER_FACTORY_H
#define FILL_PARAMETER_FACTORY_H

#include "Fill_parameter_base.hh"
#include "../../Ntuple_reader/Ntuple_reader.hh"

#include "Fill_parameter_recipes/Ladders_fill_parameter.hh"
#include "Fill_parameter_recipes/Modules_fill_parameter.hh"
#include "Fill_parameter_recipes/Hit_efficiency_fill_parameter.hh"
#include "Fill_parameter_recipes/TrajROCx_fill_parameter.hh"
#include "Fill_parameter_recipes/TrajROCy_fill_parameter.hh"
#include "Fill_parameter_recipes/ClustROCx_fill_parameter.hh"
#include "Fill_parameter_recipes/ClustROCy_fill_parameter.hh"

class Fill_parameter_factory
{
	private:
		static std :: vector<std :: unique_ptr<Fill_parameter>> fill_parameter_list;
	public:
		static std :: unique_ptr<Fill_parameter>& get_fill_parameter( const std :: string& type_p, Ntuple_reader*& ntuple_reader_p );
};


std :: unique_ptr<Fill_parameter>& Fill_parameter_factory :: get_fill_parameter( const std :: string& type_p, Ntuple_reader*& ntuple_reader_p )
{
	if( type_p == "Ladders" )
	{
		fill_parameter_list.push_back( std :: unique_ptr<Fill_parameter>( new Ladders_fill_parameter( ntuple_reader_p ) ) );
		return fill_parameter_list.back();
	}
	if( type_p == "Modules" )
	{
		fill_parameter_list.push_back( std :: unique_ptr<Fill_parameter>( new Modules_fill_parameter( ntuple_reader_p ) ) );
		return fill_parameter_list.back();
	}
	if( type_p == "HitEfficiency" )
	{
		fill_parameter_list.push_back( std :: unique_ptr<Fill_parameter>( new Hit_efficiency_fill_parameter( ntuple_reader_p ) ) );
		return fill_parameter_list.back();
	}
	if( type_p == "TrajROCx" )
	{
		fill_parameter_list.push_back( std :: unique_ptr<Fill_parameter>( new TrajROCx_fill_parameter( ntuple_reader_p ) ) );
		return fill_parameter_list.back();
	}
	if( type_p == "TrajROCy" )
	{
		fill_parameter_list.push_back( std :: unique_ptr<Fill_parameter>( new TrajROCy_fill_parameter( ntuple_reader_p ) ) );
		return fill_parameter_list.back();
	}
	if( type_p == "ClustROCx" )
	{
		fill_parameter_list.push_back( std :: unique_ptr<Fill_parameter>( new ClustROCx_fill_parameter( ntuple_reader_p ) ) );
		return fill_parameter_list.back();
	}
	if( type_p == "ClustROCy" )
	{
		fill_parameter_list.push_back( std :: unique_ptr<Fill_parameter>( new ClustROCy_fill_parameter( ntuple_reader_p ) ) );
		return fill_parameter_list.back();
	}
	std :: cerr << error_prompt << "Failed to generate postfix: " << type_p << ". (Invalid keyword?)" << std :: endl;
	exit( -1 );
	return fill_parameter_list.back(); // never reached
}

std :: vector<std :: unique_ptr<Fill_parameter>> Fill_parameter_factory :: fill_parameter_list;

#endif