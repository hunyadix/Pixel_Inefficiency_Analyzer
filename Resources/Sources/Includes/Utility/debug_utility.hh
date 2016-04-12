#ifndef DEBUG_UTILITY_H
#define DEBUG_UTILITY_H

#include <iostream>
#include <string>

#include "../Console_actor/common_actors.hh"

void conditional_debug_msg(bool condition, const std::string& msg)
{
	if(condition)
	{
		std::cerr << debug_prompt << msg << std::endl;
	}
}

void conditional_debug_msg(bool condition, std::vector<std::string> msg_parts)
{
	if(condition)
	{
		std::cerr << debug_prompt;
		for(const std::string& msg: msg_parts)
		{
			std::cerr << msg;
		}
		std::cerr << std::endl;
	}
}

#endif // DEBUG_UTILITY_H