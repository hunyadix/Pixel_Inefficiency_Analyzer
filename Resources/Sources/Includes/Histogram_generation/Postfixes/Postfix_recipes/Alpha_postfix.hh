#ifndef ALPHA_POSTFIX_H
#define ALPHA_POSTFIX_H

//#include "../../SmartHistos/SmartHistos.hh"
#include "../Postfix_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

class Alpha_postfix : public Postfix
{
	private:
		float deg;
	public:
		Alpha_postfix(Ntuple_reader*& ntuple_reader_p);
		~Alpha_postfix() 
		{
			if(debug)
			{
				std::cout << debug_prompt << "~Alpha_postfix()" << std::endl; 
			} 
		};
};

Alpha_postfix::Alpha_postfix(Ntuple_reader*& ntuple_reader_p)
{
	this -> name = "Alpha";
	this -> deg = 180 / 3.141516925;
	this -> sel = [&ntuple_reader_p, this]()
	{
		float alpha = std::abs(90 - std::abs(ntuple_reader_p -> get_traj_field_ptr() -> alpha * this -> deg));
		if(90 <= alpha)
		{
			return static_cast<size_t>(-1);
		}
		return static_cast<size_t>(alpha / 15);
	};
	this -> pf = "Alpha[0to75++15]to[15to90++15]";
	this -> leg = "|#alpha| #subset [[0to75++15],[15to90++15]]";
	this -> colz = col6_rainbow_dark;
}

#endif
