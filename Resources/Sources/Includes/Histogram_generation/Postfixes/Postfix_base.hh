#ifndef POSTFIX_BASE_H
#define POSTFIX_BASE_H

#include <string>
#include <functional>
#include "../Root_color_sets/root_color_sets.hh"
#include "../../Console_actor/common_actors.hh"

extern bool debug;

class Postfix
{
	public:
		const char* name;
		std::function<size_t()> sel;
		std::string pf;
		std::string leg;
		std::string colz;
		virtual ~Postfix()
		{
			if(debug)
			{
				std::cout << debug_prompt << "~Postfix_base()" << std::endl; 
			} 
		};
		const char* const get_name() {return this -> name;};
		double operator ()()
		{
			return this -> sel();
		}
};

#endif