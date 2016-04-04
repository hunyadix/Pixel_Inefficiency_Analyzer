#ifndef FILL_PARAMETER_BASE_H
#define FILL_PARAMETER_BASE_H

#include <string>
#include <functional>
#include "../Root_color_sets/root_color_sets.hh"

extern bool debug;

class Fill_parameter
{
	public:
		std::string name;
		size_t nbin; 
		std::vector<double> bins;
		std::function<double()> fill;
		std::string axis_title;
		virtual ~Fill_parameter()
		{
			if(debug)
			{
				std::cout << debug_prompt << "~Fill_parameter()" << std::endl; 
			}
		};
		void set_name(std::string name_p)
		{
			this -> name = name_p; 
		}
		double operator ()()
		{
			return this -> fill();
		}
};

#endif