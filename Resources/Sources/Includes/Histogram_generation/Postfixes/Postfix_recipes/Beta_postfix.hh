#ifndef BETA_POSTFIX_H
#define BETA_POSTFIX_H

//#include "../../SmartHistos/SmartHistos.hh"
#include "../Postfix_base.hh"
#include "../../../Ntuple_reader/Ntuple_reader.hh"

#include "../../../Console_actor/Console_actor.hh"

#include <string>
#include <functional>

extern bool debug;

class Beta_postfix : public Postfix
{
	private:
		float deg;
	public:
		Beta_postfix(Ntuple_reader*& ntuple_reader_p);
		~Beta_postfix() 
		{
			if(debug)
			{
				std::cout << debug_prompt << "~Beta_postfix()" << std::endl; 
			} 
		};
};

Beta_postfix::Beta_postfix(Ntuple_reader*& ntuple_reader_p)
{
	this -> name = "Beta";
	this -> deg = 180 / 3.141516925;
	this -> sel = [&ntuple_reader_p, this]()
	{
		float beta = std::abs(90 - std::abs(ntuple_reader_p -> get_traj_field_ptr() -> beta * this -> deg));
		if(90 <= beta)
		{
			return static_cast<size_t>(-1);
		}
		return static_cast<size_t>(beta / 15);
	};
	this -> pf = "Beta[0to75++15]to[15to90++15]";
	this -> leg = "|#beta| #subset [[0to75++15],[15to90++15]]";
	this -> colz = col6_rainbow_dark;
}

#endif
