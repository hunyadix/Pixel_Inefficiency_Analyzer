#ifndef POSTFIX_BASE_H
#define POSTFIX_BASE_H

#include <string>
#include <functional>
#include "../Root_color_sets/root_color_sets.hh"

extern bool debug;

class Postfix
{
	public:
		const char* name;
		std :: function<size_t()> sel;
		std :: string pf;
		std :: string leg;
		std :: string colz;
		virtual ~Postfix()
		{
			if( debug )
			{
				std :: cout << debug_prompt << "~Postfix_base()" << std :: endl; 
			} 
		};
};

#endif