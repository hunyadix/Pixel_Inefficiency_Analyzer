#ifndef CUSTOM_SMART_HISTOS_H
#define CUSTOM_SMART_HISTOS_H

#include "SmartHistos.hh"

#include "../Histogram_generation/Postfixes/Postfix_base.hh"
#include "../Histogram_generation/Fill_parameters/Fill_parameter_base.hh"

#include <memory>

/*
Postfix parameters:

const char* name;
std :: function<size_t()> sel;
std :: string pf;
std :: string leg;
std :: string colz;
*/

class Custom_smart_histos : public SmartHistos
{
	private:
		std :: map<std :: string, std :: unique_ptr<Postfix>> postfix_list; 

	public:
		void AddNewPostfix( Postfix* pf_p )
		{
			SmartHistos :: AddNewPostfix( std :: move( pf_p -> name ), std :: move( pf_p -> sel ), std :: move( pf_p -> pf ), std :: move( pf_p -> leg ), std :: move( pf_p -> colz ) );
		};
		void AddNewPostfix( std :: unique_ptr<Postfix>& pf_p )
		{
/*			const char* name = pf_p -> name.c_str();
			std :: cout << debug_prompt << "adding postfix with name: " << pf_p -> name.c_str() << std :: endl;
*/			SmartHistos :: AddNewPostfix( std :: move( pf_p -> name ), std :: move( pf_p -> sel ), std :: move( pf_p -> pf ), std :: move( pf_p -> leg ), std :: move( pf_p -> colz ) );
		};
		void AddNewFillParam( Fill_parameter* pf_p )
		{
			SmartHistos :: AddNewFillParam( pf_p -> name, SmartHistos::FillParams( { .nbin = std :: move( pf_p -> nbin ) , .bins = std :: move( pf_p -> bins ), .fill = std :: move( pf_p -> fill ), .axis_title = std :: move( pf_p -> axis_title ) } ) );
		};
		void AddNewFillParam( std :: unique_ptr<Fill_parameter>& pf_p )
		{
			SmartHistos :: AddNewFillParam( pf_p -> name, SmartHistos::FillParams( { .nbin = std :: move( pf_p -> nbin ) , .bins = std :: move( pf_p -> bins ), .fill = std :: move( pf_p -> fill ), .axis_title = std :: move( pf_p -> axis_title ) } ) );
		};
};

#endif